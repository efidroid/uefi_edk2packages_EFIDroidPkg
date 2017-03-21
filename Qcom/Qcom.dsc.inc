[LibraryClasses.common]
  RealTimeClockLib|EFIDroidPkg/Qcom/Library/RealTimeClockLib/RealTimeClockLib.inf
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialPortLib.inf
  ChipVendorDxeInitLib|EFIDroidPkg/Qcom/Library/ChipVendorDxeInitLib/ChipVendorDxeInitLib.inf
  QcomDxeTimerLib|EFIDroidPkg/Qcom/Library/QcomDGTDxeTimerLib/QcomDGTDxeTimerLib.inf

  PTableLib|EFIDroidPkg/Qcom/Library/PTableLib/PTableLib.inf

[LibraryClasses.common.SEC]
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/EarlyUartDmSerialPortLib.inf

  SmemSecLib|EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemSecLib.inf
  BoardSecLib|EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardSecLib.inf
  ClockSecLib|EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockSecLib.inf
  GpioTlmmSecLib|EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmSecLib.inf

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

  EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiDxe.inf
  EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemDxe.inf
  EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Dxe.inf
  EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
