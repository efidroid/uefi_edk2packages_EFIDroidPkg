#include <Library/LKEnvLib.h>
#include <Platform/iomap.h>
#include <Library/QcomGpioTlmmLib.h>

/* Remove the file after the gpio patch to move this to msm_shared gets merged. */
void gpio_tlmm_config(uint32_t gpio, uint8_t func,
		      uint8_t dir, uint8_t pull,
		      uint8_t drvstr, uint32_t enable)
{
	uint32_t val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= enable << 9;
	writel(val, (unsigned int *)GPIO_CONFIG_ADDR(gpio));
	return;
}

void gpio_set(uint32_t gpio, uint32_t dir)
{
	writel(dir, (unsigned int *)GPIO_IN_OUT_ADDR(gpio));
	return;
}

uint32_t gpio_status(uint32_t gpio)
{
	return readl(GPIO_IN_OUT_ADDR(gpio)) & GPIO_IN;
}
