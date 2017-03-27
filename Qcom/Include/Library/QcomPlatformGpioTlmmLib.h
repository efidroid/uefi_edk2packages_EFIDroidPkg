#ifndef __LIBRARY_QCOM_PLATFORM_GPIO_TLMM_LIB_H__
#define __LIBRARY_QCOM_PLATFORM_GPIO_TLMM_LIB_H__

void gpio_tlmm_config(uint32_t gpio, uint8_t func, uint8_t dir, uint8_t pull, uint8_t drvstr, uint32_t enable);
void gpio_set(uint32_t gpio, uint32_t dir);
uint32_t gpio_status(uint32_t gpio);

#endif
