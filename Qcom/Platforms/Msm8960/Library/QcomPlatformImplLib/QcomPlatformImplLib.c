#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/QcomPlatformImplLib.h>

#include <Platform/iomap.h>
#include <Platform/irqs.h>

#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

#define SPSS_TIMER_STATUS_DGT_EN    (1 << 0)

VOID
EFIAPI
LibQcomTimerEnable (
  UINT64 TimerTicks
  )
{
  LibQcomPlatformTimerInitialize ();

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
LibQcomTimerDisable (
  VOID
  )
{
  MmioWrite32(DGT_ENABLE, 0);
  WaitForTimerOp();
  MmioWrite32(DGT_CLEAR, 0);
  WaitForTimerOp();
}

EFI_STATUS
EFIAPI
LibQcomPlatformTimerInitialize (
  VOID
  )
{
  //
  // DGT uses LPXO source which is 27MHz.
  // Set clock divider to 4.
  //
  MmioWrite32(DGT_CLK_CTL, 3);

  return EFI_SUCCESS;
}

UINTN
EFIAPI
LibQcomPlatformTimerGetFreq (
  VOID
  )
{
  return 6750000;
}

HARDWARE_INTERRUPT_SOURCE
EFIAPI
LibQcomPlatformTimerGetIntrNum (
  VOID
  )
{
  return INT_DEBUG_TIMER_EXP;
}








