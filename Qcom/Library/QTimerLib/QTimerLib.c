#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomDxeTimerLib.h>

#include "qtimer_p.h"
#include "qtimer_mmap_hw.h"

RETURN_STATUS
EFIAPI
TimerEarlyInit (
  VOID
  )
{
  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
TimerConstructor (
  VOID
  )
{
  return RETURN_SUCCESS;
}

STATIC inline UINT64 qtimer_get_phy_timer_cnt(VOID)
{
  UINT32 phy_cnt_lo;
  UINT32 phy_cnt_hi_1;
  UINT32 phy_cnt_hi_2;

  do {
    phy_cnt_hi_1 = readl(QTMR_V1_CNTPCT_HI);
    phy_cnt_lo = readl(QTMR_V1_CNTPCT_LO);
    phy_cnt_hi_2 = readl(QTMR_V1_CNTPCT_HI);
  } while (phy_cnt_hi_1 != phy_cnt_hi_2);

  return ((UINT64)phy_cnt_hi_1 << 32) | phy_cnt_lo;
}

UINTN
EFIAPI
MicroSecondDelay (
  IN      UINTN                     MicroSeconds
  )
{
  volatile UINTN Count;
  UINTN InitCount;
  UINTN Timeout;
  UINTN Ticks;

  // calculate number of ticks we have to wait
  Ticks = ((UINT64) MicroSeconds * qtimer_get_frequency()) / 1000000;

  // get current counter value
  Count = qtimer_get_phy_timer_cnt();
  InitCount = Count;

  // Calculate timeout = cnt + ticks (mod 2^56)
  // to account for timer counter wrapping
  Timeout = (Count + Ticks) & (UINT64)(0xFFFFFFFFFFFFFFULL);

  // Wait out till the counter wrapping occurs
  // in cases where there is a wrapping.
  while (Timeout < Count && InitCount <= Count)
    Count = qtimer_get_phy_timer_cnt();

  // Wait till the number of ticks is reached
  while (Timeout > Count)
    Count = qtimer_get_phy_timer_cnt();

  return MicroSeconds;
}

UINTN
EFIAPI
NanoSecondDelay (
  IN      UINTN                     NanoSeconds
  )
{
  UINTN  MicroSeconds;

  // Round up to 1us Tick Number
  MicroSeconds = NanoSeconds / 1000;
  MicroSeconds += ((NanoSeconds % 1000) == 0) ? 0 : 1;

  MicroSecondDelay (MicroSeconds);

  return NanoSeconds;
}

UINT64
EFIAPI
GetPerformanceCounter (
  VOID
  )
{
  ASSERT (FALSE);
  return 0;
}

UINT64
EFIAPI
GetPerformanceCounterProperties (
  OUT      UINT64                    *StartValue,  OPTIONAL
  OUT      UINT64                    *EndValue     OPTIONAL
  )
{
  ASSERT (FALSE);

  return (UINT64)(-1);
}

UINT64
EFIAPI
GetTimeInNanoSecond (
  IN      UINT64                     Ticks
  )
{
  ASSERT (FALSE);
  return 0;
}
