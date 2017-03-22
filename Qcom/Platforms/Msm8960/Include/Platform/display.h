#ifndef __PLATFORM_MSM8960_DISPLAY_H
#define __PLATFORM_MSM8960_DISPLAY_H

#include <Chipset/msm_panel.h>

void apq8064_display_gpio_init(void);
void mdp_clock_init(void);
void mmss_clock_init(void);
void mmss_clock_disable(void);
void liquid_mmss_clock_init(void);
void hdmi_power_init(void);
int lvds_on(struct msm_fb_panel_data *pdata);

#endif
