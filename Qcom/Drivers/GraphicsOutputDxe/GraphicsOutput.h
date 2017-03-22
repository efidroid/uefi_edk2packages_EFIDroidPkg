/** @file
  Header file for a generic GOP driver.

Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/
#ifndef _GRAPHICS_OUTPUT_DXE_H_
#define _GRAPHICS_OUTPUT_DXE_H_
#include <PiDxe.h>
#include <Library/LKEnvLib.h>

#include <Protocol/DriverBinding.h>
#include <Protocol/DevicePath.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/Cpu.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/DevicePathLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/fbcon.h>
#include <Library/QcomTargetDisplayLib.h>

#define MAX_PCI_BAR  6

typedef struct {
  VENDOR_DEVICE_PATH            Guid;
  EFI_DEVICE_PATH_PROTOCOL      End;
} LCD_GRAPHICS_DEVICE_PATH;

typedef struct {
  UINT32                               Signature;
  EFI_HANDLE                           GraphicsOutputHandle;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION ModeInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL         GraphicsOutput;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE    GraphicsOutputMode;
  LCD_GRAPHICS_DEVICE_PATH             DevicePath;
  FRAME_BUFFER_CONFIGURE               *FrameBufferBltLibConfigure;
  UINTN                                FrameBufferBltLibConfigureSize;
} GRAPHICS_OUTPUT_PRIVATE_DATA;

#define GRAPHICS_OUTPUT_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('g', 'g', 'o', 'p')
#define GRAPHICS_OUTPUT_PRIVATE_FROM_THIS(a) \
  CR(a, GRAPHICS_OUTPUT_PRIVATE_DATA, GraphicsOutput, GRAPHICS_OUTPUT_PRIVATE_DATA_SIGNATURE)

#define PIXEL24_RED_MASK    0x00ff0000
#define PIXEL24_GREEN_MASK  0x0000ff00
#define PIXEL24_BLUE_MASK   0x000000ff

#endif
