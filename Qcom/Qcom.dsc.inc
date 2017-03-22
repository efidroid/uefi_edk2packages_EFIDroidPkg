[LibraryClasses.common]
  RealTimeClockLib|EFIDroidPkg/Qcom/Library/RealTimeClockLib/RealTimeClockLib.inf
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialPortLib.inf
  ChipVendorDxeInitLib|EFIDroidPkg/Qcom/Library/ChipVendorDxeInitLib/ChipVendorDxeInitLib.inf
  QcomDxeTimerLib|EFIDroidPkg/Qcom/Library/QcomDGTDxeTimerLib/QcomDGTDxeTimerLib.inf

  PTableLib|EFIDroidPkg/Qcom/Library/PTableLib/PTableLib.inf

  # protocol libs
  SmemLib|EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemLib.inf
  BoardLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardLib.inf
  ClockLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockLib.inf
  GpioTlmmLib|EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmLib.inf
  Pm8921Lib|EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Lib.inf

[LibraryClasses.common.SEC]
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/EarlyUartDmSerialPortLib.inf

  # Implementation libs
  SmemImplLib|EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemImplLib.inf
  BoardImplLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardImplLib.inf
  ClockImplLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockImplLib.inf
  GpioTlmmImplLib|EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmImplLib.inf

[LibraryClasses.common.PEI_CORE]
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/EarlyUartDmSerialPortLib.inf

[LibraryClasses.common.PEIM]
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/EarlyUartDmSerialPortLib.inf

[Components.common]
  EFIDroidPkg/Qcom/Drivers/TimerDxe/TimerDxe.inf

  #
  # Video support
  #
  EFIDroidPkg/Qcom/Drivers/GraphicsOutputDxe/GraphicsOutputDxe.inf

  EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemDxe.inf
  EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiDxe.inf
  EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Dxe.inf
