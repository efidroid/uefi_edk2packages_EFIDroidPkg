#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomPm8921Lib.h>
#include <Library/UefiBootServicesTableLib.h>

QCOM_PM8921_PROTOCOL *gPM8921 = NULL;

RETURN_STATUS
EFIAPI
Pm8921LibConstructor (
  VOID
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gQcomPm8921ProtocolGuid, NULL, (VOID **)&gPM8921);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
