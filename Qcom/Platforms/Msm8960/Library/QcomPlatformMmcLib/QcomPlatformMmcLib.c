#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomClockLib.h>
#include <Library/QcomPlatformMmcLib.h>
#include <Chipset/mmc.h>
#include <Platform/iomap.h>

/* Intialize MMC clock */
VOID LibQcomPlatformMmcClockInit(UINT32 interface)
{
  /* Nothing to be done. */
}

/* Configure MMC clock */
VOID LibQcomPlatformMmcClockConfig(UINT32 interface, UINT32 freq)
{
  CHAR8 sdc_clk[64];
  UINTN rate;

  snprintf(sdc_clk, 64, "sdc%u_clk", interface);

  switch(freq)
  {
  case MMC_CLK_400KHZ:
    rate = 144000;
    break;
  case MMC_CLK_48MHZ:
  case MMC_CLK_50MHZ: /* Max supported is 48MHZ */
    rate = 48000000;
    break;
  case MMC_CLK_96MHZ:
    rate = 96000000;
    break;
  default:
    ASSERT(0);
  };

  gClock->clk_get_set_enable(sdc_clk, rate, 1);
}
