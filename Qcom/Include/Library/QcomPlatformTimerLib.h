#ifndef __LIBRARY_QCOM_PLATFORM_DXE_TIMER_IMPL_LIB_H__
#define __LIBRARY_QCOM_PLATFORM_DXE_TIMER_IMPL_LIB_H__

RETURN_STATUS
EFIAPI
LibQcomPlatformTimerInitialize (
  VOID
  );

UINTN
EFIAPI
LibQcomPlatformTimerGetFreq (
  VOID
  );

#endif
