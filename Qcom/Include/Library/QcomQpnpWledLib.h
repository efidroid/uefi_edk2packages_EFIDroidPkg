#ifndef __LIBRARY_QCOM_QPNP_WLED_LIB_H__
#define __LIBRARY_QCOM_QPNP_WLED_LIB_H__

#include <Protocol/QcomQpnpWled.h>

RETURN_STATUS
EFIAPI
QpnpWledImplLibInitialize (
  VOID
  );

extern QCOM_QPNP_WLED_PROTOCOL *gQpnpWled;

#endif
