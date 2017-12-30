#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomPlatformMmcClockOverrideLib.h>
#include <Chipset/mmc_sdhci.h>
#include <Library/PlatformIdLib.h>

UINT32 LibQcomPlatformMmcTranslateClockRate(UINT32 freq)
{
  if(freq == MMC_CLK_96MHZ)
  {
    return 100000000;
  }
  else if(freq == MMC_CLK_192MHZ)
  {
    if (platform_is_msm8992())
      return 172000000;
  }
  else if(freq == MMC_CLK_400MHZ)
  {
    if (platform_is_msm8992())
      return 344000000;
    else
      return 384000000;
  }

  return freq;
}
