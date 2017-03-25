#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/QcomDxeTimerLib.h>
#include <Library/QcomPlatformDxeTimerLib.h>
#include <Chipset/dgt_timer.h>

VOID
EFIAPI
LibQcomDxeTimerEnable (
  UINT64 TimerTicks
  )
{
  LibQcomPlatformDxeTimerInitialize ();

  MmioWrite32(DGT_MATCH_VAL, (UINT32)TimerTicks);
  MmioWrite32(DGT_CLEAR, 0);
  MmioWrite32(DGT_ENABLE, DGT_ENABLE_EN | DGT_ENABLE_CLR_ON_MATCH_EN);
}

STATIC VOID
WaitForTimerOp (
  VOID
  )
{
  while (MmioRead32(SPSS_TIMER_STATUS) & SPSS_TIMER_STATUS_DGT_EN);
}


VOID
EFIAPI
LibQcomDxeTimerDisable (
  VOID
  )
{
  MmioWrite32(DGT_ENABLE, 0);
  WaitForTimerOp();
  MmioWrite32(DGT_CLEAR, 0);
  WaitForTimerOp();
}

UINTN
EFIAPI
LibQcomDxeTimerGetFreq (
  VOID
  )
{
  return LibQcomPlatformDxeTimerGetFreq();
}

VOID
EFIAPI
LibQcomDxeTimerFinishIrq (
  VOID
  )
{
}
