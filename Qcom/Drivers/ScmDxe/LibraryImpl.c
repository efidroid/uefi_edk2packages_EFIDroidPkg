#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomScmLib.h>

#include "scm_p.h"

QCOM_SCM_PROTOCOL *gScm = NULL;

STATIC QCOM_SCM_PROTOCOL mInternalScm = {
  decrypt_scm,
  decrypt_scm_v2,
  encrypt_scm,
  scm_svc_version,
  scm_svc_get_secure_state,
  scm_protect_keystore,
  scm_call,
  scm_call_atomic,
  scm_call_atomic2,
  scm_call2,
  scm_call2_atomic,
  scm_halt_pmic_arbiter,
  scm_elexec_call,
  scm_xpu_err_fatal_init,
  scm_dload_mode,
  scm_random,
  scm_io_read,
  scm_io_write,
  switch_ce_chn_cmd,
  mdtp_cipher_dip_cmd,
  set_tamper_fuse_cmd,
  qfprom_read_row_cmd,
  restore_secure_cfg,
  is_secure_boot_enable,
  is_scm_armv8_support,
};

RETURN_STATUS
EFIAPI
ScmImplLibInitialize (
  VOID
  )
{
  gScm = &mInternalScm;

  scm_init();

  return RETURN_SUCCESS;
}
