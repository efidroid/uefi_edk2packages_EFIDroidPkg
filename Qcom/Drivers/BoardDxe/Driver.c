#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Protocol/QcomBoard.h>
#include <Library/UefiBootServicesTableLib.h>

#include "board_p.h"
#include "Protocol.c"

QCOM_SMEM_PROTOCOL *gSMEM = NULL;

EFI_STATUS
EFIAPI
BoardDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gQcomSmemProtocolGuid, NULL, (VOID **)&gSMEM);
  ASSERT_EFI_ERROR (Status);

  board_init();

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomBoardProtocolGuid,      &mBoard,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
