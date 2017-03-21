#ifndef __LIBRARY_QCOM_PLATFORM_DXE_TIMER_LIB_H__
#define __LIBRARY_QCOM_PLATFORM_DXE_TIMER_LIB_H__

RETURN_STATUS
EFIAPI
LibQcomPlatformDxeTimerInitialize (
  VOID
  );

UINTN
EFIAPI
LibQcomPlatformDxeTimerGetFreq (
  VOID
  );

#endif
