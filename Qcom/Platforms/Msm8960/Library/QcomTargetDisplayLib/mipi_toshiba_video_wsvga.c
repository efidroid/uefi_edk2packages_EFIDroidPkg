/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <Library/LKEnvLib.h>
#include <Chipset/msm_panel.h>
#include <Chipset/mipi_dsi.h>
#include <Platform/iomap.h>
#include <Chipset/mdp4.h>

/* Toshiba mdt61 panel cmds */
static const unsigned char toshiba_mdt61_mcap_start[4] = {
	0xB0, 0x04, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_num_out_pixelform[8] = {
	0x03, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xB3, 0x00, 0x87, 0xFF
};

static const unsigned char toshiba_mdt61_dsi_ctrl[8] = {
	0x03, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xB6, 0x30, 0x83, 0xFF
};

static const unsigned char toshiba_mdt61_panel_driving[12] = {
	0x07, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xC0, 0x01, 0x00, 0x85,
	0x00, 0x00, 0x00, 0xFF
};

static const unsigned char toshiba_mdt61_dispV_timing[12] = {
	0x05, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xC1, 0x00, 0x10, 0x00,
	0x01, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_dispCtrl[8] = {
	0x03, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xC3, 0x00, 0x19, 0xFF
};

static const unsigned char toshiba_mdt61_test_mode_c4[4] = {
	0xC4, 0x03, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_dispH_timing[20] = {
	0x0F, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xC5, 0x00, 0x01, 0x05,
	0x04, 0x5E, 0x00, 0x00,
	0x00, 0x00, 0x0B, 0x17,
	0x05, 0x00, 0x00, 0xFF
};

static const unsigned char toshiba_mdt61_test_mode_c6[4] = {
	0xC6, 0x00, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_gamma_setA[20] = {
	0x0D, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xC8, 0x0A, 0x15, 0x18,
	0x1B, 0x1C, 0x0D, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_gamma_setB[20] = {
	0x0D, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xC9, 0x0D, 0x1D, 0x1F,
	0x1F, 0x1F, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_gamma_setC[20] = {
	0x0D, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xCA, 0x1E, 0x1F, 0x1E,
	0x1D, 0x1D, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_powerSet_ChrgPmp[12] = {
	0x05, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xD0, 0x02, 0x00, 0xA3,
	0xB8, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_testMode_d1[12] = {
	0x06, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xD1, 0x10, 0x14, 0x53,
	0x64, 0x00, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_powerSet_SrcAmp[8] = {
	0x03, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xD2, 0xB3, 0x00, 0xFF
};

static const unsigned char toshiba_mdt61_powerInt_PS[8] = {
	0x03, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xD3, 0x33, 0x03, 0xFF
};

static const unsigned char toshiba_mdt61_vreg[4] = {
	0xD5, 0x00, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_test_mode_d6[4] = {
	0xD6, 0x01, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_timingCtrl_d7[16] = {
	0x09, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xD7, 0x09, 0x00, 0x84,
	0x81, 0x61, 0xBC, 0xB5,
	0x05, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_timingCtrl_d8[12] = {
	0x07, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xD8, 0x04, 0x25, 0x90,
	0x4C, 0x92, 0x00, 0xFF
};

static const unsigned char toshiba_mdt61_timingCtrl_d9[8] = {
	0x04, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xD9, 0x5B, 0x7F, 0x05
};

static const unsigned char toshiba_mdt61_white_balance[12] = {
	0x06, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xCB, 0x00, 0x00, 0x00,
	0x1C, 0x00, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_vcs_settings[4] = {
	0xDD, 0x53, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_vcom_dc_settings[4] = {
	0xDE, 0x43, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_testMode_e3[12] = {
	0x05, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xE3, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_testMode_e4[12] = {
	0x06, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xE4, 0x00, 0x00, 0x22,
	0xAA, 0x00, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_testMode_e5[4] = {
	0xE5, 0x00, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_testMode_fa[8] = {
	0x04, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xFA, 0x00, 0x00, 0x00
};

static const unsigned char toshiba_mdt61_testMode_fd[12] = {
	0x05, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xFD, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_testMode_fe[12] = {
	0x05, 0x00, DTYPE_GEN_LWRITE, 0xC0,
	0xFE, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF
};

static const unsigned char toshiba_mdt61_mcap_end[4] = {
	0xB0, 0x03, DTYPE_GEN_WRITE2, 0x80,
};

static const unsigned char toshiba_mdt61_set_add_mode[4] = {
	0x36, 0x00, DTYPE_DCS_WRITE1, 0x80,
};

static const unsigned char toshiba_mdt61_set_pixel_format[4] = {
	0x3A, 0x70, DTYPE_DCS_WRITE1, 0x80,
};

static const unsigned char dsi_display_exit_sleep[4] = {
	0x11, 0x00, 0x15, 0x80,
};

static const unsigned char dsi_display_display_on[4] = {
	0x29, 0x00, 0x15, 0x80,
};

static struct mipi_dsi_cmd toshiba_mdt61_video_mode_cmds[] = {
	{sizeof(toshiba_mdt61_mcap_start), (char *)toshiba_mdt61_mcap_start},
	{sizeof(toshiba_mdt61_num_out_pixelform),
	 (char *)toshiba_mdt61_num_out_pixelform},
	{sizeof(toshiba_mdt61_dsi_ctrl), (char *)toshiba_mdt61_dsi_ctrl},
	{sizeof(toshiba_mdt61_panel_driving),
	 (char *)toshiba_mdt61_panel_driving},
	{sizeof(toshiba_mdt61_dispV_timing),
	 (char *)toshiba_mdt61_dispV_timing},
	{sizeof(toshiba_mdt61_dispCtrl), (char *)toshiba_mdt61_dispCtrl},
	{sizeof(toshiba_mdt61_test_mode_c4),
	 (char *)toshiba_mdt61_test_mode_c4},
	{sizeof(toshiba_mdt61_dispH_timing),
	 (char *)toshiba_mdt61_dispH_timing},
	{sizeof(toshiba_mdt61_test_mode_c6),
	 (char *)toshiba_mdt61_test_mode_c6},
	{sizeof(toshiba_mdt61_gamma_setA), (char *)toshiba_mdt61_gamma_setA},
	{sizeof(toshiba_mdt61_gamma_setB), (char *)toshiba_mdt61_gamma_setB},
	{sizeof(toshiba_mdt61_gamma_setC), (char *)toshiba_mdt61_gamma_setC},
	{sizeof(toshiba_mdt61_powerSet_ChrgPmp),
	 (char *)toshiba_mdt61_powerSet_ChrgPmp},
	{sizeof(toshiba_mdt61_testMode_d1), (char *)toshiba_mdt61_testMode_d1},
	{sizeof(toshiba_mdt61_powerSet_SrcAmp),
	 (char *)toshiba_mdt61_powerSet_SrcAmp},
	{sizeof(toshiba_mdt61_powerInt_PS), (char *)toshiba_mdt61_powerInt_PS},
	{sizeof(toshiba_mdt61_vreg), (char *)toshiba_mdt61_vreg},
	{sizeof(toshiba_mdt61_test_mode_d6),
	 (char *)toshiba_mdt61_test_mode_d6},
	{sizeof(toshiba_mdt61_timingCtrl_d7),
	 (char *)toshiba_mdt61_timingCtrl_d7},
	{sizeof(toshiba_mdt61_timingCtrl_d8),
	 (char *)toshiba_mdt61_timingCtrl_d8},
	{sizeof(toshiba_mdt61_timingCtrl_d9),
	 (char *)toshiba_mdt61_timingCtrl_d9},
	{sizeof(toshiba_mdt61_white_balance),
	 (char *)toshiba_mdt61_white_balance},
	{sizeof(toshiba_mdt61_vcs_settings),
	 (char *)toshiba_mdt61_vcs_settings},
	{sizeof(toshiba_mdt61_vcom_dc_settings),
	 (char *)toshiba_mdt61_vcom_dc_settings},
	{sizeof(toshiba_mdt61_testMode_e3), (char *)toshiba_mdt61_testMode_e3},
	{sizeof(toshiba_mdt61_testMode_e4), (char *)toshiba_mdt61_testMode_e4},
	{sizeof(toshiba_mdt61_testMode_e5), (char *)toshiba_mdt61_testMode_e5},
	{sizeof(toshiba_mdt61_testMode_fa), (char *)toshiba_mdt61_testMode_fa},
	{sizeof(toshiba_mdt61_testMode_fd), (char *)toshiba_mdt61_testMode_fd},
	{sizeof(toshiba_mdt61_testMode_fe), (char *)toshiba_mdt61_testMode_fe},
	{sizeof(toshiba_mdt61_mcap_end), (char *)toshiba_mdt61_mcap_end},
	{sizeof(toshiba_mdt61_set_add_mode),
	 (char *)toshiba_mdt61_set_add_mode},
	{sizeof(toshiba_mdt61_set_pixel_format),
	 (char *)toshiba_mdt61_set_pixel_format},
	{sizeof(dsi_display_exit_sleep), (char *)dsi_display_exit_sleep},
	{sizeof(dsi_display_display_on), (char *)dsi_display_display_on},
};

int mipi_toshiba_video_wsvga_config(void *pdata)
{
	int ret = NO_ERROR;

	/* 3 Lanes -- Enables Data Lane0, 1, 2 */
	unsigned char lane_en = 7;
	unsigned long low_pwr_stop_mode = 0;

	/* Needed or else will have blank line at top of display */
	unsigned char eof_bllp_pwr = 0x8;

	unsigned char interleav = 0;
	struct lcdc_panel_info *lcdc = NULL;
	struct msm_panel_info *pinfo = (struct msm_panel_info *)pdata;

	if (pinfo == NULL)
		return ERR_INVALID_ARGS;

	lcdc =  &(pinfo->lcdc);
	if (lcdc == NULL)
		return ERR_INVALID_ARGS;

	ret = mipi_dsi_video_mode_config((pinfo->xres + lcdc->xres_pad),
			(pinfo->yres + lcdc->yres_pad),
			(pinfo->xres),
			(pinfo->yres),
			(lcdc->h_front_porch),
			(lcdc->h_back_porch + lcdc->h_pulse_width),
			(lcdc->v_front_porch),
			(lcdc->v_back_porch + lcdc->v_pulse_width),
			(lcdc->h_pulse_width),
			(lcdc->v_pulse_width),
			pinfo->mipi.dst_format,
			pinfo->mipi.traffic_mode,
			lane_en,
			low_pwr_stop_mode,
			eof_bllp_pwr,
			interleav);
	return ret;
}

int mipi_toshiba_video_wsvga_on(void)
{
	int ret = NO_ERROR;
	return ret;
}

int mipi_toshiba_video_wsvga_off(void)
{
	int ret = NO_ERROR;
	return ret;
}

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* 600*1024, RGB888, 3 Lane 55 fps video mode */
	/* regulator */
	{0x03, 0x0a, 0x04, 0x00, 0x20},
	/* timing */
	{0xab, 0x8a, 0x18, 0x00, 0x92, 0x97, 0x1b, 0x8c,
		0x0c, 0x03, 0x04, 0xa0},
	/* phy ctrl */
	{0x5f, 0x00, 0x00, 0x10},
	/* strength */
	{0xff, 0x00, 0x06, 0x00},
	/* pll control */
	{0x0, 0x7f, 0x31, 0xda, 0x00, 0x50, 0x48, 0x63,
		0x41, 0x0f, 0x01,
		0x00, 0x14, 0x03, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01 },
};

void mipi_toshiba_video_wsvga_init(struct msm_panel_info *pinfo)
{
	if (!pinfo)
		return;

	pinfo->xres = 600;
	pinfo->yres = 1024;
	/*
	 *
	 * Panel's Horizontal input timing requirement is to
	 * include dummy(pad) data of 200 clk in addition to
	 * width and porch/sync width values
	 */
	pinfo->lcdc.xres_pad = 200;
	pinfo->lcdc.yres_pad = 0;

	pinfo->type = MIPI_VIDEO_PANEL;
	pinfo->wait_cycle = 0;
	pinfo->bpp = 24;
	pinfo->lcdc.h_back_porch = 16;
	pinfo->lcdc.h_front_porch = 23;
	pinfo->lcdc.h_pulse_width = 8;
	pinfo->lcdc.v_back_porch = 2;
	pinfo->lcdc.v_front_porch = 7;
	pinfo->lcdc.v_pulse_width = 2;
	pinfo->lcdc.border_clr = 0;	/* blk */
	pinfo->lcdc.underflow_clr = 0xff;	/* blue */
	pinfo->lcdc.hsync_skew = 0;
	pinfo->clk_rate = 384000000;

	pinfo->mipi.mode = DSI_VIDEO_MODE;
	pinfo->mipi.pulse_mode_hsa_he = FALSE;
	pinfo->mipi.hfp_power_stop = FALSE;
	pinfo->mipi.hbp_power_stop = FALSE;
	pinfo->mipi.hsa_power_stop = FALSE;
	pinfo->mipi.eof_bllp_power_stop = FALSE;
	pinfo->mipi.bllp_power_stop = FALSE;
	pinfo->mipi.traffic_mode = DSI_NON_BURST_SYNCH_EVENT;
	pinfo->mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo->mipi.vc = 0;
	pinfo->mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo->mipi.data_lane0 = TRUE;
	pinfo->mipi.data_lane1 = TRUE;
	pinfo->mipi.data_lane2 = TRUE;
	pinfo->mipi.t_clk_post = 0x20;
	pinfo->mipi.t_clk_pre = 0x2d;
	pinfo->mipi.stream = 0; /* dma_p */
	pinfo->mipi.mdp_trigger = 0;
	pinfo->mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo->mipi.frame_rate = 55;
	pinfo->mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	pinfo->mipi.tx_eot_append = TRUE;

	pinfo->mipi.num_of_lanes = 3;
	pinfo->mipi.panel_cmds = toshiba_mdt61_video_mode_cmds;
	pinfo->mipi.num_of_panel_cmds =
				 ARRAY_SIZE(toshiba_mdt61_video_mode_cmds);

	pinfo->on = mipi_toshiba_video_wsvga_on;
	pinfo->off = mipi_toshiba_video_wsvga_off;
	pinfo->config = mipi_toshiba_video_wsvga_config;

	return;
}
