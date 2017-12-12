#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014-2016, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#

################################################################################
#
# FV Section
#
# [FV] section is used to define what components or modules are placed within a flash
# device file.  This section also defines order the components and modules are positioned
# within the image.  The [FV] section consists of define statements, set statements and
# module statements.
#
################################################################################

[FV.FvMain]
FvNameGuid         = 8ff51dbd-b856-4acb-b859-8536d892ed3a
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

  APRIORI DXE {
    # first, install the PCD driver and call DxeInit
    INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
    INF EFIDroidPkg/Drivers/DxeInit/DxeInit.inf

    # next, install cpu protocol and enable the interrupt controller
    INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
    INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

    # next, setup the FDT client and setup all memory regions
    INF EFIDroidPkg/Drivers/FdtClientDxe/FdtClientDxe.inf
    INF EFIDroidPkg/Drivers/HighMemDxe/HighMemDxe.inf

    # finally, call DxeInitPostGic
    INF EFIDroidPkg/Drivers/DxeInit/DxeInitPostGic.inf
  }

  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF EFIDroidPkg/Drivers/DxeInit/DxeInit.inf
  INF EFIDroidPkg/Drivers/DxeInit/DxeInitPostGic.inf

  #
  # PI DXE Drivers producing Architectural Protocols (EFI Services)
  #
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
  INF MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf
!if $(SECURE_BOOT_ENABLE) == TRUE
  INF SecurityPkg/VariableAuthenticated/SecureBootConfigDxe/SecureBootConfigDxe.inf
!endif
  INF MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  #
  # TimestampDxe
  #
  INF MdeModulePkg/Universal/TimestampDxe/TimestampDxe.inf

  #
  # Multiple Console IO support
  #
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  INF MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  INF MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

  #
  # FAT filesystem + GPT/MBR partitioning + UDF filesystem
  #
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/Disk/UdfDxe/UdfDxe.inf

  #
  # FV FileSystem
  #
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

  #
  # Thread support
  #
  INF EFIDroidLKLPkg/UEFIThreads/UEFIThreads.inf

  #
  # LKL support
  #
  INF EFIDroidLKLPkg/LKL/LKL.inf

  #
  # LKL filesystem
  #
  INF EFIDroidLKLPkg/LKLFS/LKLFS.inf

  #
  # Platform Driver
  #
  INF EFIDroidPkg/Drivers/FdtClientDxe/FdtClientDxe.inf
  INF EFIDroidPkg/Drivers/HighMemDxe/HighMemDxe.inf

  #
  # UEFI application (Shell Embedded Boot Loader)
  #
  INF ShellPkg/Application/Shell/Shell.inf

  #
  # Bds
  #
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  INF MdeModulePkg/Universal/DriverHealthManagerDxe/DriverHealthManagerDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Application/UiApp/UiApp.inf

  #
  # TianoCore logo (splash screen)
  #
  INF MdeModulePkg/Logo/LogoDxe.inf

  #
  # Ramdisk support
  #
  INF MdeModulePkg/Universal/Disk/RamDiskDxe/RamDiskDxe.inf

  #
  # ChipVendor FV
  #
  FILE FV_IMAGE = B67596E9-EA27-40E4-885C-5AE83D414BD4 {
    SECTION FV_IMAGE = FvMain$(EFIDROID_CHIPVENDOR)
  }

  #
  # Platform FV
  #
  FILE FV_IMAGE = 61FD7C3E-E7F1-4F2D-9509-382F8CAE8B67 {
    SECTION FV_IMAGE = FvMain$(EFIDROID_PLATFORM)
  }

  #
  # Device FV
  #
  FILE FV_IMAGE = 66A648D0-9913-4331-AEBF-C06E783384AE {
    SECTION FV_IMAGE = FvMainDevice
  }
