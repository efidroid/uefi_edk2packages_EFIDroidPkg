/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
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

#ifndef _PLATFORM_MSM_SHARED_MIPI_DSI_H_
#define _PLATFORM_MSM_SHARED_MIPI_DSI_H_

#include <Chipset/msm_panel.h>

#define PASS                        0
#define FAIL                        1

#define DSI_CLKOUT_TIMING_CTRL                REG_DSI(0x0C0)
#define DSI_SOFT_RESET                        REG_DSI(0x114)
#define DSI_CAL_CTRL                          REG_DSI(0x0F4)

#define DSIPHY_SW_RESET                       REG_DSI(0x128)
#define DSIPHY_PLL_RDY                        REG_DSI(0x280)
#define DSIPHY_REGULATOR_CAL_PWR_CFG          REG_DSI(0x518)

#define DSI_CLK_CTRL                          REG_DSI(0x118)
#define DSI_TRIG_CTRL                         REG_DSI(0x080)
#define DSI_CTRL                              REG_DSI(0x000)
#define DSI_COMMAND_MODE_DMA_CTRL             REG_DSI(0x038)
#define DSI_COMMAND_MODE_MDP_CTRL             REG_DSI(0x03C)
#define DSI_COMMAND_MODE_MDP_DCS_CMD_CTRL     REG_DSI(0x040)
#define DSI_DMA_CMD_OFFSET                    REG_DSI(0x044)
#define DSI_DMA_CMD_LENGTH                    REG_DSI(0x048)
#define DSI_COMMAND_MODE_MDP_STREAM0_CTRL     REG_DSI(0x054)
#define DSI_COMMAND_MODE_MDP_STREAM0_TOTAL    REG_DSI(0x058)
#define DSI_COMMAND_MODE_MDP_STREAM1_CTRL     REG_DSI(0x05C)
#define DSI_COMMAND_MODE_MDP_STREAM1_TOTAL    REG_DSI(0x060)
#define DSI_ERR_INT_MASK0                     REG_DSI(0x108)
#define DSI_INT_CTRL                          REG_DSI(0x10C)

#define DSI_VIDEO_MODE_ACTIVE_H               REG_DSI(0x020)
#define DSI_VIDEO_MODE_ACTIVE_V               REG_DSI(0x024)
#define DSI_VIDEO_MODE_TOTAL                  REG_DSI(0x028)
#define DSI_VIDEO_MODE_HSYNC                  REG_DSI(0x02C)
#define DSI_VIDEO_MODE_VSYNC                  REG_DSI(0x030)
#define DSI_VIDEO_MODE_VSYNC_VPOS             REG_DSI(0x034)

#define DSI_MISR_CMD_CTRL                     REG_DSI(0x09C)
#define DSI_MISR_VIDEO_CTRL                   REG_DSI(0x0A0)
#define DSI_EOT_PACKET_CTRL                   REG_DSI(0x0C8)
#define DSI_VIDEO_MODE_CTRL                   REG_DSI(0x00C)
#define DSI_CAL_STRENGTH_CTRL                 REG_DSI(0x100)
#define DSI_CMD_MODE_DMA_SW_TRIGGER           REG_DSI(0x08C)
#define DSI_CMD_MODE_MDP_SW_TRIGGER           REG_DSI(0x090)

#define DSI_LANE_CTRL                         REG_DSI(0x0A8)

#define MIPI_DSI_MRPS       0x04	/* Maximum Return Packet Size */
#define MIPI_DSI_REG_LEN    16	/* 4 x 4 bytes register */

#define DTYPE_GEN_WRITE2 0x23	/* 4th Byte is 0x80 */
#define DTYPE_GEN_LWRITE 0x29	/* 4th Byte is 0xc0 */
#define DTYPE_DCS_WRITE1 0x15	/* 4th Byte is 0x80 */

#define MIPI_VIDEO_MODE	        1
#define MIPI_CMD_MODE           2

struct mipi_dsi_phy_ctrl {
	uint32_t regulator[5];
	uint32_t timing[12];
	uint32_t ctrl[4];
	uint32_t strength[4];
	uint32_t pll[21];
};

struct mipi_dsi_cmd {
	int size;
	char *payload;
};

struct mipi_dsi_panel_config {
	char mode;
	char num_of_lanes;
	char lane_swap;
	struct mipi_dsi_phy_ctrl *dsi_phy_config;
	struct mipi_dsi_cmd *panel_cmds;
	int num_of_panel_cmds;
};

enum {		/* mipi dsi panel */
	DSI_VIDEO_MODE,
	DSI_CMD_MODE,
};
#define DSI_NON_BURST_SYNCH_PULSE	0
#define DSI_NON_BURST_SYNCH_EVENT	1
#define DSI_BURST_MODE			2

#define DSI_RGB_SWAP_RGB	0
#define DSI_RGB_SWAP_RBG	1
#define DSI_RGB_SWAP_BGR	2
#define DSI_RGB_SWAP_BRG	3
#define DSI_RGB_SWAP_GRB	4
#define DSI_RGB_SWAP_GBR	5

#define DSI_VIDEO_DST_FORMAT_RGB565		0
#define DSI_VIDEO_DST_FORMAT_RGB666		1
#define DSI_VIDEO_DST_FORMAT_RGB666_LOOSE	2
#define DSI_VIDEO_DST_FORMAT_RGB888		3

#define DSI_CMD_DST_FORMAT_RGB111	0
#define DSI_CMD_DST_FORMAT_RGB332	3
#define DSI_CMD_DST_FORMAT_RGB444	4
#define DSI_CMD_DST_FORMAT_RGB565	6
#define DSI_CMD_DST_FORMAT_RGB666	7
#define DSI_CMD_DST_FORMAT_RGB888	8

#define DSI_CMD_TRIGGER_NONE		0x0	/* mdp trigger */
#define DSI_CMD_TRIGGER_TE		0x02
#define DSI_CMD_TRIGGER_SW		0x04
#define DSI_CMD_TRIGGER_SW_SEOF		0x05	/* cmd dma only */
#define DSI_CMD_TRIGGER_SW_TE		0x06

int mipi_config(struct msm_fb_panel_data *panel);
int mipi_dsi_video_mode_config(unsigned short disp_width,
		unsigned short disp_height,
		unsigned short img_width,
		unsigned short img_height,
		unsigned short hsync_porch0_fp,
		unsigned short hsync_porch0_bp,
		unsigned short vsync_porch0_fp,
		unsigned short vsync_porch0_bp,
		unsigned short hsync_width,
		unsigned short vsync_width,
		unsigned short dst_format,
		unsigned short traffic_mode,
		unsigned char lane_en,
		unsigned low_pwr_stop_mode,
		unsigned char eof_bllp_pwr,
		unsigned char interleav);
int mipi_cmd_trigger(void);
int mipi_dsi_on(void);
int mipi_dsi_off(void);
int mipi_dsi_cmds_tx(struct mipi_dsi_cmd *cmds, int count);
int mipi_dsi_cmds_rx(char **rp, int len);
#endif
