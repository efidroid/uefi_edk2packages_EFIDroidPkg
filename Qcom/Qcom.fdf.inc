[FV.FvMainQcom]
FvNameGuid         = de7b5f67-7d94-4f11-ac12-0a9e70264076
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

  INF EFIDroidPkg/Qcom/Drivers/TimerDxe/TimerDxe.inf

  !if $(DISPLAY_TYPE) != DISABLED && $(EARLYDEBUG_TYPE) != DISPLAY
  INF EFIDroidPkg/Qcom/Drivers/GraphicsOutputDxe/GraphicsOutputDxe.inf
  !else
  INF EFIDroidPkg/Qcom/Drivers/EmuGraphicsOutputDxe/EmuGraphicsOutputDxe.inf
  !endif
