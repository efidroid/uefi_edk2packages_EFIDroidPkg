#include <Library/LKEnvLib.h>
#include <Platform/display.h>
#include <Platform/clock.h>
#include <Platform/iomap.h>
#include <Library/QcomClockLib.h>
#include <Library/QcomPm8921Lib.h>

static struct pm8xxx_gpio_init pm8921_display_gpios_apq[] = {
	/* Display GPIOs */
	/* Bl: ON, PWM mode */
	PM8921_GPIO_OUTPUT_FUNC(PM_GPIO(26), 1, PM_GPIO_FUNC_2),
	/* LCD1_PWR_EN_N */
	PM8921_GPIO_OUTPUT_BUFCONF(PM_GPIO(36), 0, LOW, OPEN_DRAIN),
	/* DISP_RESET_N */
	PM8921_GPIO_OUTPUT_BUFCONF(PM_GPIO(25), 1, LOW, CMOS),
};

void apq8064_display_gpio_init(void)
{
	int i = 0;
	int num = 0;

	num = ARRAY_SIZE(pm8921_display_gpios_apq);

	for (i = 0; i < num; i++) {
		gPM8921->pm8921_gpio_config(pm8921_display_gpios_apq[i].gpio,
			&(pm8921_display_gpios_apq[i].config));
	}
}

/* Turn on MDP related clocks and pll's for MDP */
void mdp_clock_init(void)
{
	/* Set MDP clock to 200MHz */
	gClock->clk_get_set_enable("mdp_clk", 200000000, 1);

	/* Seems to lose pixels without this from status 0x051E0048 */
	gClock->clk_get_set_enable("lut_mdp", 0, 1);
}

/* Set rate and enable the clock */
static void clock_config(uint32_t ns, uint32_t md, uint32_t ns_addr, uint32_t md_addr)
{
	unsigned int val = 0;

	/* Activate the reset for the M/N Counter */
	val = 1 << 7;
	writel(val, ns_addr);

	/* Write the MD value into the MD register */
	if (md_addr != 0x0)
		writel(md, md_addr);

	/* Write the ns value, and active reset for M/N Counter, again */
	val = 1 << 7;
	val |= ns;
	writel(val, ns_addr);

	/* De-activate the reset for M/N Counter */
	val = 1 << 7;
	val = ~val;
	val = val & readl(ns_addr);
	writel(val, ns_addr);

	/* Enable the Clock Root */
	val = 1 << 11;
	val = val | readl(ns_addr);
	writel(val, ns_addr);

	/* Enable the Clock Branch */
	val = 1 << 9;
	val = val | readl(ns_addr);
	writel(val, ns_addr);

	/* Enable the M/N Counter */
	val = 1 << 8;
	val = val | readl(ns_addr);
	writel(val, ns_addr);
}

/* Write the M,N,D values and enable the MMSS Clocks */
static void config_mmss_clk(uint32_t ns,
		     uint32_t md,
		     uint32_t cc,
		     uint32_t ns_addr, uint32_t md_addr, uint32_t cc_addr)
{
	unsigned int val = 0;

	clock_config(ns, md, ns_addr, md_addr);

	/* Enable MND counter */
	val = cc | (1 << 5);
	val = val | readl(cc_addr);
	writel(val, cc_addr);

	/* Enable the root of the clock tree */
	val = 1 << 2;
	val = val | readl(cc_addr);
	writel(val, cc_addr);

	/* Enable the Pixel Clock */
	val = 1 << 0;
	val = val | readl(cc_addr);
	writel(val, cc_addr);

	/* Force On */
	val = 1 << 31;
	val = val | readl(cc_addr);
	writel(val, cc_addr);
}

/* Initialize all clocks needed by Display */
void mmss_clock_init(void)
{
	/* Configure Pixel clock */
	config_mmss_clk(PIXEL_NS_VAL, PIXEL_MD_VAL, PIXEL_CC_VAL,
			DSI_PIXEL_NS_REG, DSI_PIXEL_MD_REG, DSI_PIXEL_CC_REG);

	/* Configure DSI clock */
	config_mmss_clk(DSI_NS_VAL, DSI_MD_VAL, DSI_CC_VAL, DSI_NS_REG,
			DSI_MD_REG, DSI_CC_REG);

	/* Configure Byte clock */
	config_mmss_clk(BYTE_NS_VAL, 0x0, BYTE_CC_VAL, DSI1_BYTE_NS_REG, 0x0,
			DSI1_BYTE_CC_REG);

	/* Configure ESC clock */
	config_mmss_clk(ESC_NS_VAL, 0x0, ESC_CC_VAL, DSI1_ESC_NS_REG, 0x0,
			DSI1_ESC_CC_REG);
}

void liquid_mmss_clock_init(void)
{
	/* Configure Pixel clock = 78.75 MHZ */
	config_mmss_clk(0x2003, 0x01FB, 0x0005,
			DSI_PIXEL_NS_REG, DSI_PIXEL_MD_REG, DSI_PIXEL_CC_REG);

	/* Configure DSI clock = 236.25 MHZ */
	config_mmss_clk(0x03, 0x03FB, 0x05,
			DSI_NS_REG, DSI_MD_REG, DSI_CC_REG);

	/* Configure Byte clock = 59.06 MHZ */
	config_mmss_clk(0x0B01, 0x0, 0x80ff0025,
			DSI1_BYTE_NS_REG, 0x0, DSI1_BYTE_CC_REG);

	/* Configure ESC clock = 13.5 MHZ */
	config_mmss_clk(0x1B00, 0x0, 0x005,
			DSI1_ESC_NS_REG, 0x0, DSI1_ESC_CC_REG);
}

void mmss_clock_disable(void)
{
	writel(0x0, DSI1_BYTE_CC_REG);
	writel(0x0, DSI_PIXEL_CC_REG);
	writel(0x0, DSI1_ESC_CC_REG);

	/* Disable root clock */
	writel(0x0, DSI_CC_REG);
}
