[LibraryClasses.common]
  PlatformDxeInitLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/PlatformDxeInitLib/PlatformDxeInitLib.inf
  EfiResetSystemLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/ResetSystemLib/ResetSystemLib.inf
  TimerLib|EFIDroidPkg/Qcom/Library/GPTTimerLib/GPTTimerLib.inf

  QcomPlatformClockInitLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformClockInitLib/QcomPlatformClockInitLib.inf
  QcomPlatformDxeTimerLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformDxeTimerLib/QcomPlatformDxeTimerLib.inf
  QcomPlatformUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformUartDmLib/QcomPlatformUartDmLib.inf
  QcomPlatformGpioTlmmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformGpioTlmmLib/QcomPlatformGpioTlmmLib.inf
  QcomPlatformDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomPlatformDisplayLib/QcomPlatformDisplayLib.inf

  QcomTargetBoardLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetBoardLib/QcomTargetBoardLib.inf
  QcomTargetUartDmLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetUartDmLib/QcomTargetUartDmLib.inf
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Platforms/Msm8960/Library/QcomTargetDisplayLib/QcomTargetDisplayLib.inf

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
