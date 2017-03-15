/** @file
*
*  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
*  Copyright (c) 2014, Linaro Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <PiPei.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <libfdt.h>

#include <Guid/FdtHob.h>

EFI_STATUS
EFIAPI
PlatformPeim (
  VOID
  )
{
  VOID               *Base;
  VOID               *NewBase;
  UINTN              FdtSize;
  UINTN              FdtPages;
  UINT64             *FdtHobData;

  Base = (VOID*)(UINTN)PcdGet64 (PcdDeviceTreeInitialBaseAddress);
  ASSERT (Base != NULL);

  if (fdt_check_header (Base) == 0) {
    FdtSize = fdt_totalsize (Base) + PcdGet32 (PcdDeviceTreeAllocationPadding);
    FdtPages = EFI_SIZE_TO_PAGES (FdtSize);
    NewBase = AllocatePages (FdtPages);
    ASSERT (NewBase != NULL);
    fdt_open_into (Base, NewBase, EFI_PAGES_TO_SIZE (FdtPages));

    FdtHobData = BuildGuidHob (&gFdtHobGuid, sizeof *FdtHobData);
    ASSERT (FdtHobData != NULL);
    *FdtHobData = (UINTN)NewBase;
  }

  BuildFvHob (PcdGet64 (PcdFvBaseAddress), PcdGet32 (PcdFvSize));

  return EFI_SUCCESS;
}
