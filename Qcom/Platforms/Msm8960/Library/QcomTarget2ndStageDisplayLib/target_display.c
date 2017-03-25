#include <Library/LKEnvLib.h>
#include <Library/fbcon.h>
#include <Chipset/mdp4.h>
#include <Chipset/lcdc.h>
#include <Chipset/mipi_dsi.h>
#include <Platform/display.h>
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

    // set dst format
    unsigned char DST_FORMAT = 8;
    int data = 0x00100000;
    data |= ((PcdGet64(PcdMipiDsiRgbSwap) & 0x07) << 16);
    writel(data | DST_FORMAT, DSI_COMMAND_MODE_MDP_CTRL);

    // handle cmd trigger
    uint32_t trigger_ctrl = readl(DSI_TRIG_CTRL);
    int mdp_trigger = (trigger_ctrl >> 4) & 4;
    if (mdp_trigger == DSI_CMD_TRIGGER_SW) {
        config.update_start = mipi_update_sw_trigger;
    }

    // setup fbcon
    fbcon_setup(&config);
}
