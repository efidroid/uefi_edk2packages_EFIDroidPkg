#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomPm8x41Lib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/KeypadDeviceImplLib.h>
#include <Library/KeypadDeviceHelperLib.h>
#include <Protocol/KeypadDevice.h>
#include <Protocol/FdtClient.h>

typedef enum {
  KEY_DEVICE_TYPE_UNKNOWN,

  KEY_DEVICE_TYPE_TLMM,
  KEY_DEVICE_TYPE_PM8X41,
  KEY_DEVICE_TYPE_PM8X41_PON,
} KEY_DEVICE_TYPE;

typedef struct {
  KEY_CONTEXT      EfiKeyContext;
  BOOLEAN          IsValid;
  KEY_DEVICE_TYPE  DeviceType;

  // gpio
  UINT8            Gpio;
  BOOLEAN          ActiveLow;

  // pon
  UINT32           PonType;
} KEY_CONTEXT_PRIVATE;

STATIC KEY_CONTEXT_PRIVATE KeyContextPower;
STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeUp;
STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeDown;

STATIC KEY_CONTEXT_PRIVATE* KeyList[] = {
  &KeyContextPower,
  &KeyContextVolumeUp,
  &KeyContextVolumeDown,
};

STATIC
VOID
KeypadInitializeKeyContextPrivate (
  KEY_CONTEXT_PRIVATE  *Context
  )
{
  Context->IsValid = FALSE;
  Context->Gpio = 0;
  Context->DeviceType = KEY_DEVICE_TYPE_UNKNOWN;
  Context->ActiveLow = FALSE;
}

STATIC
KEY_CONTEXT_PRIVATE*
KeypadKeyCodeToKeyContext (
  UINT32 KeyCode
  )
{
  if (KeyCode == 114)
    return &KeyContextVolumeDown;
  else if (KeyCode == 115)
    return &KeyContextVolumeUp;
  else if (KeyCode == 116)
    return &KeyContextPower;
  else
    return NULL;
}

RETURN_STATUS
EFIAPI
KeypadDeviceImplConstructor (
  VOID
  )
{
  EFI_STATUS                     Status, FindNodeStatus;
  FDT_CLIENT_PROTOCOL            *FdtClient;
  INT32                          Node, SubNode;
  UINTN                          Index;
  UINT32                         PropertySize;

  // reset all keys
  for (Index=0; Index<ARRAY_SIZE(KeyList); Index++) {
    KeypadInitializeKeyContextPrivate (KeyList[Index]);
  }

  // get fdt client
  Status = gBS->LocateProtocol (&gFdtClientProtocolGuid, NULL, (VOID **)&FdtClient);
  ASSERT_EFI_ERROR (Status);

  // iterate over all gpio keys
  for (FindNodeStatus = FdtClient->FindCompatibleNode (FdtClient, "gpio-keys", &Node);
       !EFI_ERROR (FindNodeStatus);
       FindNodeStatus = FdtClient->FindNextCompatibleNode (FdtClient, "gpio-keys", Node, &Node))
  {
    for (FindNodeStatus = FdtClient->FindSubNode(FdtClient, Node, &SubNode);
         !EFI_ERROR (FindNodeStatus);
         FindNodeStatus = FdtClient->FindNextSubNode(FdtClient, SubNode, &SubNode))
    {
      CONST UINT32         *KeyCodePtr;
      KEY_CONTEXT_PRIVATE  *Context;
      CONST UINT32         *GpioInfo;
      INT32                PHandleNode;
      CONST CHAR8          *StrProp;

      // get key code
      Status = FdtClient->GetNodeProperty (FdtClient, SubNode, "linux,code", (CONST VOID **)&KeyCodePtr, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize!=sizeof(*KeyCodePtr)) {
        continue;
      }

      // get key context
      Context = KeypadKeyCodeToKeyContext (SwapBytes32 (*KeyCodePtr));
      if (Context == NULL)
        continue;

      // get gpios node
      Status = FdtClient->GetNodeProperty (FdtClient, SubNode, "gpios", (CONST VOID **)&GpioInfo, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize<sizeof(*GpioInfo)*3) {
        continue;
      }

      // fetch gpio info
      Context->Gpio = SwapBytes32(GpioInfo[1]);
      Context->ActiveLow = SwapBytes32(GpioInfo[2]) & 0x1;

      // get gpio driver node
      Status = FdtClient->FindNodeByPHandle(FdtClient, SwapBytes32(GpioInfo[0]), &PHandleNode);
      if (EFI_ERROR (Status)) {
        continue;
      }

      // get gpio driver type
      Status = FdtClient->GetNodeProperty(FdtClient, PHandleNode, "compatible", (CONST VOID **)&StrProp, NULL);
      if (EFI_ERROR (Status)) {
        continue;
      }

      // set device type
      if (AsciiStrCmp (StrProp, "qcom,msm-gpio") == 0)
          Context->DeviceType = KEY_DEVICE_TYPE_TLMM;
      else if (AsciiStrCmp (StrProp, "qcom,qpnp-pin") == 0)
          Context->DeviceType = KEY_DEVICE_TYPE_PM8X41;
      else
          continue;

      // done :)
      Context->IsValid = TRUE;
    }
  }

  // iterate over all pon nodes
  for (FindNodeStatus = FdtClient->FindCompatibleNode (FdtClient, "qcom,qpnp-power-on", &Node);
       !EFI_ERROR (FindNodeStatus);
       FindNodeStatus = FdtClient->FindNextCompatibleNode (FdtClient, "qcom,qpnp-power-on", Node, &Node))
  {
    for (FindNodeStatus = FdtClient->FindSubNode(FdtClient, Node, &SubNode);
         !EFI_ERROR (FindNodeStatus);
         FindNodeStatus = FdtClient->FindNextSubNode(FdtClient, SubNode, &SubNode))
    {
      CONST UINT32         *KeyCodePtr;
      KEY_CONTEXT_PRIVATE  *Context;
      CONST UINT32         *PonType;

      // get key code
      Status = FdtClient->GetNodeProperty (FdtClient, SubNode, "linux,code", (CONST VOID **)&KeyCodePtr, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize!=sizeof(*KeyCodePtr)) {
        continue;
      }

      // get key context
      Context = KeypadKeyCodeToKeyContext (SwapBytes32 (*KeyCodePtr));
      if (Context == NULL)
        continue;

      // get pon type
      Status = FdtClient->GetNodeProperty (FdtClient, SubNode, "qcom,pon-type", (CONST VOID **)&PonType, &PropertySize);
      if (EFI_ERROR (Status) || PropertySize!=sizeof(*PonType)) {
        continue;
      }

      // fill context
      Context->DeviceType = KEY_DEVICE_TYPE_PM8X41_PON;
      Context->PonType = SwapBytes32 (*PonType);
      Context->IsValid = TRUE;
    }
  }

  return RETURN_SUCCESS;
}

EFI_STATUS EFIAPI KeypadDeviceImplReset (KEYPAD_DEVICE_PROTOCOL *This)
{
  LibKeyInitializeKeyContext(&KeyContextPower.EfiKeyContext);
  KeyContextPower.EfiKeyContext.KeyData.Key.UnicodeChar = CHAR_CARRIAGE_RETURN;

  LibKeyInitializeKeyContext(&KeyContextVolumeUp.EfiKeyContext);
  KeyContextVolumeUp.EfiKeyContext.KeyData.Key.ScanCode = SCAN_UP;

  LibKeyInitializeKeyContext(&KeyContextVolumeDown.EfiKeyContext);
  KeyContextVolumeDown.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  return EFI_SUCCESS;
}

EFI_STATUS KeypadDeviceImplGetKeys (KEYPAD_DEVICE_PROTOCOL *This, KEYPAD_RETURN_API *KeypadReturnApi, UINT64 Delta)
{
  UINT8    GpioStatus;
  BOOLEAN  IsPressed;
  INTN     RC;
  UINTN    Index;

  for (Index=0; Index<ARRAY_SIZE(KeyList); Index++) {
    KEY_CONTEXT_PRIVATE *Context = KeyList[Index];

    // check if this is a valid key
    if (Context->IsValid == FALSE)
      continue;

    // get status
    if (Context->DeviceType == KEY_DEVICE_TYPE_TLMM) {
      GpioStatus = gGpioTlmm->gpio_status(Context->Gpio);
      RC = 0;
    }
    else if (Context->DeviceType == KEY_DEVICE_TYPE_PM8X41) {
      RC = gPm8x41->pm8x41_gpio_get(Context->Gpio, &GpioStatus);
    }
    else if (Context->DeviceType == KEY_DEVICE_TYPE_PM8X41_PON) {
      if (Context->PonType == 0x1)
        GpioStatus = gPm8x41->pm8x41_resin_status ();
      else if (Context->PonType == 0x0)
        GpioStatus = gPm8x41->pm8x41_get_pwrkey_is_pressed ();
      else
        continue;

      RC = 0;
    }
    else {
      continue;
    }
    if (RC != 0)
      continue;

    // update key status
    IsPressed = (GpioStatus ? 1 : 0) ^ Context->ActiveLow;
    LibKeyUpdateKeyStatus(&Context->EfiKeyContext, KeypadReturnApi, IsPressed, Delta);
  }

  return EFI_SUCCESS;
}



