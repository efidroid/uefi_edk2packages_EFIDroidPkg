#include <Library/LKEnvLib.h>
#include <Platform/clock.h>
#include <Platform/iomap.h>
#include <Library/QcomClockLib.h>

void mdp_gdsc_ctrl(uint8_t enable)
{
	uint32_t reg = 0;
	reg = readl(MDP_GDSCR);
	if (enable) {
		if (!(reg & GDSC_POWER_ON_BIT)) {
			reg &=  ~(BIT(0) | GDSC_EN_FEW_WAIT_MASK);
			reg |= GDSC_EN_FEW_WAIT_256_MASK;
			writel(reg, MDP_GDSCR);
			while(!(readl(MDP_GDSCR) & (GDSC_POWER_ON_BIT)));
		} else {
			dprintf(INFO, "MDP GDSC already enabled\n");
		}
	} else {
		reg |= BIT(0);
		writel(reg, MDP_GDSCR);
		while(readl(MDP_GDSCR) & (GDSC_POWER_ON_BIT));
	}
}

/* Configure MDP clock */
void mdp_clock_enable(void)
{
	int ret;

	/* Set MDP clock to 240MHz */
	ret = gClock->clk_get_set_enable("mdp_ahb_clk", 0, 1);
	if(ret)
	{
		dprintf(CRITICAL, "failed to set mdp_ahb_clk ret = %d\n", ret);
		ASSERT(0);
	}

	ret = gClock->clk_get_set_enable("mdss_mdp_clk_src", 300000000, 1);
	if(ret)
	{
		dprintf(CRITICAL, "failed to set mdp_clk_src ret = %d\n", ret);
		ASSERT(0);
	}

	ret = gClock->clk_get_set_enable("mdss_vsync_clk", 0, 1);
	if(ret)
	{
		dprintf(CRITICAL, "failed to set mdss vsync clk ret = %d\n", ret);
		ASSERT(0);
	}

	ret = gClock->clk_get_set_enable("mdss_mdp_clk", 0, 1);
	if(ret)
	{
		dprintf(CRITICAL, "failed to set mdp_clk ret = %d\n", ret);
		ASSERT(0);
	}
}

void mdp_clock_disable(void)
{
	gClock->clk_disable(gClock->clk_get("mdss_vsync_clk"));
	gClock->clk_disable(gClock->clk_get("mdss_mdp_clk"));
	gClock->clk_disable(gClock->clk_get("mdss_mdp_clk_src"));
	gClock->clk_disable(gClock->clk_get("mdp_ahb_clk"));

}

void mmss_bus_clock_enable(void)
{
	int ret;
	/* Configure MMSSNOC AXI clock */
	ret = gClock->clk_get_set_enable("mmss_mmssnoc_axi_clk", 300000000, 1);
	if(ret)
	{
		dprintf(CRITICAL, "failed to set mmssnoc_axi_clk ret = %d\n", ret);
		ASSERT(0);
	}

	/* Configure S0 AXI clock */
	ret = gClock->clk_get_set_enable("mmss_s0_axi_clk", 300000000, 1);
	if(ret)
	{
		dprintf(CRITICAL, "failed to set mmss_s0_axi_clk ret = %d\n", ret);
		ASSERT(0);
	}

	/* Configure AXI clock */
	ret = gClock->clk_get_set_enable("mdss_axi_clk", 300000000, 1);
	if(ret)
	{
		dprintf(CRITICAL, "failed to set mdss_axi_clk ret = %d\n", ret);
		ASSERT(0);
	}
}

void mmss_bus_clock_disable(void)
{
	/* Disable MDSS AXI clock */
	gClock->clk_disable(gClock->clk_get("mdss_axi_clk"));

	/* Disable MMSSNOC S0AXI clock */
	gClock->clk_disable(gClock->clk_get("mmss_s0_axi_clk"));

	/* Disable MMSSNOC AXI clock */
	gClock->clk_disable(gClock->clk_get("mmss_mmssnoc_axi_clk"));
}

void mmss_dsi_clock_enable(uint32_t dsi_pixel0_cfg_rcgr, uint32_t flags,
			uint8_t pclk0_m, uint8_t pclk0_n, uint8_t pclk0_d)
{
	int ret;

	if (flags & MMSS_DSI_CLKS_FLAG_DSI0) {
		/* Enable DSI0 branch clocks */
		writel(0x100, DSI_BYTE0_CFG_RCGR);
		writel(0x1, DSI_BYTE0_CMD_RCGR);
		writel(0x1, DSI_BYTE0_CBCR);

		writel(dsi_pixel0_cfg_rcgr, DSI_PIXEL0_CFG_RCGR);
		writel(0x1, DSI_PIXEL0_CMD_RCGR);
		writel(0x1, DSI_PIXEL0_CBCR);

		writel(pclk0_m, DSI_PIXEL0_M);
		writel(pclk0_n, DSI_PIXEL0_N);
		writel(pclk0_d, DSI_PIXEL0_D);

		ret = gClock->clk_get_set_enable("mdss_esc0_clk", 0, 1);
		if(ret)
		{
			dprintf(CRITICAL, "failed to set esc0_clk ret = %d\n", ret);
			ASSERT(0);
		}
	}

	if (flags & MMSS_DSI_CLKS_FLAG_DSI1) {
		/* Enable DSI1 branch clocks */
		writel(0x100, DSI_BYTE1_CFG_RCGR);
		writel(0x1, DSI_BYTE1_CMD_RCGR);
		writel(0x1, DSI_BYTE1_CBCR);

		writel(dsi_pixel0_cfg_rcgr, DSI_PIXEL1_CFG_RCGR);
		writel(0x1, DSI_PIXEL1_CMD_RCGR);
		writel(0x1, DSI_PIXEL1_CBCR);

		writel(pclk0_m, DSI_PIXEL1_M);
		writel(pclk0_n, DSI_PIXEL1_N);
		writel(pclk0_d, DSI_PIXEL1_D);

		ret = gClock->clk_get_set_enable("mdss_esc1_clk", 0, 1);
		if(ret)
		{
			dprintf(CRITICAL, "failed to set esc1_clk ret = %d\n", ret);
			ASSERT(0);
		}
	}
}

void mmss_dsi_clock_disable(uint32_t flags)
{
	if (flags & MMSS_DSI_CLKS_FLAG_DSI0) {
		gClock->clk_disable(gClock->clk_get("mdss_esc0_clk"));
		writel(0x0, DSI_BYTE0_CBCR);
		writel(0x0, DSI_PIXEL0_CBCR);
	}

	if (flags & MMSS_DSI_CLKS_FLAG_DSI1) {
		gClock->clk_disable(gClock->clk_get("mdss_esc1_clk"));
		writel(0x0, DSI_BYTE1_CBCR);
		writel(0x0, DSI_PIXEL1_CBCR);
	}
}

void hdmi_core_ahb_clk_enable(void)
{
	int ret;

	/* Configure hdmi ahb clock */
	ret = gClock->clk_get_set_enable("hdmi_ahb_clk", 0, 1);
	if(ret) {
		dprintf(CRITICAL, "failed to set hdmi_ahb_clk ret = %d\n", ret);
		ASSERT(0);
	}

	/* Configure hdmi core clock */
	ret = gClock->clk_get_set_enable("hdmi_core_clk", 19200000, 1);
	if(ret) {
		dprintf(CRITICAL, "failed to set hdmi_core_clk ret = %d\n", ret);
		ASSERT(0);
	}
}

void hdmi_pixel_clk_enable(uint32_t rate)
{
	int ret;

	/* Configure hdmi pixel clock */
	ret = gClock->clk_get_set_enable("hdmi_extp_clk", rate, 1);
	if(ret) {
		dprintf(CRITICAL, "failed to set hdmi_extp_clk ret = %d\n", ret);
		ASSERT(0);
	}
}

void hdmi_pixel_clk_disable(void)
{
	gClock->clk_disable(gClock->clk_get("hdmi_extp_clk"));
}

void hdmi_core_ahb_clk_disable(void)
{
	gClock->clk_disable(gClock->clk_get("hdmi_core_clk"));
	gClock->clk_disable(gClock->clk_get("hdmi_ahb_clk"));
}
