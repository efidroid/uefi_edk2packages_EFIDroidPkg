#ifndef _PM8921_PRIVATE_H
#define _PM8921_PRIVATE_H

typedef int (*pm8921_read_func)(uint8_t *data, uint32_t length, uint32_t addr);
typedef int (*pm8921_write_func)(uint8_t *data, uint32_t length, uint32_t addr);

typedef struct
{
	uint32_t initialized;

	pm8921_read_func	read;
	pm8921_write_func	write;

} pm8921_dev_t;

void pm8921_init(pm8921_dev_t *);
int  pm8921_gpio_config(int gpio, struct pm8921_gpio *param);
void pm8921_boot_done(void);
int  pm8921_ldo_set_voltage(uint32_t ldo_id, uint32_t voltage);
int  pm8921_config_reset_pwr_off(unsigned reset);
int  pm8921_gpio_get(uint8_t gpio, uint8_t *status);
int  pm8921_pwrkey_status(uint8_t *status);
int pm8921_config_led_current(enum pm8921_leds led_num,
	uint8_t current,
	enum led_mode sink,
	int enable);
int pm8921_config_drv_keypad(unsigned int drv_flash_sel,
	unsigned int flash_logic,
	unsigned int flash_ensel);
int pm8921_low_voltage_switch_enable(uint8_t lvs_id);
int pm8921_mpp_set_digital_output(uint8_t mpp_id);
int pm8921_rtc_alarm_disable(void);
int pm89xx_bat_alarm_set(bat_vol_t, bat_vol_t);
int pm89xx_bat_alarm_status(uint8_t *, uint8_t *);
int pm89xx_vbus_status(void);
int pm89xx_ldo_set_voltage(const char * , uint32_t);

/* External PWM functions */
int pm8921_pwm_enable(uint8_t pwm_id, pm8921_dev_t *dev);
int pm8921_pwm_config(uint8_t pwm_id,
    uint32_t duty_us,
    uint32_t period_us,
    pm8921_dev_t *dev);

#endif // _PM8921_PRIVATE_H
