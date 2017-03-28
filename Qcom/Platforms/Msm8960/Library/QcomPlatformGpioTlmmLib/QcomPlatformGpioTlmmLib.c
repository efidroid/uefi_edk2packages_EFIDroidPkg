#include <Library/LKEnvLib.h>
#include <Platform/iomap.h>
#include <Library/QcomGpioTlmmLib.h>

void gpio_tlmm_config(uint32_t gpio, uint8_t func,
		      uint8_t dir, uint8_t pull,
		      uint8_t drvstr, uint32_t enable)
{
	// since we're using unified header files, we need to swap this one
	enable = !enable;

	unsigned int val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= enable << 9;
	unsigned int *addr = (unsigned int *)GPIO_CONFIG_ADDR(gpio);
	writel(val, addr);
	return;
}

void gpio_set(uint32_t gpio, uint32_t dir)
{
	unsigned int *addr = (unsigned int *)GPIO_IN_OUT_ADDR(gpio);
	writel(dir, addr);
	return;
}

uint32_t gpio_status(uint32_t gpio)
{
	return readl(GPIO_IN_OUT_ADDR(gpio)) & GPIO_IN;
}
