#ifndef __LIBRARY_DXEINIT_H__
#define __LIBRARY_DXEINIT_H__

EFI_STATUS
EFIAPI
LibChipVendorDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
EFIAPI
LibPlatformDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

#endif
