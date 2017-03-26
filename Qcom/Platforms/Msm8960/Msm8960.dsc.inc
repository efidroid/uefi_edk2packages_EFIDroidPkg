!include EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialConfigNormal.dsc.inc

[LibraryClasses.common]
  EfiResetSystemLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/ResetSystemLib/ResetSystemLib.inf
  TimerLib|EFIDroidPkg/Qcom/Library/GPTTimerLib/GPTTimerLib.inf

  # platform
  QcomPlatformClockInitLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformClockInitLib/QcomPlatformClockInitLib.inf
  QcomPlatformUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformUartDmLib/QcomPlatformUartDmLib.inf
  QcomPlatformGpioTlmmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformGpioTlmmLib/QcomPlatformGpioTlmmLib.inf
  QcomPlatformDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformDisplayLib/QcomPlatformDisplayLib.inf
  QcomPlatformDisplayMinimalLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformDisplayLib/QcomPlatformDisplayMinimalLib.inf

  # target
  QcomTargetBoardLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetBoardLib/QcomTargetBoardLib.inf
  QcomTargetUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetUartDmLib/QcomTargetUartDmLib.inf

  # target display
!if $(EARLYDEBUG_TYPE) == DISPLAY
  # always use the 2ndstage lib, because SEC has initialized the screen already
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTarget2ndStageDisplayLib/QcomTarget2ndStageDisplayLib.inf
!else

!if $(DISPLAY_TYPE) == 2NDSTAGE
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTarget2ndStageDisplayLib/QcomTarget2ndStageDisplayLib.inf
!endif
!if $(DISPLAY_TYPE) == NATIVE
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetDisplayLib/QcomTargetDisplayLib.inf
!endif

!endif

  # serial lib
!if $(EARLYDEBUG_TYPE) == UART
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialPortLib.inf
!endif
!if $(EARLYDEBUG_TYPE) == DISPLAY
  SerialPortLib|EFIDroidPkg/Qcom/Library/DisplaySerialPortLib/DisplaySerialPortLib.inf
!endif

[LibraryClasses.common.SEC, LibraryClasses.common.PEI_CORE, LibraryClasses.common.PEIM]
  # Implementation libs
  SmemLib|EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemImplLib.inf
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardImplLib.inf
  ClockLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockImplLib.inf
  GpioTlmmLib|EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmImplLib.inf
  SsbiLib|EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiImplLib.inf
  Pm8921Lib|EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921ImplLib.inf

!if $(DISPLAY_TYPE) == 2NDSTAGE
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTarget2ndStageDisplayLib/QcomTarget2ndStageDisplayLib.inf
!endif
!if $(DISPLAY_TYPE) == NATIVE
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetDisplayLib/QcomTargetDisplayLib.inf
!endif

  # early serial lib
!if $(EARLYDEBUG_TYPE) == UART
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/EarlyUartDmSerialPortLib.inf
!endif
!if $(EARLYDEBUG_TYPE) == DISPLAY
  SerialPortLib|EFIDroidPkg/Qcom/Library/DisplaySerialPortLib/EarlyDisplaySerialPortLib.inf
!endif

[LibraryClasses.common.DXE_DRIVER]
  PlatformDxeInitLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/PlatformDxeInitLib/PlatformDxeInitLib.inf
  QcomDxeTimerLib|EFIDroidPkg/Qcom/Library/QcomDGTDxeTimerLib/QcomDGTDxeTimerLib.inf
  QcomPlatformDxeTimerLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformDxeTimerLib/QcomPlatformDxeTimerLib.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_APPLICATION, LibraryClasses.common.UEFI_DRIVER]
  # protocol libs
  SmemLib|EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemLib.inf
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardLib.inf
  ClockLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockLib.inf
  GpioTlmmLib|EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmLib.inf
  SsbiLib|EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiLib.inf
  Pm8921Lib|EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Lib.inf

[PcdsFeatureFlag.common]
  # without this, the memory speed is very slow
  gArmTokenSpaceGuid.PcdNormalMemoryNonshareableOverride|TRUE

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x02000000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x02002000

  gQcomTokenSpaceGuid.PcdMsmSharedBase|0x80000000
  gQcomTokenSpaceGuid.PcdMsmSharedSize|0x00200000
  gQcomTokenSpaceGuid.PcdIntDebugTimerExp|17
  gQcomTokenSpaceGuid.PcdUartDmClkRxTxBitRate|0xFF

  gQcomTokenSpaceGuid.PcdMsmDgtBase|0x0200A024
  gQcomTokenSpaceGuid.PcdMsmDgtMatchValOffset|0x0000
  gQcomTokenSpaceGuid.PcdMsmDgtCountValOffset|0x0004
  gQcomTokenSpaceGuid.PcdMsmDgtEnableOffset|0x0008
  gQcomTokenSpaceGuid.PcdMsmDgtClearOffset|0x000C
  gQcomTokenSpaceGuid.PcdMsmDgtClkCtlOffset|0x0010

  gQcomTokenSpaceGuid.PcdMsmGptBase|0x0200A004
  gQcomTokenSpaceGuid.PcdMsmGptMatchValOffset|0x0000
  gQcomTokenSpaceGuid.PcdMsmGptCountValOffset|0x0004
  gQcomTokenSpaceGuid.PcdMsmGptEnableOffset|0x0008
  gQcomTokenSpaceGuid.PcdMsmGptClearOffset|0x000C

  gQcomTokenSpaceGuid.PcdSpssTimerStatusReg|0x0200A088

[Components.common]
  EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemDxe.inf
  EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiDxe.inf
  EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Dxe.inf
