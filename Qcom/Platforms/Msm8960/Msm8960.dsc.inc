!include EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialConfigNormal.dsc.inc

[LibraryClasses.common]
  ResetSystemLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/ResetSystemLib/ResetSystemLib.inf
  TimerLib|EFIDroidPkg/Qcom/Library/GPTTimerLib/GPTTimerLib.inf

  # platform
  QcomPlatformClockInitLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformClockInitLib/QcomPlatformClockInitLib.inf
  QcomPlatformUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformUartDmLib/QcomPlatformUartDmLib.inf
  QcomPlatformDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformDisplayLib/QcomPlatformDisplayLib.inf
  QcomPlatformDisplayMinimalLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformDisplayLib/QcomPlatformDisplayMinimalLib.inf
  QcomPlatformMmcLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformMmcLib/QcomPlatformMmcLib.inf

  # target
  QcomTargetBoardLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetBoardLib/QcomTargetBoardLib.inf
  QcomTargetUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetUartDmLib/QcomTargetUartDmLib.inf
  QcomTargetMmcLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetMmcLib/QcomTargetMmcLib.inf

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
  GpioTlmmLib|EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmImplLib.inf
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
  QcomPlatformUsbLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformUsbLib/QcomPlatformUsbLib.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_APPLICATION, LibraryClasses.common.UEFI_DRIVER]
  # protocol libs
  SmemLib|EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemLib.inf
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardLib.inf
  ClockLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockLib.inf
  GpioTlmmLib|EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmLib.inf
  SsbiLib|EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiLib.inf
  Pm8921Lib|EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Lib.inf
  I2cQupLib|EFIDroidPkg/Qcom/Drivers/I2cQupDxe/I2cQupLib.inf

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

  gQcomTokenSpaceGuid.PcdGpioTlmmBaseAddress|0x00800000
  gQcomTokenSpaceGuid.PcdGpioTlmmSummaryIrq|48

  gQcomTokenSpaceGuid.PcdGpioTlmmIoOffset|0x1004
  gQcomTokenSpaceGuid.PcdGpioTlmmIoElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmCtlOffset|0x1000
  gQcomTokenSpaceGuid.PcdGpioTlmmCtlElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrCfgOffset|0x1008
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrCfgElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrStatusOffset|0x100c
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrStatusElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetOffset|0x400
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetElementSize|0x4

  gQcomTokenSpaceGuid.PcdGpioTlmmIntrEnableBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrStatusBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrAckHigh|TRUE

  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetKpssValue|4
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrRawStatusBit|3
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrPolarityBit|1
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrDetectionBit|2
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrDetectionWidth|1

  gQcomTokenSpaceGuid.PcdGpioTlmmInBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmOutBit|1
  gQcomTokenSpaceGuid.PcdGpioTlmmOeBit|9
  gQcomTokenSpaceGuid.PcdGpioTlmmMuxBit|2
  gQcomTokenSpaceGuid.PcdGpioTlmmDrvBit|6
  gQcomTokenSpaceGuid.PcdGpioTlmmPullBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmNumFunctions|11

  gQcomTokenSpaceGuid.PcdUsbHsBase|0x12500000
  gQcomTokenSpaceGuid.PcdUsbHsIrq|132

[Components.common]
  EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemDxe.inf
  EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmInterruptDxe/GpioTlmmInterruptDxe.inf
  EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiDxe.inf
  EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Dxe.inf
  EFIDroidPkg/Qcom/Drivers/MMCHSDxe/MMCHS.inf
  EFIDroidPkg/Qcom/Drivers/I2cQupDxe/I2cQupDxe.inf
  EFIDroidPkg/Qcom/Drivers/UsbDxe/UsbDxe.inf
