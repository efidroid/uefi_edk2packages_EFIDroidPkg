#ifndef __LIBRARY_QCOMPLATFORMIMPLLIB_H__
#define __LIBRARY_QCOMPLATFORMIMPLLIB_H__

#include <Protocol/HardwareInterrupt.h>

VOID
EFIAPI
LibQcomTimerEnable (
  UINT64 TimerTicks
  );

VOID
EFIAPI
LibQcomTimerDisable (
  VOID
  );


EFI_STATUS
EFIAPI
LibQcomPlatformTimerInitialize (
  VOID
  );

UINTN
EFIAPI
LibQcomPlatformTimerGetFreq (
  VOID
  );

HARDWARE_INTERRUPT_SOURCE
EFIAPI
LibQcomPlatformTimerGetIntrNum (
  VOID
  );

#endif
