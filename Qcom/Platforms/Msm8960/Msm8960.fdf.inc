[FV.FvMainMsm8960]
FvNameGuid         = d1d5e5ff-c8dd-4316-b689-4d7bb5d1bb98
BlockSize          = 0x40
NumBlocks          = 0         # This FV gets compressed so make it just big enough
FvAlignment        = 16        # FV alignment and FV attributes setting.
ERASE_POLARITY     = 1
MEMORY_MAPPED      = TRUE
STICKY_WRITE       = TRUE
LOCK_CAP           = TRUE
LOCK_STATUS        = TRUE
WRITE_DISABLED_CAP = TRUE
WRITE_ENABLED_CAP  = TRUE
WRITE_STATUS       = TRUE
WRITE_LOCK_CAP     = TRUE
WRITE_LOCK_STATUS  = TRUE
READ_DISABLED_CAP  = TRUE
READ_ENABLED_CAP   = TRUE
READ_STATUS        = TRUE
READ_LOCK_CAP      = TRUE
READ_LOCK_STATUS   = TRUE

  INF EFIDroidPkg/Qcom/Drivers/SmemDxe/SmemDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  INF EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  INF EFIDroidGPLPkg/Qcom/Drivers/GpioTlmmInterruptDxe/GpioTlmmInterruptDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/SsbiDxe/SsbiDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/Pm8921Dxe/Pm8921Dxe.inf
  INF EFIDroidPkg/Qcom/Drivers/MMCHSDxe/MMCHS.inf
  INF EFIDroidPkg/Qcom/Drivers/I2cQupDxe/I2cQupDxe.inf
