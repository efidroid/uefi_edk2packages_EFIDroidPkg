!include EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialConfigBlkBlsp.dsc.inc

[LibraryClasses.common]
  EfiResetSystemLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/ResetSystemLib/ResetSystemLib.inf
  TimerLib|EFIDroidPkg/Qcom/Library/QTimerLib/QTimerLib.inf

  # platform
  QcomPlatformClockInitLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformClockInitLib/QcomPlatformClockInitLib.inf
  QcomPlatformUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformUartDmLib/QcomPlatformUartDmLib.inf
  QcomPlatformGpioTlmmLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformGpioTlmmLib/QcomPlatformGpioTlmmLib.inf
  QcomPlatformDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8994/Library/QcomPlatformDisplayLib/QcomPlatformDisplayLib.inf

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
  GpioTlmmLib|EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmImplLib.inf
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
  GpioTlmmLib|EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmLib.inf
  SpmiLib|EFIDroidPkg/Qcom/Drivers/SpmiDxe/SpmiLib.inf
  Pm8x41Lib|EFIDroidPkg/Qcom/Drivers/Pm8x41Dxe/Pm8x41Lib.inf
  ScmLib|EFIDroidPkg/Qcom/Drivers/ScmDxe/ScmLib.inf
  QpnpWledLib|EFIDroidPkg/Qcom/Drivers/QpnpWledDxe/QpnpWledLib.inf
  RpmLib|EFIDroidPkg/Qcom/Drivers/SmdRpmDxe/RpmLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardLib.inf

[PcdsFeatureFlag.common]
  # without this, the memory speed is very slow
  gArmTokenSpaceGuid.PcdNormalMemoryNonshareableOverride|TRUE

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xF9000000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xF9002000

  gQcomTokenSpaceGuid.PcdMsmSharedBase|0x06A00000
  gQcomTokenSpaceGuid.PcdMsmSharedSize|0x00200000
  gQcomTokenSpaceGuid.PcdIntDebugTimerExp|41
  gQcomTokenSpaceGuid.PcdQTimerBase|0xF9021000
  gQcomTokenSpaceGuid.PcdUartDmClkRxTxBitRate|0xCC

  gQcomTokenSpaceGuid.PcdSpmiBaseAddress|0xFC4C0000
  gQcomTokenSpaceGuid.PcdApcsAlias0IpcInterrupt|0xF900D008
  gQcomTokenSpaceGuid.PcdSmdIrq|200

[Components.common]
  EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemDxe.inf
  EFIDroidPkg/Qcom/Drivers/SmemPtableDxe/SmemPtableDxe.inf
  EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  EFIDroidPkg/Qcom/Drivers/SpmiDxe/SpmiDxe.inf
  EFIDroidPkg/Qcom/Drivers/Pm8x41Dxe/Pm8x41Dxe.inf
  EFIDroidPkg/Qcom/Drivers/ScmDxe/ScmDxe.inf
  EFIDroidPkg/Qcom/Drivers/QpnpWledDxe/QpnpWledDxe.inf
  EFIDroidPkg/Qcom/Drivers/SmdRpmDxe/RpmDxe.inf
