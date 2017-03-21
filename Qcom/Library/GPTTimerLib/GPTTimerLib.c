#include <Base.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#define GPT_REG(off)        (((UINTN)PcdGet64(PcdMsmGptBase)) + (off))

#define GPT_MATCH_VAL        GPT_REG((UINTN)PcdGet64(PcdMsmGptMatchValOffset))
#define GPT_COUNT_VAL        GPT_REG((UINTN)PcdGet64(PcdMsmGptCountValOffset))
#define GPT_ENABLE           GPT_REG((UINTN)PcdGet64(PcdMsmGptEnableOffset))
#define GPT_CLEAR            GPT_REG((UINTN)PcdGet64(PcdMsmGptClearOffset))

#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1

RETURN_STATUS
EFIAPI
TimerEarlyInit (
  VOID
  )
{
  // disable counter
  MmioWrite32(GPT_CLEAR, 0);
  MmioWrite32(GPT_ENABLE, 0);
  while (MmioRead32(GPT_COUNT_VAL) != 0);

  // enable counter
  MmioWrite32(GPT_ENABLE, GPT_ENABLE_EN);

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
  Ticks = (MicroSeconds * 33 + 1000 - 33) / 1000;

  // get current counter value
  Count = MmioRead32(GPT_COUNT_VAL);
  InitCount = Count;

  // Calculate timeout = cnt + ticks (mod 2^56)
  // to account for timer counter wrapping
  Timeout = (Count + Ticks) & (UINTN)(0xFFFFFFFF);

  // Wait out till the counter wrapping occurs
  // in cases where there is a wrapping.
  while (Timeout < Count && InitCount <= Count)
    Count = MmioRead32(GPT_COUNT_VAL);

  // Wait till the number of ticks is reached
  while (Timeout > Count)
    Count = MmioRead32(GPT_COUNT_VAL);

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
