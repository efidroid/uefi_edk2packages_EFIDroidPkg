#include <Library/LKEnvLib.h>
#include <Library/fbcon.h>
#include <Platform/iomap.h>
#include <Library/QcomTargetDisplayLib.h>

static struct fbcon_config config = {0};

RETURN_STATUS LibQcomTargetDisplayCallSecConstructors(VOID)
{
  return RETURN_SUCCESS;
}

void target_display_init(void)
{
  // dump format
  config.base = (void *) readl(MDP_DMA_P_BUF_ADDR);
  config.width = readl(MDP_DMA_P_BUF_Y_STRIDE)/3;
  config.height = readl(MDP_DMA_P_SIZE)>>16;
  config.stride = config.width;
  config.bpp = 24;
  config.format = FB_FORMAT_RGB888;

  // setup fbcon
  fbcon_setup(&config);
}
