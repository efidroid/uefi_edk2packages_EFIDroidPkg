#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomTargetMmcLib.h>
#include <Chipset/mmc.h>
#include <Platform/iomap.h>

static unsigned mmc_sdc_base[] =
    { MSM_SDC1_BASE, MSM_SDC2_BASE, MSM_SDC3_BASE, MSM_SDC4_BASE };

VOID LibQcomTargetMmcInit(INIT_SLOT_CB InitSlot)
{
  unsigned base_addr;
  unsigned char slot;

  /* Trying Slot 1 first */
  slot = 1;
  base_addr = mmc_sdc_base[slot - 1];
  if (InitSlot(slot, base_addr) == NULL) {
    /* Trying Slot 3 next */
    slot = 3;
    base_addr = mmc_sdc_base[slot - 1];
    if (InitSlot(slot, base_addr) == NULL) {
      DEBUG((EFI_D_ERROR, "mmc init failed!"));
    }
  }
}

/*
 * Function to set the capabilities for the host
 */
VOID LibQcomTargetMmcCaps(struct mmc_host *host)
{
  host->caps.ddr_mode = 1;
  host->caps.hs200_mode = 1;
  host->caps.bus_width = MMC_BOOT_BUS_WIDTH_8_BIT;
  host->caps.hs_clk_rate = MMC_CLK_96MHZ;
}
