#include "MMCHS.h"

QCOM_BAM_PROTOCOL *mBam = NULL;

MMC_PLATFORM_CALLBACK_API mPlatformCallbackApi = {
  mmc_boot_main,
  mmc_boot_mci_clk_enable,
  mmc_boot_mci_clk_disable,
};

STATIC BIO_INSTANCE mBioTemplate = {
  BIO_INSTANCE_SIGNATURE,
  NULL, // Handle
  { // BlockIo
    EFI_BLOCK_IO_INTERFACE_REVISION,   // Revision
    NULL,                              // *Media
    MMCHSReset,                        // Reset
    MMCHSReadBlocks,                   // ReadBlocks
    MMCHSWriteBlocks,                  // WriteBlocks
    MMCHSFlushBlocks                   // FlushBlocks
  },
  { // BlockMedia
    BIO_INSTANCE_SIGNATURE,                   // MediaId
    FALSE,                                    // RemovableMedia
    TRUE,                                     // MediaPresent
    FALSE,                                    // LogicalPartition
    FALSE,                                    // ReadOnly
    FALSE,                                    // WriteCaching
    0,                                        // BlockSize
    4,                                        // IoAlign
    0,                                        // Pad
    0                                         // LastBlock
  },
  { // DevicePath
   {
      {
        HARDWARE_DEVICE_PATH, HW_VENDOR_DP,
        { (UINT8) (sizeof(VENDOR_DEVICE_PATH)), (UINT8) ((sizeof(VENDOR_DEVICE_PATH)) >> 8) },
      },
      // Hardware Device Path for Bio
      EFI_CALLER_ID_GUID // Use the driver's GUID
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      { sizeof (EFI_DEVICE_PATH_PROTOCOL), 0 }
    }
  }
};

EFI_STATUS
EFIAPI
MMCHSReset (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN BOOLEAN                        ExtendedVerification
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MMCHSReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  OUT VOID                          *Buffer
  )
{
  BIO_INSTANCE              *Instance;
  EFI_BLOCK_IO_MEDIA        *Media;
  UINTN                     BlockSize;
  UINTN                     RC;

  Instance  = BIO_INSTANCE_FROM_BLOCKIO_THIS(This);
  Media     = &Instance->BlockMedia;
  BlockSize = Media->BlockSize;

  if (MediaId != Media->MediaId) {
    return EFI_MEDIA_CHANGED;
  }

  if (Lba > Media->LastBlock) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Lba + (BufferSize / BlockSize) - 1) > Media->LastBlock) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize % BlockSize != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize == 0) {
    return EFI_SUCCESS;
  }

  RC = mmc_read ((UINT64) Lba * BlockSize, Buffer, BufferSize);
  if (RC == MMC_BOOT_E_SUCCESS)
    return EFI_SUCCESS;
  else
    return EFI_DEVICE_ERROR;
}

EFI_STATUS
EFIAPI
MMCHSWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  IN VOID                           *Buffer
  )
{
  BIO_INSTANCE              *Instance;
  EFI_BLOCK_IO_MEDIA        *Media;
  UINTN                      BlockSize;
  UINTN                     RC;

  Instance  = BIO_INSTANCE_FROM_BLOCKIO_THIS(This);
  Media     = &Instance->BlockMedia;
  BlockSize = Media->BlockSize;

  if (MediaId != Media->MediaId) {
    return EFI_MEDIA_CHANGED;
  }

  if (Lba > Media->LastBlock) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Lba + (BufferSize / BlockSize) - 1) > Media->LastBlock) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize % BlockSize != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize == 0) {
    return EFI_SUCCESS;
  }

  RC = mmc_write ((UINT64) Lba * BlockSize, BufferSize, Buffer);
  if (RC == MMC_BOOT_E_SUCCESS)
    return EFI_SUCCESS;
  else
    return EFI_DEVICE_ERROR;
}

EFI_STATUS
EFIAPI
MMCHSFlushBlocks (
  IN EFI_BLOCK_IO_PROTOCOL  *This
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
BioInstanceContructor (
  OUT BIO_INSTANCE** NewInstance
  )
{
  BIO_INSTANCE* Instance;

  Instance = AllocateCopyPool (sizeof(BIO_INSTANCE), &mBioTemplate);
  if (Instance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Instance->BlockIo.Media = &Instance->BlockMedia;

  *NewInstance = Instance;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MMCHSInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS      Status;
  BIO_INSTANCE    *Instance;

  if (FeaturePcdGet(PcdMmcBamSupport)) {
    Status = gBS->LocateProtocol (&gQcomBamProtocolGuid, NULL, (VOID **)&mBam);
    if (EFI_ERROR(Status)) {
      return EFI_NOT_READY;
    }
  }

  // Initialize device
  LibQcomTargetMmcInit (&mPlatformCallbackApi);

  // allocate instance
  Status = BioInstanceContructor (&Instance);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // set data
  Instance->BlockMedia.BlockSize = mmc_get_device_blocksize();
  Instance->BlockMedia.LastBlock = mmc_get_device_capacity()/Instance->BlockMedia.BlockSize - 1;

  // Publish BlockIO
  Status = gBS->InstallMultipleProtocolInterfaces (
              &Instance->Handle,
              &gEfiBlockIoProtocolGuid,    &Instance->BlockIo,
              &gEfiDevicePathProtocolGuid, &Instance->DevicePath,
              NULL
              );

  return Status;
}
