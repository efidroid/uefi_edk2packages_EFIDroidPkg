#ifndef __QCOM_PROTOCOL_PM8921_H__
#define __QCOM_PROTOCOL_PM8921_H__

#include <Device/pm8921.h>
#include <Device/pm8921_pwm.h>

#define QCOM_PM8921_PROTOCOL_GUID \
  { 0x0ea8a93b, 0x3e74, 0x41b5, { 0x84, 0x84, 0x4f, 0x78, 0x5b, 0xa1, 0xba, 0x62 } }

typedef struct _QCOM_PM8921_PROTOCOL   QCOM_PM8921_PROTOCOL;

typedef int  (EFIAPI *pm8921_gpio_config_t)(int gpio, struct pm8921_gpio *param);
typedef void (EFIAPI *pm8921_boot_done_t)(void);
typedef int  (EFIAPI *pm8921_ldo_set_voltage_t)(uint32_t ldo_id, uint32_t voltage);
typedef int  (EFIAPI *pm8921_config_reset_pwr_off_t)(unsigned reset);
typedef int  (EFIAPI *pm8921_gpio_get_t)(uint8_t gpio, uint8_t *status);
typedef int  (EFIAPI *pm8921_pwrkey_status_t)(uint8_t *status);
typedef int  (EFIAPI *pm8921_config_led_current_t)(enum pm8921_leds led_num, uint8_t current, enum led_mode sink, int enable);
typedef int  (EFIAPI *pm8921_config_drv_keypad_t)(unsigned int drv_flash_sel, unsigned int flash_logic, unsigned int flash_ensel);
typedef int  (EFIAPI *pm8921_low_voltage_switch_enable_t)(uint8_t lvs_id);
typedef int  (EFIAPI *pm8921_mpp_set_digital_output_t)(uint8_t mpp_id);
typedef int  (EFIAPI *pm8921_rtc_alarm_disable_t)(void);
typedef int  (EFIAPI *pm89xx_bat_alarm_set_t)(bat_vol_t, bat_vol_t);
typedef int  (EFIAPI *pm89xx_bat_alarm_status_t)(uint8_t *, uint8_t *);
typedef int  (EFIAPI *pm89xx_vbus_status_t)(void);
typedef int  (EFIAPI *pm89xx_ldo_set_voltage_t)(const char * , uint32_t);

struct _QCOM_PM8921_PROTOCOL {
  pm8921_gpio_config_t               pm8921_gpio_config;
  pm8921_boot_done_t                 pm8921_boot_done;
  pm8921_ldo_set_voltage_t           pm8921_ldo_set_voltage;
  pm8921_config_reset_pwr_off_t      pm8921_config_reset_pwr_off;
  pm8921_gpio_get_t                  pm8921_gpio_get;
  pm8921_pwrkey_status_t             pm8921_pwrkey_status;
  pm8921_config_led_current_t        pm8921_config_led_current;
  pm8921_config_drv_keypad_t         pm8921_config_drv_keypad;
  pm8921_low_voltage_switch_enable_t pm8921_low_voltage_switch_enable;
  pm8921_mpp_set_digital_output_t    pm8921_mpp_set_digital_output;
  pm8921_rtc_alarm_disable_t         pm8921_rtc_alarm_disable;
  pm89xx_bat_alarm_set_t             pm89xx_bat_alarm_set;
  pm89xx_bat_alarm_status_t          pm89xx_bat_alarm_status;
  pm89xx_vbus_status_t               pm89xx_vbus_status;
  pm89xx_ldo_set_voltage_t           pm89xx_ldo_set_voltage;
};

extern EFI_GUID gQcomPm8921ProtocolGuid;

#endif
