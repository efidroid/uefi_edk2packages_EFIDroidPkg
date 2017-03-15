#include <PiDxe.h>

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Platform/iomap.h>

EFI_STATUS
EFIAPI
LibPlatformDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_PHYSICAL_ADDRESS            Memory;

  Status = PcdSet64S (PcdGicInterruptInterfaceBase, (UINT64) MSM_GIC_CPU_BASE);
  ASSERT_RETURN_ERROR (Status);

  Status = PcdSet64S (PcdGicDistributorBase, (UINT64) MSM_GIC_DIST_BASE);
  ASSERT_RETURN_ERROR (Status);

#ifdef MSM_GIC_REDIST_BASE
  Status = PcdSet64S (PcdGicRedistributorsBase, (UINT64) MSM_GIC_REDIST_BASE);
  ASSERT_RETURN_ERROR (Status);
#endif


  Memory = 0x80000000;
  Status = gBS->AllocatePages (
                  AllocateAddress,
                  EfiReservedMemoryType,
                  0x200000/EFI_PAGE_SIZE,
                  &Memory
                  );
  ASSERT_RETURN_ERROR (Status);

  return EFI_SUCCESS;
}
