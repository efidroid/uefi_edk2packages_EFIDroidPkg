#ifndef __QCOM_PROTOCOL_SCM_H__
#define __QCOM_PROTOCOL_SCM_H__

#include <Chipset/scm.h>

#define QCOM_SCM_PROTOCOL_GUID \
  { 0x231fddbd, 0xe8ff, 0x4df3, { 0x91, 0xb8, 0xe2, 0x12, 0xd7, 0x01, 0x70, 0xd6 } }

typedef struct _QCOM_SCM_PROTOCOL   QCOM_SCM_PROTOCOL;

typedef int (EFIAPI *QCOM_SSBI_RW_FUNCTION)(unsigned char *buffer, unsigned short length, unsigned short slave_addr);

typedef int      (EFIAPI *decrypt_scm_t)(uint32_t ** img_ptr, uint32_t * img_len_ptr);
typedef int      (EFIAPI *decrypt_scm_v2_t)(uint32_t ** img_ptr, uint32_t * img_len_ptr);
typedef int      (EFIAPI *encrypt_scm_t)(uint32_t ** img_ptr, uint32_t * img_len_ptr);
typedef int      (EFIAPI *scm_svc_version_t)(uint32 * major, uint32 * minor);
typedef int      (EFIAPI *scm_svc_get_secure_state_t)(uint32_t *state_low, uint32_t *state_high);
typedef int      (EFIAPI *scm_protect_keystore_t)(uint32_t * img_ptr, uint32_t  img_len);
typedef int      (EFIAPI *scm_call_t)(uint32_t svc_id, uint32_t cmd_id, const void *cmd_buf, size_t cmd_len, void *resp_buf, size_t resp_len);
typedef int      (EFIAPI *scm_call_atomic_t)(uint32_t svc, uint32_t cmd, uint32_t arg1);
typedef int      (EFIAPI *scm_call_atomic2_t)(uint32_t svc, uint32_t cmd, uint32_t arg1, uint32_t arg2);
typedef uint32_t (EFIAPI *scm_call2_t)(scmcall_arg *arg, scmcall_ret *ret);
typedef int      (EFIAPI *scm_call2_atomic_t)(uint32_t svc, uint32_t cmd, uint32_t arg1, uint32_t arg2);
typedef int      (EFIAPI *scm_halt_pmic_arbiter_t)(void);
typedef void     (EFIAPI *scm_elexec_call_t)(paddr_t kernel_entry, paddr_t dtb_offset);
typedef int      (EFIAPI *scm_xpu_err_fatal_init_t)(void);
typedef int      (EFIAPI *scm_dload_mode_t)(int mode);
typedef int      (EFIAPI *scm_random_t)(uint32_t * rbuf, uint32_t  r_len);
typedef uint32_t (EFIAPI *scm_io_read_t)(addr_t address);
typedef uint32_t (EFIAPI *scm_io_write_t)(uint32_t address, uint32_t val);
typedef uint8_t  (EFIAPI *switch_ce_chn_cmd_t)(enum ap_ce_channel_type channel);
typedef int      (EFIAPI *mdtp_cipher_dip_cmd_t)(uint8_t *in_buf, uint32_t in_buf_size, uint8_t *out_buf, uint32_t out_buf_size, uint32_t direction);
typedef void     (EFIAPI *set_tamper_fuse_cmd_t)(void);
typedef int      (EFIAPI *qfprom_read_row_cmd_t)(uint32_t row_address, uint32_t addr_type, uint32_t *row_data, uint32_t *qfprom_api_status);
typedef int      (EFIAPI *restore_secure_cfg_t)(uint32_t id);
typedef bool     (EFIAPI *is_secure_boot_enable_t)(void);
typedef bool     (EFIAPI *is_scm_armv8_support_t)(void);


struct _QCOM_SCM_PROTOCOL {
  decrypt_scm_t               decrypt_scm;
  decrypt_scm_v2_t            decrypt_scm_v2;
  encrypt_scm_t               encrypt_scm;
  scm_svc_version_t           scm_svc_version;
  scm_svc_get_secure_state_t  scm_svc_get_secure_state;
  scm_protect_keystore_t      scm_protect_keystore;
  scm_call_t                  scm_call;
  scm_call_atomic_t           scm_call_atomic;
  scm_call_atomic2_t          scm_call_atomic2;
  scm_call2_t                 scm_call2;
  scm_call2_atomic_t          scm_call2_atomic;
  scm_halt_pmic_arbiter_t     scm_halt_pmic_arbiter;
  scm_elexec_call_t           scm_elexec_call;
  scm_xpu_err_fatal_init_t    scm_xpu_err_fatal_init;
  scm_dload_mode_t            scm_dload_mode;
  scm_random_t                scm_random;
  scm_io_read_t               scm_io_read;
  scm_io_write_t              scm_io_write;
  switch_ce_chn_cmd_t         switch_ce_chn_cmd;
  mdtp_cipher_dip_cmd_t       mdtp_cipher_dip_cmd;
  set_tamper_fuse_cmd_t       set_tamper_fuse_cmd;
  qfprom_read_row_cmd_t       qfprom_read_row_cmd;
  restore_secure_cfg_t        restore_secure_cfg;
  is_secure_boot_enable_t     is_secure_boot_enable;
  is_scm_armv8_support_t      is_scm_armv8_support;
};

extern EFI_GUID gQcomScmProtocolGuid;

#endif
