/** @file
  Implementation for a generic GOP driver.

Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/

#include "GraphicsOutput.h"

/**
  Returns information for an available graphics mode that the graphics device
  and the set of active video output devices supports.

  @param  This                  The EFI_GRAPHICS_OUTPUT_PROTOCOL instance.
  @param  ModeNumber            The mode number to return information on.
  @param  SizeOfInfo            A pointer to the size, in bytes, of the Info buffer.
  @param  Info                  A pointer to callee allocated buffer that returns information about ModeNumber.

  @retval EFI_SUCCESS           Valid mode information was returned.
  @retval EFI_DEVICE_ERROR      A hardware error occurred trying to retrieve the video mode.
  @retval EFI_INVALID_PARAMETER ModeNumber is not valid.

**/
EFI_STATUS
EFIAPI
GraphicsOutputQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  )
{
  if (This == NULL || Info == NULL || SizeOfInfo == NULL || ModeNumber >= This->Mode->MaxMode) {
    return EFI_INVALID_PARAMETER;
  }

  *SizeOfInfo = This->Mode->SizeOfInfo;
  *Info       = AllocateCopyPool (*SizeOfInfo, This->Mode->Info);
  return EFI_SUCCESS;
}

/**
  Set the video device into the specified mode and clears the visible portions of
  the output display to black.

  @param  This              The EFI_GRAPHICS_OUTPUT_PROTOCOL instance.
  @param  ModeNumber        Abstraction that defines the current video mode.

  @retval EFI_SUCCESS       The graphics mode specified by ModeNumber was selected.
  @retval EFI_DEVICE_ERROR  The device had an error and could not complete the request.
  @retval EFI_UNSUPPORTED   ModeNumber is not supported by this device.

**/
EFI_STATUS
EFIAPI
GraphicsOutputSetMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN  UINT32                       ModeNumber
)
{
  RETURN_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL    Black;
  GRAPHICS_OUTPUT_PRIVATE_DATA     *Private;

  if (ModeNumber >= This->Mode->MaxMode) {
    return EFI_UNSUPPORTED;
  }

  Private = GRAPHICS_OUTPUT_PRIVATE_FROM_THIS (This);

  Black.Blue = 0;
  Black.Green = 0;
  Black.Red = 0;
  Black.Reserved = 0;

  Status = FrameBufferBlt (
             Private->FrameBufferBltLibConfigure,
             &Black,
             EfiBltVideoFill,
             0, 0,
             0, 0,
             This->Mode->Info->HorizontalResolution,
             This->Mode->Info->VerticalResolution,
             0
             );
  return RETURN_ERROR (Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
}

/**
  Blt a rectangle of pixels on the graphics screen. Blt stands for BLock Transfer.

  @param  This         Protocol instance pointer.
  @param  BltBuffer    The data to transfer to the graphics screen.
                       Size is at least Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL).
  @param  BltOperation The operation to perform when copying BltBuffer on to the graphics screen.
  @param  SourceX      The X coordinate of source for the BltOperation.
  @param  SourceY      The Y coordinate of source for the BltOperation.
  @param  DestinationX The X coordinate of destination for the BltOperation.
  @param  DestinationY The Y coordinate of destination for the BltOperation.
  @param  Width        The width of a rectangle in the blt rectangle in pixels.
  @param  Height       The height of a rectangle in the blt rectangle in pixels.
  @param  Delta        Not used for EfiBltVideoFill or the EfiBltVideoToVideo operation.
                       If a Delta of zero is used, the entire BltBuffer is being operated on.
                       If a subrectangle of the BltBuffer is being used then Delta
                       represents the number of bytes in a row of the BltBuffer.

  @retval EFI_SUCCESS           BltBuffer was drawn to the graphics screen.
  @retval EFI_INVALID_PARAMETER BltOperation is not valid.
  @retval EFI_DEVICE_ERROR      The device had an error and could not complete the request.

**/
EFI_STATUS
EFIAPI
GraphicsOutputBlt (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL      *This,
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *BltBuffer, OPTIONAL
  IN  EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
  IN  UINTN                             SourceX,
  IN  UINTN                             SourceY,
  IN  UINTN                             DestinationX,
  IN  UINTN                             DestinationY,
  IN  UINTN                             Width,
  IN  UINTN                             Height,
  IN  UINTN                             Delta         OPTIONAL
  )
{
  RETURN_STATUS                         Status;
  EFI_TPL                               Tpl;
  GRAPHICS_OUTPUT_PRIVATE_DATA          *Private;

  Private = GRAPHICS_OUTPUT_PRIVATE_FROM_THIS (This);
  //
  // We have to raise to TPL_NOTIFY, so we make an atomic write to the frame buffer.
  // We would not want a timer based event (Cursor, ...) to come in while we are
  // doing this operation.
  //
  Tpl = gBS->RaiseTPL (TPL_NOTIFY);
  Status = FrameBufferBlt (
             Private->FrameBufferBltLibConfigure,
             BltBuffer,
             BltOperation,
             SourceX, SourceY,
             DestinationX, DestinationY, Width, Height,
             Delta
             );
  gBS->RestoreTPL (Tpl);

  if (!RETURN_ERROR(Status)) {
    switch (BltOperation) {
    case EfiBltVideoToVideo:
    case EfiBltVideoFill:
    case EfiBltBufferToVideo:
      fbcon_flush();
      break;

    case EfiBltVideoToBltBuffer:
    default:
      break;
    }
  }

  return RETURN_ERROR (Status) ? EFI_INVALID_PARAMETER : EFI_SUCCESS;
}

CONST GRAPHICS_OUTPUT_PRIVATE_DATA mGraphicsOutputInstanceTemplate = {
  GRAPHICS_OUTPUT_PRIVATE_DATA_SIGNATURE,          // Signature
  NULL,                                            // GraphicsOutputHandle
  {                                                // ModeInfo
    0,                                             // Version
    0,                                             // HorizontalResolution
    0,                                             // VerticalResolution
    PixelBltOnly,                                  // PixelFormat
    { 0 },                                         // PixelInformation
    0,                                             // PixelsPerScanLine
  },
  {
    GraphicsOutputQueryMode,
    GraphicsOutputSetMode,
    GraphicsOutputBlt,
    NULL                                           // Mode
  },
  {
    1,                                             // MaxMode
    0,                                             // Mode
    NULL,                                          // Info
    sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), // SizeOfInfo
    0,                                             // FrameBufferBase
    0                                              // FrameBufferSize
  },
  { // DevicePath
    {
      {
        HARDWARE_DEVICE_PATH, HW_VENDOR_DP,
        { (UINT8) (sizeof(VENDOR_DEVICE_PATH)), (UINT8) ((sizeof(VENDOR_DEVICE_PATH)) >> 8) },
      },
      // Hardware Device Path for Lcd
      EFI_CALLER_ID_GUID // Use the driver's GUID
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      { sizeof(EFI_DEVICE_PATH_PROTOCOL), 0 }
    }
  },
  NULL,                                            // FrameBufferBltLibConfigure
  0                                                // FrameBufferBltLibConfigureSize
};

EFI_STATUS
EFIAPI
InitializeGraphicsOutput (
  IN EFI_HANDLE                        ImageHandle,
  IN EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                        Status;
  RETURN_STATUS                     ReturnStatus;
  GRAPHICS_OUTPUT_PRIVATE_DATA      *Private;
  EFI_PHYSICAL_ADDRESS              FrameBufferBase;
  UINTN                             FrameBufferSize;
  struct fbcon_config               *FbConfig;
  EFI_CPU_ARCH_PROTOCOL             *Cpu;

  // Ensure the Cpu architectural protocol is already installed
  Status = gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID **)&Cpu);
  ASSERT_EFI_ERROR(Status);

  // initialize display
  target_display_init();

  // get framebuffer information
  FbConfig = fbcon_display();
  if (FbConfig==NULL || FbConfig->base==NULL || FbConfig->width==0 || FbConfig->height==0) {
    DEBUG((EFI_D_ERROR, "Invalid FbConfig: base=%p size=%ux%ux%u\n", FbConfig->base, FbConfig->width, FbConfig->height, FbConfig->bpp));
    return EFI_DEVICE_ERROR;
  }
  FrameBufferBase = (UINTN) FbConfig->base;
  FrameBufferSize = ROUNDUP(FbConfig->width * FbConfig->height * (FbConfig->bpp/8), EFI_PAGE_SIZE);

  // set framebuffer cachability
  Status = Cpu->SetMemoryAttributes (Cpu, FrameBufferBase, FrameBufferSize, PcdGet64 (PcdGraphicsOutputFbCacheability));
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    gBS->FreePool ((VOID*)(UINTN)FrameBufferBase);
    return Status;
  }

  Private = AllocateCopyPool (sizeof (mGraphicsOutputInstanceTemplate), &mGraphicsOutputInstanceTemplate);
  if (Private == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto EarlyReturnStatus;
  }

  Private->ModeInfo.Version = 0;
  Private->ModeInfo.HorizontalResolution = FbConfig->width;
  Private->ModeInfo.VerticalResolution = FbConfig->height;
  Private->ModeInfo.PixelFormat = PixelBitMask;
  Private->ModeInfo.PixelInformation.RedMask = PIXEL24_RED_MASK;
  Private->ModeInfo.PixelInformation.GreenMask = PIXEL24_GREEN_MASK;
  Private->ModeInfo.PixelInformation.BlueMask = PIXEL24_BLUE_MASK;
  Private->ModeInfo.PixelsPerScanLine = Private->ModeInfo.HorizontalResolution;

  Private->GraphicsOutputMode.FrameBufferBase = FrameBufferBase;
  Private->GraphicsOutputMode.FrameBufferSize = FrameBufferSize;
  Private->GraphicsOutputMode.Info = &Private->ModeInfo;

  //
  // Fix up Mode pointer in GraphicsOutput
  //
  Private->GraphicsOutput.Mode = &Private->GraphicsOutputMode;

  //
  // Create the FrameBufferBltLib configuration.
  //
  ReturnStatus = FrameBufferBltConfigure (
                   (VOID *) (UINTN) Private->GraphicsOutput.Mode->FrameBufferBase,
                   Private->GraphicsOutput.Mode->Info,
                   Private->FrameBufferBltLibConfigure,
                   &Private->FrameBufferBltLibConfigureSize
                   );
  if (ReturnStatus == RETURN_BUFFER_TOO_SMALL) {
    Private->FrameBufferBltLibConfigure = AllocatePool (Private->FrameBufferBltLibConfigureSize);
    if (Private->FrameBufferBltLibConfigure != NULL) {
      ReturnStatus = FrameBufferBltConfigure (
                       (VOID *) (UINTN) Private->GraphicsOutput.Mode->FrameBufferBase,
                       Private->GraphicsOutput.Mode->Info,
                       Private->FrameBufferBltLibConfigure,
                       &Private->FrameBufferBltLibConfigureSize
                       );
    }
  }
  if (RETURN_ERROR (ReturnStatus)) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeMemory;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Private->GraphicsOutputHandle,
                  &gEfiGraphicsOutputProtocolGuid, &Private->GraphicsOutput,
                  &gEfiDevicePathProtocolGuid, &Private->DevicePath,
                  NULL
                  );

FreeMemory:
  if (EFI_ERROR (Status)) {
    if (Private != NULL) {
      if (Private->FrameBufferBltLibConfigure != NULL) {
        FreePool (Private->FrameBufferBltLibConfigure);
      }
      FreePool (Private);
    }
  }
  else {
    Status = PcdSet32S (PcdVideoHorizontalResolution, Private->ModeInfo.HorizontalResolution);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet32S (PcdVideoVerticalResolution, Private->ModeInfo.VerticalResolution);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet32S (PcdSetupVideoHorizontalResolution, Private->ModeInfo.HorizontalResolution);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet32S (PcdSetupVideoVerticalResolution, Private->ModeInfo.VerticalResolution);
    ASSERT_EFI_ERROR (Status);
  }

EarlyReturnStatus:
  return Status;
}
