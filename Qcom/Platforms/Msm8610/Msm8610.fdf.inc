[FV.FvMainMsm8610]
FvNameGuid         = 1208B3C1-51D5-4381-8194-67AA8B73C161
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
  INF EFIDroidPkg/Qcom/Drivers/SmemPtableDxe/SmemPtableDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/BoardDxe/BoardDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/ClockDxe/ClockDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/SpmiDxe/SpmiDxe.inf
  INF EFIDroidPkg/Qcom/Drivers/Pm8x41Dxe/Pm8x41Dxe.inf
  INF EFIDroidPkg/Qcom/Drivers/ScmDxe/ScmDxe.inf
  #INF EFIDroidPkg/Qcom/Drivers/QpnpWledDxe/QpnpWledDxe.inf
  #INF EFIDroidPkg/Qcom/Drivers/SmdRpmDxe/RpmDxe.inf
