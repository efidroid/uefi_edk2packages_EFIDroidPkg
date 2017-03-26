#ifndef _SCM_PRIVATE_H
#define _SCM_PRIVATE_H

#define SHA256_DIGEST_LENGTH	32

int decrypt_scm(uint32_t ** img_ptr, uint32_t * img_len_ptr);
int decrypt_scm_v2(uint32_t ** img_ptr, uint32_t * img_len_ptr);
int encrypt_scm(uint32_t ** img_ptr, uint32_t * img_len_ptr);
int scm_svc_version(uint32 * major, uint32 * minor);

/**
 * Check security status on the device. Returns the security check result.
 * Bit value 0 means the check passing, and bit value 1 means the check failing.
 *
 * @state_low[out] : lower 32 bits of the state:
 *                   Bit 0: secboot enabling check failed
 *                   Bit 1: Sec HW key is not programmed
 *                   Bit 2: debug disable check failed
 *                   Bit 3: Anti-rollback check failed
 *                   Bit 4: fuse config check failed
 *                   Bit 5: rpmb fuse check failed
 * @state_high[out] : highr 32 bits of the state.
 *
 * Returns 0 on success, negative on failure.
 */
int scm_svc_get_secure_state(uint32_t *state_low, uint32_t *state_high);

int scm_protect_keystore(uint32_t * img_ptr, uint32_t  img_len);
int
scm_call(uint32_t svc_id, uint32_t cmd_id, const void *cmd_buf,
        size_t cmd_len, void *resp_buf, size_t resp_len);

uint8_t switch_ce_chn_cmd(enum ap_ce_channel_type channel);

/**
 * Encrypt or Decrypt a Data Integrity Partition (DIP) structure using a
 * HW derived key. The DIP is used for storing integrity information for
 * Mobile Device Theft Protection (MDTP) service.
 *
 * @in_buf[in] : Pointer to plain text buffer.
 * @in_buf_size[in] : Plain text buffer size.
 * @out_buf[in] : Pointer to encrypted buffer.
 * @out_buf_size[in] : Encrypted buffer size.
 * @direction[in] : 0 for ENCRYPTION, 1 for DECRYPTION.
 *
 * Returns 0 on success, negative on failure.
 */
int mdtp_cipher_dip_cmd(uint8_t *in_buf, uint32_t in_buf_size, uint8_t *out_buf,
                          uint32_t out_buf_size, uint32_t direction);

void set_tamper_fuse_cmd(void);

/**
 * Reads the row data of the specified QFPROM row address.
 *
 * @row_address[in] : Row address in the QFPROM region to read.
 * @addr_type[in] : Raw or corrected address.
 * @row_data[in] : Pointer to the data to be read.
 * @qfprom_api_status[out] : Status of the read operation.
 *
 * Returns Any errors while reading data from the specified
 * Returns 0 on success, negative on failure.
 */
int qfprom_read_row_cmd(uint32_t row_address, uint32_t addr_type, uint32_t *row_data, uint32_t *qfprom_api_status);

int scm_halt_pmic_arbiter(void);
int scm_call_atomic(uint32_t svc, uint32_t cmd, uint32_t arg1);
int scm_call_atomic2(uint32_t svc, uint32_t cmd, uint32_t arg1, uint32_t arg2);
int restore_secure_cfg(uint32_t id);

void scm_elexec_call(paddr_t kernel_entry, paddr_t dtb_offset);
int scm_random(uint32_t * rbuf, uint32_t  r_len);
/* API to configure XPU violations as fatal */
int scm_xpu_err_fatal_init(void);

/* APIs to support ARM scm standard
 * Takes arguments : x0-x5 and returns result
 * in x0-x3*/
uint32_t scm_call2(scmcall_arg *arg, scmcall_ret *ret);
int scm_call2_atomic(uint32_t svc, uint32_t cmd, uint32_t arg1, uint32_t arg2);

/* Perform any scm init needed before making scm calls
 * Used for checking if armv8 SCM support present
 */
void scm_init(void);
bool is_secure_boot_enable(void);

/* Is armv8 supported */
bool is_scm_armv8_support(void);

int scm_dload_mode(int mode);

uint32_t scm_io_read(addr_t address);
uint32_t scm_io_write(uint32_t address, uint32_t val);

#endif
