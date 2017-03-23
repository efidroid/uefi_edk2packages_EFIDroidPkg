[LibraryClasses.common]
  RealTimeClockLib|EFIDroidPkg/Qcom/Library/RealTimeClockLib/RealTimeClockLib.inf
  PTableLib|EFIDroidPkg/Qcom/Library/PTableLib/PTableLib.inf
  FbConLib|EFIDroidPkg/Qcom/Library/FbConLib/FbConLib.inf

[LibraryClasses.common.DXE_DRIVER]
  ChipVendorDxeInitLib|EFIDroidPkg/Qcom/Library/ChipVendorDxeInitLib/ChipVendorDxeInitLib.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER]
  # this allows us to boot without a screen without GraphicsOutputDxe complaining
  QcomTargetDisplayLib|EFIDroidPkg/Qcom/Library/QcomTargetDisplayLibNull/QcomTargetDisplayLibNull.inf

[Components.common]
  EFIDroidPkg/Qcom/Drivers/TimerDxe/TimerDxe.inf
  EFIDroidPkg/Qcom/Drivers/GraphicsOutputDxe/GraphicsOutputDxe.inf
