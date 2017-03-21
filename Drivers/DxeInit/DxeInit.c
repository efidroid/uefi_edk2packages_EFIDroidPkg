#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/DxeInit.h>

EFI_STATUS
EFIAPI
DxeInitInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;

  Status = LibChipVendorDxeInitialize (ImageHandle, SystemTable);
  ASSERT_RETURN_ERROR (Status);

  Status = LibPlatformDxeInitialize (ImageHandle, SystemTable);
  ASSERT_RETURN_ERROR (Status);

  return Status;
}

EFI_STATUS
EFIAPI
DxeInitPostGicInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;

  Status = LibChipVendorDxePostGicInitialize (ImageHandle, SystemTable);
  ASSERT_RETURN_ERROR (Status);

  Status = LibPlatformDxePostGicInitialize (ImageHandle, SystemTable);
  ASSERT_RETURN_ERROR (Status);

  return Status;
}
