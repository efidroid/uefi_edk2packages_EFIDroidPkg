#include <PiDxe.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/QcomQpnpWledLib.h>
#include <Library/LKEnvLib.h>

EFI_STATUS
EFIAPI
QpnpWledDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  QpnpWledImplLibInitialize();

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomQpnpWledProtocolGuid,      gQpnpWled,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
