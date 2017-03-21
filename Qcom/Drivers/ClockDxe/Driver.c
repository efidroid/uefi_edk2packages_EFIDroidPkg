#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Protocol/QcomClock.h>
#include <Library/QcomPlatformClockInitLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "clock_p.h"
#include "Protocol.c"

EFI_STATUS
EFIAPI
ClockDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;
  struct clk_lookup *clist = NULL;
  unsigned num = 0;

  Status = LibQcomPlatformClockInit (&clist, &num);
  ASSERT_EFI_ERROR (Status);
  clk_init(clist, num);

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomClockProtocolGuid,      &mClock,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
