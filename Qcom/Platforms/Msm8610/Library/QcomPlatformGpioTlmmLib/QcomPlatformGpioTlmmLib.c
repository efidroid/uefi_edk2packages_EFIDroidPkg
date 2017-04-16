#include <Library/LKEnvLib.h>
#include <Platform/iomap.h>
#include <Chipset/gpio.h>

void gpio_tlmm_config(uint32_t gpio, uint8_t func,
                      uint8_t dir, uint8_t pull,
                      uint8_t drvstr, uint32_t enable)
{
	uint32_t val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= enable << 9;
	writel(val, (uint32_t *)GPIO_CONFIG_ADDR(gpio));
	return;
}

void gpio_set(uint32_t gpio, uint32_t dir)
{
	writel(dir, (uint32_t *)GPIO_IN_OUT_ADDR(gpio));
	return;
}

uint32_t gpio_status(uint32_t gpio)
{
	return readl(GPIO_IN_OUT_ADDR(gpio)) & GPIO_IN;
}
