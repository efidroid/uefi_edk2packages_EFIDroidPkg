#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomClockSecLib.h>
#include <Library/QcomPlatformClockInitLib.h>

#include "clock_p.h"
#include "Protocol.c"

QCOM_CLOCK_PROTOCOL *gClock = NULL;

RETURN_STATUS
EFIAPI
ClockSecLibConstructor (
  VOID
  )
{
  RETURN_STATUS Status;
  struct clk_lookup *clist = NULL;
  unsigned num = 0;

  gClock = &mClock;

  Status = LibQcomPlatformClockInit (&clist, &num);
  ASSERT_EFI_ERROR (Status);
  clk_init(clist, num);

  return Status;
}
