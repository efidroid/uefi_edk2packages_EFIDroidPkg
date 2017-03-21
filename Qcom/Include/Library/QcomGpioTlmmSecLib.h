#ifndef __LIBRARY_QCOM_GPIO_TLMM_SECLIB_H__
#define __LIBRARY_QCOM_GPIO_TLMM_SECLIB_H__

#include <Protocol/QcomGpioTlmm.h>

RETURN_STATUS
EFIAPI
GpioTlmmSecLibConstructor (
  VOID
  );

extern QCOM_GPIO_TLMM_PROTOCOL *gGpioTlmm;

#endif
