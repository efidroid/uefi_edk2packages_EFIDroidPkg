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
#include <Library/ATagLib.h>
#include <libfdt.h>

#include <Guid/FdtHob.h>

STATIC
EFI_STATUS
EFIAPI
BuildFdtHob (
  VOID *Base
  )
{
  VOID               *NewBase;
  UINTN              FdtSize;
  UINTN              FdtPages;
  UINT64             *FdtHobData;

  if (fdt_check_header (Base) != 0) {
    return EFI_UNSUPPORTED;
  }

  FdtSize = fdt_totalsize (Base) + PcdGet32 (PcdDeviceTreeAllocationPadding);
  FdtPages = EFI_SIZE_TO_PAGES (FdtSize);
  NewBase = AllocatePages (FdtPages);
  ASSERT (NewBase != NULL);
  fdt_open_into (Base, NewBase, EFI_PAGES_TO_SIZE (FdtPages));

  FdtHobData = BuildGuidHob (&gFdtHobGuid, sizeof *FdtHobData);
  ASSERT (FdtHobData != NULL);
  *FdtHobData = (UINTN)NewBase;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
BuildAtagsHob (
  VOID *Base
  )
{
  VOID               *NewBase;
  UINTN              TagsSize;
  UINTN              TagsPages;
  UINT64             *AtagsHobData;
  struct tag         *Tag;
  struct tag         *ATags;
  UINTN              FdtPages;
  UINTN              FdtSize;
  VOID               *FdtBase;
  UINT64             *FdtHobData;
  BOOLEAN            First;
  INTN               err;

  ATags = (struct tag*)Base;

  if (ATags->hdr.tag != ATAG_CORE) {
    return EFI_UNSUPPORTED;
  }

  // calculate tags size
  for (Tag = ATags; Tag->hdr.size; Tag = tag_next(Tag));
  Tag++;
  TagsSize = ((UINTN)Tag)-((UINTN)ATags);

  // allocate memory for tags copy
  TagsPages = EFI_SIZE_TO_PAGES (TagsSize);
  NewBase = AllocatePages (TagsPages);
  ASSERT (NewBase != NULL);
  CopyMem (NewBase, Base, EFI_PAGES_TO_SIZE (TagsPages));

  // create tags Hob
  AtagsHobData = BuildGuidHob (&gAtagsHobGuid, sizeof *AtagsHobData);
  ASSERT (AtagsHobData != NULL);
  *AtagsHobData = (UINTN)NewBase;

  // create new, empty fdt
  FdtSize = SIZE_8KB + PcdGet32 (PcdDeviceTreeAllocationPadding);
  FdtPages = EFI_SIZE_TO_PAGES (FdtSize);
  FdtBase = AllocatePages (FdtPages);
  ASSERT (FdtBase != NULL);
  err = fdt_create_empty_tree (FdtBase, EFI_PAGES_TO_SIZE (FdtPages));
  if (err) goto FDT_ERR;

  // create basic chosen node
  INTN off_chosen = fdt_add_subnode(FdtBase, 0, "chosen");
  ASSERT(off_chosen>0);
  err = fdt_setprop_u32(FdtBase, off_chosen, "#address-cells", 2);
  if (err) goto FDT_ERR;
  err = fdt_setprop_u32(FdtBase, off_chosen, "#size-cells", 2);
  if (err) goto FDT_ERR;
  err = fdt_setprop_string(FdtBase, off_chosen, "device_type", "memory");
  if (err) goto FDT_ERR;

  // insert memory information
  First = TRUE;
  for (Tag = ATags; Tag->hdr.size; Tag = tag_next(Tag)) {
    if (Tag->hdr.tag == ATAG_MEM) {
      if (First) {
        err = fdt_setprop_u64(FdtBase, off_chosen, "reg", Tag->u.mem.start);
        First = FALSE;
      }
      else
        err = fdt_appendprop_u64(FdtBase, off_chosen, "reg", Tag->u.mem.start);
      if (err) goto FDT_ERR;

      err = fdt_appendprop_u64(FdtBase, off_chosen, "reg", Tag->u.mem.size);
    }
  }

  // create fdt hob
  FdtHobData = BuildGuidHob (&gFdtHobGuid, sizeof *FdtHobData);
  ASSERT (FdtHobData != NULL);
  *FdtHobData = (UINTN)FdtBase;

  return EFI_SUCCESS;

FDT_ERR:
  DEBUG ((EFI_D_ERROR, "FDT: %a\n", fdt_strerror(err)));
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
PlatformPeim (
  VOID
  )
{
  EFI_STATUS         Status;
  VOID               *Base;

  Base = (VOID*)(UINTN)PcdGet64 (PcdDeviceTreeInitialBaseAddress);
  ASSERT (Base != NULL);

  Status = BuildFdtHob (Base);
  if (EFI_ERROR(Status)) {
    Status = BuildAtagsHob (Base);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "Can't identify tags type\n"));
    }
  }

  BuildFvHob (PcdGet64 (PcdFvBaseAddress), PcdGet32 (PcdFvSize));

  return EFI_SUCCESS;
}
