#ifndef __QCOM_PROTOCOL_GPIO_TLMM_H__
#define __QCOM_PROTOCOL_GPIO_TLMM_H__

#include <Chipset/gpio.h>

#define QCOM_GPIO_TLMM_PROTOCOL_GUID \
  { 0x8054947b, 0x3223, 0x407a, { 0xa1, 0xcc, 0x31, 0x22, 0x2f, 0x80, 0x66, 0x40 } }

typedef struct _QCOM_GPIO_TLMM_PROTOCOL   QCOM_GPIO_TLMM_PROTOCOL;

typedef void (EFIAPI *gpio_tlmm_config_t)(uint32_t gpio, uint8_t func, uint8_t dir, uint8_t pull, uint8_t drvstr, uint32_t enable);
typedef void (EFIAPI *gpio_set_t)(uint32_t gpio, uint32_t dir);

struct _QCOM_GPIO_TLMM_PROTOCOL {
  gpio_tlmm_config_t         gpio_tlmm_config;
  gpio_set_t                 gpio_set;
};

extern EFI_GUID gQcomGpioTlmmProtocolGuid;

#endif
