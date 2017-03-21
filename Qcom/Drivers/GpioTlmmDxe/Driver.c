#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/QcomPlatformGpioTlmmLib.h>
#include <Protocol/QcomGpioTlmm.h>

#include "Protocol.c"


EFI_STATUS
EFIAPI
GpioTlmmDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomGpioTlmmProtocolGuid,      &mGpioTlmm,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
