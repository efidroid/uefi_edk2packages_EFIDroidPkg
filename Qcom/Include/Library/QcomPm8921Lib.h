#ifndef __LIBRARY_QCOM_PM8921_LIB_H__
#define __LIBRARY_QCOM_PM8921_LIB_H__

#include <Protocol/QcomPm8921.h>

RETURN_STATUS
EFIAPI
Pm8921ImplLibInitialize (
  VOID
  );

extern QCOM_PM8921_PROTOCOL *gPM8921;

#endif
