#include <PiDxe.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomPlatformI2cQupLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/QcomClockLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/FdtClient.h>

#include <stdlib.h>

STATIC   FDT_CLIENT_PROTOCOL *mFdtClient = NULL;

STATIC
EFI_STATUS
EFIAPI
LibQcomPlatformI2cQupBlspClockConfig (
  UINT32 QupBase
  )
{
  uint8_t ret = 0;
  char clk_name[64];
  struct clk *qup_clk;

  snprintf(clk_name, sizeof(clk_name), "qup%08x_ahb_iface_clk", QupBase);
  ret = gClock->clk_get_set_enable(clk_name, 0, 1);
  if (ret) {
    DEBUG((DEBUG_ERROR, "Failed to enable %a clock\n", clk_name));
    return EFI_DEVICE_ERROR;
  }

  snprintf(clk_name, sizeof(clk_name), "gcc_qup%08x_i2c_apps_clk", QupBase);
  qup_clk = gClock->clk_get(clk_name);
  if (!qup_clk) {
    DEBUG((DEBUG_ERROR, "Failed to get %a\n", clk_name));
    return EFI_NOT_FOUND;
  }

  ret = gClock->clk_enable(qup_clk);
  if (ret) {
    DEBUG((DEBUG_ERROR, "Failed to enable %a\n", clk_name));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LibQcomPlatformI2cQupAddBusses (
  REGISTER_I2C_DEVICE RegisterI2cDevice
  )
{
  EFI_STATUS                     Status, FindNodeStatus;
  INT32                          Node;
  UINT32                         PropertySize;

  // iterate over all gpio keys
  for (FindNodeStatus = mFdtClient->FindCompatibleNode (mFdtClient, "qcom,i2c-msm-v2", &Node);
       !EFI_ERROR (FindNodeStatus);
       FindNodeStatus = mFdtClient->FindNextCompatibleNode (mFdtClient, "qcom,i2c-msm-v2", Node, &Node))
  {
    CONST UINT32 *FdtU32;
    CONST UINT32 *FdtPinCtrlArray;
    UINT32 QupBase;
    UINT32 QupIrq;
    INT32 PinCtrl0Node;
    UINTN NumPinCtrls;
    UINT32 FreqOut;
    UINT32 FreqIn;
    UINTN i, j;
    CONST CHAR8 *Alias = NULL;
    INTN DeviceId = -1;

    Status = mFdtClient->FindAliasForNode (mFdtClient, Node, &Alias);
    if (!EFI_ERROR (Status) && Alias != NULL) {
      if (!AsciiStrnCmp(Alias, "i2c", 3)) {
        DeviceId = atoi(Alias+3);
      }
    }

    // get base address
    Status = mFdtClient->GetNodeProperty (mFdtClient, Node, "reg", (CONST VOID **)&FdtU32, &PropertySize);
    if (EFI_ERROR (Status) || PropertySize!=sizeof(*FdtU32)*2) {
      DEBUG((DEBUG_ERROR, "Invalid property size for 'reg': %u\n", PropertySize));
      continue;
    }
    QupBase = SwapBytes32(FdtU32[0]);

    // get irq
    Status = mFdtClient->GetNodeProperty (mFdtClient, Node, "interrupts", (CONST VOID **)&FdtU32, &PropertySize);
    if (EFI_ERROR (Status) || PropertySize!=sizeof(*FdtU32)*3) {
      DEBUG((DEBUG_ERROR, "Invalid property size for 'interrupts': %u\n", PropertySize));
      continue;
    }
    QupIrq = SwapBytes32(FdtU32[1]);

    // get output freq
    Status = mFdtClient->GetNodeProperty (mFdtClient, Node, "qcom,clk-freq-out", (CONST VOID **)&FdtU32, &PropertySize);
    if (EFI_ERROR (Status) || PropertySize!=sizeof(*FdtU32)) {
      DEBUG((DEBUG_ERROR, "Invalid property size for 'qcom,clk-freq-out': %u\n", PropertySize));
      continue;
    }
    FreqOut = SwapBytes32(FdtU32[0]);

    // get input freq
    Status = mFdtClient->GetNodeProperty (mFdtClient, Node, "qcom,clk-freq-in", (CONST VOID **)&FdtU32, &PropertySize);
    if (EFI_ERROR (Status) || PropertySize!=sizeof(*FdtU32)) {
      DEBUG((DEBUG_ERROR, "Invalid property size for 'qcom,clk-freq-in': %u\n", PropertySize));
      continue;
    }
    FreqIn = SwapBytes32(FdtU32[0]);

    // get pinctrl array
    Status = mFdtClient->GetNodeProperty (mFdtClient, Node, "pinctrl-0", (CONST VOID **)&FdtPinCtrlArray, &PropertySize);
    if (EFI_ERROR (Status) || PropertySize%sizeof(*FdtPinCtrlArray)) {
      DEBUG((DEBUG_ERROR, "Invalid property size for 'pinctrl-0': %u\n", PropertySize));
      continue;
    }
    NumPinCtrls = PropertySize/sizeof(*FdtPinCtrlArray);

    DEBUG((DEBUG_INFO, "id: %d base: %08x irq: %u num_pinctrls: %u\n", DeviceId, QupBase, QupIrq, NumPinCtrls));

    for (i=0; i<NumPinCtrls; i++) {
      UINT32 DriveStrength;
      GPIO_PULL Pull;
      BOOLEAN SetPull = TRUE;
      INTN PinCtrlParentNode;
      UINT32 PinFunction;
      UINT32 NumPins;
      CONST UINT32 *FdtPinArray;

      // get pinctrl node
      Status = mFdtClient->FindNodeByPHandle(mFdtClient, SwapBytes32(FdtPinCtrlArray[i]), &PinCtrl0Node);
      if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "Can't find node for pinctrl phandle\n"));
        continue;
      }

      // get drive strength
      Status = mFdtClient->GetNodeProperty (mFdtClient, PinCtrl0Node, "drive-strength", (CONST VOID **)&FdtU32, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize!=sizeof(*FdtU32)) {
        DEBUG((DEBUG_ERROR, "Invalid property size for 'drive-strength': %u\n", PropertySize));
        continue;
      }
      DriveStrength = SwapBytes32(FdtU32[0]);

      if (mFdtClient->HasNodeProperty(mFdtClient, PinCtrl0Node, "bias-disable"))
        Pull = GPIO_PULL_NONE;
      else if (mFdtClient->HasNodeProperty(mFdtClient, PinCtrl0Node, "bias-bus-hold"))
        Pull = GPIO_PULL_KEEPER;
      else if (mFdtClient->HasNodeProperty(mFdtClient, PinCtrl0Node, "bias-pull-up"))
        Pull = GPIO_PULL_UP;
      else if (mFdtClient->HasNodeProperty(mFdtClient, PinCtrl0Node, "bias-pull-down"))
        Pull = GPIO_PULL_DOWN;
      else
        SetPull = FALSE;

      // get pinctrl node
      Status = mFdtClient->GetParentNode(mFdtClient, PinCtrl0Node, &PinCtrlParentNode);
      if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "Can't find parent node of pinctrl\n"));
        continue;
      }

      // TODO: check if this a tlmm gpio

      // get func
      Status = mFdtClient->GetNodeProperty (mFdtClient, PinCtrlParentNode, "qcom,pin-func", (CONST VOID **)&FdtU32, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize!=sizeof(*FdtU32)) {
        DEBUG((DEBUG_ERROR, "Invalid property size for 'qcom,pin-func': %u\n", PropertySize));
        continue;
      }
      PinFunction = SwapBytes32(FdtU32[0]);

      // get number of pins
      Status = mFdtClient->GetNodeProperty (mFdtClient, PinCtrlParentNode, "qcom,num-grp-pins", (CONST VOID **)&FdtU32, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize!=sizeof(*FdtU32)) {
        DEBUG((DEBUG_ERROR, "Invalid property size for 'qcom,num-grp-pins': %u\n", PropertySize));
        continue;
      }
      NumPins = SwapBytes32(FdtU32[0]);

      DEBUG((DEBUG_INFO, "ds=%02x pull=%d fn=%u npins=%u fout=%u fin=%u\n", DriveStrength, Pull, PinFunction, NumPins, FreqOut, FreqIn));

      // get pin array
      Status = mFdtClient->GetNodeProperty (mFdtClient, PinCtrlParentNode, "qcom,pins", (CONST VOID **)&FdtPinArray, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize < NumPins*(sizeof(*FdtPinArray)*2)) {
        DEBUG((DEBUG_ERROR, "Invalid property size for 'qcom,num-grp-pins': %u\n", PropertySize));
        continue;
      }

      for(j=0; j<NumPins; j++) {
        UINT32 PinId = SwapBytes32(FdtPinArray[j*2 + 1]);

        // gpio config
        gGpioTlmm->SetFunction(PinId, PinFunction);
        gGpioTlmm->SetDriveStrength(PinId, DriveStrength);
        if (SetPull) {
          gGpioTlmm->SetPull(PinId, DriveStrength);
        }
      }

    }

    // clock config
    Status = LibQcomPlatformI2cQupBlspClockConfig(QupBase);
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "Can't configure clocks for i2c device\n"));
      continue;
    }

    struct qup_i2c_dev *dev;

    dev = AllocateZeroPool(sizeof(struct qup_i2c_dev));
    if (!dev) {
      DEBUG((DEBUG_ERROR, "Can't allocate memory for i2c device\n"));
      continue;
    }

    dev->id = DeviceId;
    dev->qup_irq = PcdGet64(PcdGicSpiStart) + QupIrq;
    dev->qup_base = QupBase;

    dev->device_type = I2C_QUP_DEVICE_TYPE_BLSP;
    Status = RegisterI2cDevice(dev, FreqOut, FreqIn);
    if (EFI_ERROR(Status)) {
      continue;
    }

    DEBUG((DEBUG_INFO, "Registered i2c device 0x%08x with ID %u\n", dev->qup_base, dev->id));
  }

  return EFI_SUCCESS;
}


RETURN_STATUS
EFIAPI
PlatformI2cQupLibConstructor (
  VOID
  )
{
  EFI_STATUS  Status;

  // get fdt client
  Status = gBS->LocateProtocol (&gFdtClientProtocolGuid, NULL, (VOID **)&mFdtClient);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
