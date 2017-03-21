#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/QcomSmem.h>

#include "smem_p.h"
#include "Protocol.c"


EFI_STATUS
EFIAPI
SmemDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomSmemProtocolGuid,      &mSMEM,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
