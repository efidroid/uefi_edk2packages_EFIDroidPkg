#ifndef __LIBRARY_QCOM_SCM_LIB_H__
#define __LIBRARY_QCOM_SCM_LIB_H__

#include <Protocol/QcomScm.h>

RETURN_STATUS
EFIAPI
ScmImplLibInitialize (
  VOID
  );

extern QCOM_SCM_PROTOCOL *gScm;

#endif
