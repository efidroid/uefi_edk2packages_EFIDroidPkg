#include <PiDxe.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomTargetMmcSdhciLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/FdtClient.h>
#include <Chipset/mmc_sdhci.h>

#include <stdlib.h>

#define SDC1_HDRV_PULL_CTL           (PcdGet64(PcdGpioTlmmBaseAddress) + PcdGet64(PcdMmcSdc1HdrvPullCtlOffset))
#define SDC2_HDRV_PULL_CTL           (PcdGet64(PcdGpioTlmmBaseAddress) + PcdGet64(PcdMmcSdc2HdrvPullCtlOffset))

VOID LibQcomTargetMmcSdhciInit(INIT_SLOT_CB InitSlot)
{
  EFI_STATUS                     Status, FindNodeStatus;
  FDT_CLIENT_PROTOCOL            *FdtClient;
  INT32                          Node;
  UINTN                          Index;
  UINT32                         PropertySize;

  // get fdt client
  Status = gBS->LocateProtocol (&gFdtClientProtocolGuid, NULL, (VOID **)&FdtClient);
  ASSERT_EFI_ERROR (Status);

  // iterate over all gpio keys
  for (FindNodeStatus = FdtClient->FindCompatibleNode (FdtClient, "qcom,sdhci-msm", &Node);
       !EFI_ERROR (FindNodeStatus);
       FindNodeStatus = FdtClient->FindNextCompatibleNode (FdtClient, "qcom,sdhci-msm", Node, &Node))
  {
    struct mmc_config_data config = {0};
    CONST UINT32  *FdtU32;
    CONST CHAR8   *FdtChar8;
    CONST UINT32  *FdtPinCtrlArray;
    UINTN NumPinCtrls;
    UINT32 tlmm_reg = 0;
    UINT32 clk_hdrv_off;
    UINT32 cmd_hdrv_off;
    UINT32 data_hdrv_off;
    UINT32 clk_pull_off;
    UINT32 cmd_pull_off;
    UINT32 data_pull_off;
    UINT32 rclk_pull_off;
    UINT8 clk_drv;
    UINT8 cmd_drv;
    UINT8 dat_drv;
    UINT8 clk_pull;
    UINT8 cmd_pull;
    UINT8 dat_pull;
    UINT8 rclk_pull;
    BOOLEAN FoundClk = FALSE;
    BOOLEAN FoundCmd = FALSE;
    BOOLEAN FoundDat = FALSE;
    BOOLEAN FoundRclk = FALSE;
    BOOLEAN IsError = FALSE;

    // get bus width
    Status = FdtClient->GetNodeProperty(FdtClient, Node, "qcom,bus-width", (CONST VOID **)&FdtU32, NULL);
    if (EFI_ERROR (Status)) {
      continue;
    }

    // parse bus width
    switch (SwapBytes32 (*FdtU32)) {
      case 0x8:
        config.bus_width = DATA_BUS_WIDTH_8BIT;
        break;
      case 0x4:
        config.bus_width = DATA_BUS_WIDTH_4BIT;
        break;
      default:
        config.bus_width = DATA_BUS_WIDTH_1BIT;
        break;
    }
    DEBUG ((DEBUG_INFO, "bus-width: %x\n", config.bus_width));

    // get clock rates
    Status = FdtClient->GetNodeProperty (FdtClient, Node, "qcom,clk-rates", (CONST VOID **)&FdtU32, &PropertySize);
    if (EFI_ERROR (Status) || (PropertySize%sizeof(*FdtU32))!=0) {
      continue;
    }

    // use highest clock rate
    for (Index = 0; Index < PropertySize/sizeof(*FdtU32); Index++) {
      UINT32 ClockRate = SwapBytes32 (FdtU32[Index]);
      if (ClockRate > config.max_clk_rate)
        config.max_clk_rate = ClockRate;
    }
    DEBUG ((DEBUG_INFO, "max_clk_rate: %u\n", config.max_clk_rate));

    // bus name
    Status = FdtClient->GetNodeProperty(FdtClient, Node, "qcom,msm-bus,name", (CONST VOID **)&FdtChar8, NULL);
    if (EFI_ERROR (Status) || AsciiStrnCmp(FdtChar8, "sdhc", 4) != 0) {
      continue;
    }

    // get slot id
    config.slot = atoul(&FdtChar8[4]);
    DEBUG ((DEBUG_INFO, "slot: %u\n", config.slot));

    // get regs
    Status = FdtClient->GetNodeProperty (FdtClient, Node, "reg", (CONST VOID **)&FdtU32, &PropertySize);
    if (EFI_ERROR (Status) || PropertySize != sizeof(*FdtU32)*4) {
      continue;
    }

    // get base addresses
    config.sdhc_base = SwapBytes32 (FdtU32[0]);
    config.pwrctl_base = SwapBytes32 (FdtU32[2]);
    DEBUG ((DEBUG_INFO, "sdhc_base: %x\n", config.sdhc_base));
    DEBUG ((DEBUG_INFO, "pwrctl_base: %x\n", config.pwrctl_base));

    // get interrupts
    Status = FdtClient->GetNodeProperty (FdtClient, Node, "interrupts", (CONST VOID **)&FdtU32, &PropertySize);
    if (EFI_ERROR (Status) || PropertySize != sizeof(*FdtU32)*6) {
      continue;
    }

    // get power irq
    config.pwr_irq = PcdGet64(PcdGicSpiStart) + SwapBytes32 (FdtU32[4]);
    DEBUG ((DEBUG_INFO, "pwr_irq: %d\n", config.pwr_irq));

    config.hs200_support = 0;
    config.hs400_support = 0;
    config.use_io_switch = 0;

    // get speed mode
    Status = FdtClient->GetNodeProperty(FdtClient, Node, "qcom,bus-speed-mode", (CONST VOID **)&FdtChar8, &PropertySize);
    if (!EFI_ERROR (Status)) {
      for (FdtChar8 = FdtChar8; FdtChar8 < FdtChar8 + PropertySize && *FdtChar8;
         FdtChar8 += 1 + AsciiStrLen (FdtChar8)) {
        if (AsciiStrnCmp ("HS400", FdtChar8, 5) == 0) {
          config.hs400_support = 1;
        }
        else if (AsciiStrnCmp ("HS200", FdtChar8, 5) == 0) {
          config.hs200_support = 1;
        }
      }
    }
    DEBUG((DEBUG_INFO, "hs200: %d  hs400: %d\n", config.hs200_support, config.hs400_support));

    // get pinctrl
    Status = FdtClient->GetNodeProperty (FdtClient, Node, "pinctrl-0", (CONST VOID **)&FdtPinCtrlArray, &PropertySize);
    if (EFI_ERROR (Status) || (PropertySize%sizeof(*FdtPinCtrlArray))!=0) {
      continue;
    }
    NumPinCtrls = PropertySize/sizeof(*FdtPinCtrlArray);

    for (Index = 0; Index < NumPinCtrls; Index++) {
      INT32 PinCtrlNode;
      INT32 PinCtrlParentNode;
      UINT8 PinCtrlPull;
      UINT32 DriveStrengthMA;
      UINT8 DriveStrength;
      BOOLEAN HasDriveStrength = FALSE;

      // get pinctrl node
      Status = FdtClient->FindNodeByPHandle(FdtClient, SwapBytes32(FdtPinCtrlArray[Index]), &PinCtrlNode);
      if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "Can't find node for pinctrl phandle\n"));
        IsError = TRUE;
        break;
      }

      // get parent node
      Status = FdtClient->GetParentNode(FdtClient, PinCtrlNode, &PinCtrlParentNode);
      if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "Can't find parent node of pinctrl\n"));
        IsError = TRUE;
        break;
      }

      // get label
      Status = FdtClient->GetNodeProperty(FdtClient, PinCtrlParentNode, "label", (CONST VOID **)&FdtChar8, NULL);
      if (EFI_ERROR (Status)) {
        IsError = TRUE;
        break;
      }
      DEBUG((DEBUG_ERROR, "label: %a\n", FdtChar8));

      // get pull
      if (FdtClient->HasNodeProperty(FdtClient, PinCtrlNode, "bias-disable"))
        PinCtrlPull = TLMM_NO_PULL;
      else if (FdtClient->HasNodeProperty(FdtClient, PinCtrlNode, "bias-pull-up"))
        PinCtrlPull = TLMM_PULL_UP;
      else if (FdtClient->HasNodeProperty(FdtClient, PinCtrlNode, "bias-pull-down"))
        PinCtrlPull = TLMM_PULL_DOWN;
      else
        PinCtrlPull = TLMM_NO_PULL;

      // get drive strength
      Status = FdtClient->GetNodeProperty (FdtClient, PinCtrlNode, "drive-strength", (CONST VOID **)&FdtU32, &PropertySize);
      if (!EFI_ERROR (Status) && PropertySize==sizeof(*FdtU32)) {
        HasDriveStrength = TRUE;
        DriveStrengthMA = SwapBytes32(FdtU32[0]);

        switch (DriveStrengthMA) {
          case 16:
            DriveStrength = TLMM_CUR_VAL_16MA;
            break;
          case 14:
            DriveStrength = TLMM_CUR_VAL_14MA;
            break;
          case 12:
            DriveStrength = TLMM_CUR_VAL_12MA;
            break;
          case 10:
            DriveStrength = TLMM_CUR_VAL_10MA;
            break;
          case 8:
            DriveStrength = TLMM_CUR_VAL_8MA;
            break;
          case 6:
            DriveStrength = TLMM_CUR_VAL_6MA;
            break;
          case 4:
            DriveStrength = TLMM_CUR_VAL_4MA;
            break;
          case 2:
            DriveStrength = TLMM_CUR_VAL_2MA;
            break;
          default:
            DEBUG((DEBUG_ERROR, "Invalid drive strength: %u\n", DriveStrengthMA));
            IsError = TRUE;
            break;
        }
      }

      if (IsError)
        break;

      if (!AsciiStrCmp(FdtChar8+5, "clk")) {
        if (!HasDriveStrength) {
          DEBUG((DEBUG_ERROR, "drive strength is needed for clk\n"));
          IsError = TRUE;
          break;
        }

        clk_drv = DriveStrength;
        clk_pull = PinCtrlPull;
        FoundClk = TRUE;
      }
      else if (!AsciiStrCmp(FdtChar8+5, "cmd")) {
        if (!HasDriveStrength) {
          DEBUG((DEBUG_ERROR, "drive strength is needed for cmd\n"));
          IsError = TRUE;
          break;
        }

        cmd_drv = DriveStrength;
        cmd_pull = PinCtrlPull;
        FoundCmd = TRUE;
      }
      else if (!AsciiStrCmp(FdtChar8+5, "data")) {
        if (!HasDriveStrength) {
          DEBUG((DEBUG_ERROR, "drive strength is needed for data\n"));
          IsError = TRUE;
          break;
        }

        dat_drv = DriveStrength;
        dat_pull = PinCtrlPull;
        FoundDat = TRUE;
      }
      else if (!AsciiStrCmp(FdtChar8+5, "rclk")) {
        if (HasDriveStrength) {
          DEBUG((DEBUG_WARN, "drive strength is not supported for rclk\n"));
        }

        rclk_pull = PinCtrlPull;
        FoundRclk = TRUE;
      }
    }

    if (IsError || !FoundClk || !FoundCmd || !FoundDat || !FoundRclk) {
      DEBUG((DEBUG_ERROR, "incomplete pinctrl\n"));
      continue;
    }

    if (config.slot == 0x1)
    {
      tlmm_reg = SDC1_HDRV_PULL_CTL;
      clk_hdrv_off = SDC1_CLK_HDRV_CTL_OFF;
      cmd_hdrv_off = SDC1_CMD_HDRV_CTL_OFF;
      data_hdrv_off = SDC1_DATA_HDRV_CTL_OFF;
      clk_pull_off = SDC1_CLK_PULL_CTL_OFF;
      cmd_pull_off = SDC1_CMD_PULL_CTL_OFF;
      data_pull_off = SDC1_DATA_PULL_CTL_OFF;
      rclk_pull_off = SDC1_RCLK_PULL_CTL_OFF;
    }
    else if (config.slot == 0x2)
    {
      tlmm_reg = SDC2_HDRV_PULL_CTL;
      clk_hdrv_off = SDC2_CLK_HDRV_CTL_OFF;
      cmd_hdrv_off = SDC2_CMD_HDRV_CTL_OFF;
      data_hdrv_off = SDC2_DATA_HDRV_CTL_OFF;
      clk_pull_off = SDC2_CLK_PULL_CTL_OFF;
      cmd_pull_off = SDC2_CMD_PULL_CTL_OFF;
      data_pull_off = SDC2_DATA_PULL_CTL_OFF;
    }
    else
    {
      DEBUG((DEBUG_ERROR, "Unsupported SDC slot passed: %u\n", config.slot));
      return;
    }

    /* Drive strength configs for sdc pins */
    struct tlmm_cfgs sdc_hdrv_cfg[] =
    {
      { clk_hdrv_off,  clk_drv, TLMM_HDRV_MASK, tlmm_reg },
      { cmd_hdrv_off,  cmd_drv, TLMM_HDRV_MASK, tlmm_reg },
      { data_hdrv_off, dat_drv, TLMM_HDRV_MASK, tlmm_reg },
    };

    /* Pull configs for sdc pins */
    struct tlmm_cfgs sdc_pull_cfg[] =
    {
      { clk_pull_off,  clk_pull, TLMM_PULL_MASK, tlmm_reg },
      { cmd_pull_off,  cmd_pull, TLMM_PULL_MASK, tlmm_reg },
      { data_pull_off, dat_pull, TLMM_PULL_MASK, tlmm_reg },
    };

    /* Set the drive strength & pull control values */
    gGpioTlmm->tlmm_set_hdrive_ctrl(sdc_hdrv_cfg, ARRAY_SIZE(sdc_hdrv_cfg));
    gGpioTlmm->tlmm_set_pull_ctrl(sdc_pull_cfg, ARRAY_SIZE(sdc_pull_cfg));

    if (config.slot == 0x1)
    {
      struct tlmm_cfgs sdc_rclk_cfg[] =
      {
        { rclk_pull_off, rclk_pull, TLMM_PULL_MASK, tlmm_reg },
      };

      gGpioTlmm->tlmm_set_pull_ctrl(sdc_rclk_cfg, ARRAY_SIZE(sdc_rclk_cfg));
    }

    if (InitSlot(&config) == NULL) {
      DEBUG((DEBUG_ERROR, "Can't initialize mmc slot %u\n", config.slot));
    }
  }
}
