#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomPm8921Lib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
Pm8921DxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  Pm8921ImplLibInitialize();

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomPm8921ProtocolGuid,      gPM8921,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
