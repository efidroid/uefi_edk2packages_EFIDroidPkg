#include <PiDxe.h>

EFI_STATUS
EFIAPI
LibPlatformDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LibPlatformDxePostGicInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  return EFI_SUCCESS;
}
