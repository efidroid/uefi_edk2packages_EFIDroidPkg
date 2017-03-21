#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/QcomPlatformTimerLib.h>
#include <Chipset/dgt_timer.h>

RETURN_STATUS
EFIAPI
LibQcomPlatformDxeTimerInitialize (
  VOID
  )
{
  //
  // DGT uses LPXO source which is 27MHz.
  // Set clock divider to 4.
  //
  MmioWrite32(DGT_CLK_CTL, 3);

  return RETURN_SUCCESS;
}

UINTN
EFIAPI
LibQcomPlatformDxeTimerGetFreq (
  VOID
  )
{
  return 6750000;
}
