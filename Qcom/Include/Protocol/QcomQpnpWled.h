#ifndef __QCOM_PROTOCOL_QPNP_WLED_H__
#define __QCOM_PROTOCOL_QPNP_WLED_H__

#define QCOM_QPNP_WLED_PROTOCOL_GUID \
  { 0xd6b4369d, 0x19f3, 0x48c2, { 0xa4, 0xd4, 0x6a, 0x8a, 0xfa, 0x45, 0xf4, 0x2b } }

typedef struct _QCOM_QPNP_WLED_PROTOCOL   QCOM_QPNP_WLED_PROTOCOL;

struct qpnp_wled_config_data {
  BOOLEAN display_type;
  CHAR8   pwr_up_delay;
  CHAR8   pwr_down_delay;
  CHAR8   ibb_discharge_en;
  UINT32  lab_min_volt;
  UINT32  lab_max_volt;
  UINT32  ibb_min_volt;
  UINT32  ibb_max_volt;
  UINT32  ibb_init_volt;
  UINT32  lab_init_volt;
};

typedef INTN (EFIAPI *qpnp_wled_init_t)(struct qpnp_wled_config_data *config);
typedef INTN (EFIAPI *qpnp_ibb_enable_t)(BOOLEAN state);
typedef VOID (EFIAPI *qpnp_wled_enable_backlight_t)(INTN enable);

struct _QCOM_QPNP_WLED_PROTOCOL {
  qpnp_wled_init_t              qpnp_wled_init;
  qpnp_ibb_enable_t             qpnp_ibb_enable;
  qpnp_wled_enable_backlight_t  qpnp_wled_enable_backlight;
};

extern EFI_GUID gQcomWpnpWledProtocolGuid;

#endif
