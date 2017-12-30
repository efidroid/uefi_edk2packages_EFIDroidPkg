!include EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialConfigBlkBlsp.dsc.inc

[LibraryClasses.common]
  ResetSystemLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/ResetSystemLib/ResetSystemLib.inf
  TimerLib|EFIDroidPkg/Qcom/Library/QTimerLib/QTimerLib.inf

  # platform
  QcomPlatformClockInitLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformClockInitLib/QcomPlatformClockInitLib.inf
  QcomPlatformUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformUartDmLib/QcomPlatformUartDmLib.inf
  QcomPlatformDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformDisplayLib/QcomPlatformDisplayLib.inf
  QcomPlatformMmcLib|EFIDroidPkg/Qcom/Library/FdtPlatformMmcLib/QcomPlatformMmcLib.inf
  QcomPlatformMmcClockOverrideLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformMmcClockOverrideLib/QcomPlatformMmcClockOverrideLib.inf

  # target
  QcomTargetBoardLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomTargetBoardLib/QcomTargetBoardLib.inf
  QcomTargetUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomTargetUartDmLib/QcomTargetUartDmLib.inf

  # target display
!if $(EARLYDEBUG_TYPE) == DISPLAY
  # always use the 2ndstage lib, because SEC has initialized the screen already
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomTarget2ndStageDisplayLib/QcomTarget2ndStageDisplayLib.inf
!else

!if $(DISPLAY_TYPE) == 2NDSTAGE
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomTarget2ndStageDisplayLib/QcomTarget2ndStageDisplayLib.inf
!endif
!if $(DISPLAY_TYPE) == NATIVE
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomTargetDisplayLib/QcomTargetDisplayLib.inf
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
  SmemPtableLib|EFIDroidPkg/Qcom/Drivers/SmemPtableDxe/SmemPtableImplLib.inf
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardImplLib.inf
  ClockLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockImplLib.inf
  GpioTlmmLib|EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmImplLib.inf
  SpmiLib|EFIDroidPkg/Qcom/Drivers/SpmiDxe/SpmiImplLib.inf
  Pm8x41Lib|EFIDroidPkg/Qcom/Drivers/Pm8x41Dxe/Pm8x41ImplLib.inf
  ScmLib|EFIDroidPkg/Qcom/Drivers/ScmDxe/ScmImplLib.inf
  QpnpWledLib|EFIDroidPkg/Qcom/Drivers/QpnpWledDxe/QpnpWledImplLib.inf

!if $(DISPLAY_TYPE) == 2NDSTAGE
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomTarget2ndStageDisplayLib/QcomTarget2ndStageDisplayLib.inf
!endif
!if $(DISPLAY_TYPE) == NATIVE
  # this doesn't work because we need RPM
  #QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomTargetDisplayLib/QcomTargetDisplayLib.inf
!endif

  # early serial lib
!if $(EARLYDEBUG_TYPE) == UART
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/EarlyUartDmSerialPortLib.inf
!endif
!if $(EARLYDEBUG_TYPE) == DISPLAY
  SerialPortLib|EFIDroidPkg/Qcom/Library/DisplaySerialPortLib/EarlyDisplaySerialPortLib.inf
!endif

[LibraryClasses.common.DXE_DRIVER]
  PlatformDxeInitLib|EFIDroidPkg/Qcom/Library/PlatformDxeInitLibNull/PlatformDxeInitLibNull.inf
  QcomDxeTimerLib|EFIDroidPkg/Qcom/Library/QTimerLib/QcomQTimerDxeTimerLib.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_APPLICATION, LibraryClasses.common.UEFI_DRIVER]
  # protocol libs
  SmemLib|EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemLib.inf
  SmemPtableLib|EFIDroidPkg/Qcom/Drivers/SmemPtableDxe/SmemPtableLib.inf
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardLib.inf
  ClockLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockLib.inf
  GpioTlmmLib|EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmLib.inf
  SpmiLib|EFIDroidPkg/Qcom/Drivers/SpmiDxe/SpmiLib.inf
  Pm8x41Lib|EFIDroidPkg/Qcom/Drivers/Pm8x41Dxe/Pm8x41Lib.inf
  ScmLib|EFIDroidPkg/Qcom/Drivers/ScmDxe/ScmLib.inf
  QpnpWledLib|EFIDroidPkg/Qcom/Drivers/QpnpWledDxe/QpnpWledLib.inf
  RpmLib|EFIDroidPkg/Qcom/Drivers/SmdRpmDxe/RpmLib.inf
  I2cQupLib|EFIDroidPkg/Qcom/Drivers/I2cQupDxe/I2cQupLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardLib.inf

[PcdsFeatureFlag.common]
  # without this, the memory speed is very slow
  gArmTokenSpaceGuid.PcdNormalMemoryNonshareableOverride|TRUE

  gQcomMsm8994TokenSpaceGuid.PcdDisplayEnable20nmPll90Phase|TRUE

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xF9000000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xF9002000
  gQcomTokenSpaceGuid.PcdGicSpiStart|32

  gQcomTokenSpaceGuid.PcdMsmSharedBase|0x06A00000
  gQcomTokenSpaceGuid.PcdMsmSharedSize|0x00200000
  gQcomTokenSpaceGuid.PcdIntDebugTimerExp|41
  gQcomTokenSpaceGuid.PcdQTimerBase|0xF9021000
  gQcomTokenSpaceGuid.PcdUartDmClkRxTxBitRate|0xCC

  gQcomTokenSpaceGuid.PcdSpmiBaseAddress|0xFC4C0000
  gQcomTokenSpaceGuid.PcdApcsAlias0IpcInterrupt|0xF900D008
  gQcomTokenSpaceGuid.PcdSmdIrq|200

  gQcomTokenSpaceGuid.PcdGpioTlmmBaseAddress|0xFD510000
  gQcomTokenSpaceGuid.PcdGpioTlmmSummaryIrq|240

  gQcomTokenSpaceGuid.PcdGpioTlmmIoOffset|0x1004
  gQcomTokenSpaceGuid.PcdGpioTlmmIoElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmCtlOffset|0x1000
  gQcomTokenSpaceGuid.PcdGpioTlmmCtlElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrCfgOffset|0x1008
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrCfgElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrStatusOffset|0x100c
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrStatusElementSize|0x10
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetOffset|0x1008
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetElementSize|0x10

  gQcomTokenSpaceGuid.PcdGpioTlmmIntrEnableBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrStatusBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrAckHigh|FALSE

  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetBit|5
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrTargetKpssValue|4
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrRawStatusBit|4
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrPolarityBit|1
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrDetectionBit|2
  gQcomTokenSpaceGuid.PcdGpioTlmmIntrDetectionWidth|2

  gQcomTokenSpaceGuid.PcdGpioTlmmInBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmOutBit|1
  gQcomTokenSpaceGuid.PcdGpioTlmmOeBit|9
  gQcomTokenSpaceGuid.PcdGpioTlmmMuxBit|2
  gQcomTokenSpaceGuid.PcdGpioTlmmDrvBit|6
  gQcomTokenSpaceGuid.PcdGpioTlmmPullBit|0
  gQcomTokenSpaceGuid.PcdGpioTlmmNumFunctions|12

  gQcomTokenSpaceGuid.PcdSdccMciHcMode|0x00000078
  gQcomTokenSpaceGuid.PcdSdccHcPwrctlStatusReg|0x000000DC
  gQcomTokenSpaceGuid.PcdSdccHcPwrctlMaskReg|0x000000E0
  gQcomTokenSpaceGuid.PcdSdccHcPwrctlClearReg|0x000000E4
  gQcomTokenSpaceGuid.PcdSdccHcPwrctlCtlReg|0x000000E8
  gQcomTokenSpaceGuid.PcdMmcSdhciDdrCfgVal|0x80040870 # DDR_CFG_DLY_VAL
  gQcomTokenSpaceGuid.PcdMmcSdc1HdrvPullCtlOffset|0x00002044
  gQcomTokenSpaceGuid.PcdMmcSdc2HdrvPullCtlOffset|0x00002048

[Components.common]
  EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemDxe.inf
  EFIDroidPkg/Qcom/Drivers/SmemPtableDxe/SmemPtableDxe.inf
  EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmInterruptDxe/GpioTlmmInterruptDxe.inf
  EFIDroidPkg/Qcom/Drivers/SpmiDxe/SpmiDxe.inf
  EFIDroidPkg/Qcom/Drivers/Pm8x41Dxe/Pm8x41Dxe.inf
  EFIDroidPkg/Qcom/Drivers/ScmDxe/ScmDxe.inf
  EFIDroidPkg/Qcom/Drivers/QpnpWledDxe/QpnpWledDxe.inf
  EFIDroidPkg/Qcom/Drivers/SmdRpmDxe/RpmDxe.inf
  EFIDroidPkg/Qcom/Drivers/I2cQupDxe/I2cQupDxe.inf
