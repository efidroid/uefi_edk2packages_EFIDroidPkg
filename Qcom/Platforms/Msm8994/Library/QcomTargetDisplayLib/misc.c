#include <Library/LKEnvLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/QcomBoardLib.h>
#include <Chipset/smem.h>
#include <Chipset/msm_panel.h>

static uint8_t splash_override;
/* Returns 1 if target supports continuous splash screen. */
int target_cont_splash_screen(void)
{
	uint8_t splash_screen = 0;
	if(!splash_override) {
		switch(gBoard->board_hardware_id())
		{
			case HW_PLATFORM_SURF:
			case HW_PLATFORM_MTP:
			case HW_PLATFORM_FLUID:
			case HW_PLATFORM_LIQUID:
				dprintf(SPEW, "Target_cont_splash=1\n");
				splash_screen = 1;
				break;
			default:
				dprintf(SPEW, "Target_cont_splash=0\n");
				splash_screen = 0;
		}
	}
	return splash_screen;
}

void target_force_cont_splash_disable(uint8_t override)
{
        splash_override = override;
}

void target_edp_panel_init(struct msm_panel_info *pinfo)
{
	return;
}

int target_edp_panel_clock(uint8_t enable, struct msm_panel_info *pinfo)
{
	return 0;
}

int target_edp_panel_enable(void)
{
	return 0;
}

int target_edp_panel_disable(void)
{
	return 0;
}

int target_edp_bl_ctrl(int enable)
{
	return 0;
}

uint8_t target_panel_auto_detect_enabled(void)
{
	return 0;
}

int target_display_post_on(void)
{
	return 0;
}

int target_display_pre_off(void)
{
	return 0;
}

int target_display_post_off(void)
{
	return 0;
}
