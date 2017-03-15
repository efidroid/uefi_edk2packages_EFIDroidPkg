[LibraryClasses.common]
  EfiResetSystemLib|EFIDroidPkg/Qcom/Library/ResetSystemLib/ResetSystemLib.inf
  TimerLib|EFIDroidPkg/Qcom/Library/TimerLib/TimerLib.inf
  RealTimeClockLib|EFIDroidPkg/Qcom/Library/RealTimeClockLib/RealTimeClockLib.inf
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/UartDmSerialPortLib.inf
  ChipVendorDxeInitLib|EFIDroidPkg/Qcom/Library/ChipVendorDxeInitLib/ChipVendorDxeInitLib.inf

[LibraryClasses.common.SEC]
  SerialPortLib|EFIDroidPkg/Qcom/Library/UartDmSerialPortLib/EarlyUartDmSerialPortLib.inf

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
