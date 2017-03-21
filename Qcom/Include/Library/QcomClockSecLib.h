#ifndef __LIBRARY_QCOM_CLOCK_SECLIB_H__
#define __LIBRARY_QCOM_CLOCK_SECLIB_H__

#include <Protocol/QcomClock.h>

RETURN_STATUS
EFIAPI
ClockSecLibConstructor (
  VOID
  );

extern QCOM_CLOCK_PROTOCOL *gClock;

#endif
