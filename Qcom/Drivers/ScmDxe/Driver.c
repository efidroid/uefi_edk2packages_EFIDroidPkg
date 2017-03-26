#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomScmLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
ScmDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  ScmImplLibInitialize();

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomScmProtocolGuid,      gScm,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
