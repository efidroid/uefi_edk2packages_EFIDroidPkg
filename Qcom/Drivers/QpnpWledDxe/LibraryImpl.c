#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomQpnpWledLib.h>

#include "qpnp_wled_p.h"

QCOM_QPNP_WLED_PROTOCOL *gQpnpWled = NULL;

STATIC QCOM_QPNP_WLED_PROTOCOL mInternalQpnpWled = {
  qpnp_wled_init,
  qpnp_ibb_enable,
  qpnp_wled_enable_backlight,
};

RETURN_STATUS
EFIAPI
QpnpWledImplLibInitialize (
  VOID
  )
{
  gQpnpWled = &mInternalQpnpWled;
  return RETURN_SUCCESS;
}
