#ifndef __LIBRARY_QCOM_SMEM_SECLIB_H__
#define __LIBRARY_QCOM_SMEM_SECLIB_H__

#include <Protocol/QcomSmem.h>

RETURN_STATUS
EFIAPI
SmemSecLibConstructor (
  VOID
  );

extern QCOM_SMEM_PROTOCOL *gSMEM;

#endif
