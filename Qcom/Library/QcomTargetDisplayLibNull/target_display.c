#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Library/fbcon.h>
#include <Library/MemoryAllocationLib.h>

static struct fbcon_config config = {0};

void target_display_init(void)
{
  config.width = 800;
  config.height = 600;
  config.stride = config.width;
  config.bpp = 24;
  config.format = FB_FORMAT_RGB888;

  config.base = AllocateAlignedPages (EFI_SIZE_TO_PAGES(config.width * config.height * (config.bpp/3)), EFI_PAGE_SIZE);
  if (config.base == NULL) {
    return;
  }

  // setup fbcon
  fbcon_setup(&config);
}
