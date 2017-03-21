/*
 * Copyright (c) 2015, Linaro Ltd. All rights reserved.
 *
 * This program and the accompanying materials
 * are licensed and made available under the terms and conditions of the BSD License
 * which accompanies this distribution.  The full text of the license may be found at
 * http://opensource.org/licenses/bsd-license.php
 *
 * THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 * WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 */

#include <Uefi.h>
#include <Include/libfdt.h>
#include "atags.h"

#define ROUNDDOWN(a, b) ((a) & ~((b)-1))

BOOLEAN
FindMemnodeFdt (
  IN  VOID    *DeviceTreeBlob,
  OUT UINT64  *SystemMemoryBase,
  OUT UINT64  *SystemMemorySize
  )
{
  INT32         MemoryNode;
  INT32         AddressCells;
  INT32         SizeCells;
  INT32         Length;
  CONST INT32   *Prop;

  if (fdt_check_header (DeviceTreeBlob) != 0) {
    return FALSE;
  }

  //
  // Look for a node called "memory" at the lowest level of the tree
  //
  MemoryNode = fdt_path_offset (DeviceTreeBlob, "/memory");
  if (MemoryNode <= 0) {
    return FALSE;
  }

  //
  // Retrieve the #address-cells and #size-cells properties
  // from the root node, or use the default if not provided.
  //
  AddressCells = 1;
  SizeCells = 1;

  Prop = fdt_getprop (DeviceTreeBlob, 0, "#address-cells", &Length);
  if (Length == 4) {
    AddressCells = fdt32_to_cpu (*Prop);
  }

  Prop = fdt_getprop (DeviceTreeBlob, 0, "#size-cells", &Length);
  if (Length == 4) {
    SizeCells = fdt32_to_cpu (*Prop);
  }

  //
  // Now find the 'reg' property of the /memory node, and read the first
  // range listed.
  //
  Prop = fdt_getprop (DeviceTreeBlob, MemoryNode, "reg", &Length);

  if (Length < (AddressCells + SizeCells) * sizeof (INT32)) {
    return FALSE;
  }

  *SystemMemoryBase = fdt32_to_cpu (Prop[0]);
  if (AddressCells > 1) {
    *SystemMemoryBase = (*SystemMemoryBase << 32) | fdt32_to_cpu (Prop[1]);
  }
  Prop += AddressCells;

  *SystemMemorySize = fdt32_to_cpu (Prop[0]);
  if (SizeCells > 1) {
    *SystemMemorySize = (*SystemMemorySize << 32) | fdt32_to_cpu (Prop[1]);
  }

  return TRUE;
}

BOOLEAN
FindMemnodeAtags (
  IN  VOID    *Tags,
  OUT UINT64  *SystemMemoryBase,
  OUT UINT64  *SystemMemorySize
  )
{
  struct tag *ATags;
  struct tag *Tag;
  UINT64     MemStart;
  UINT64     MemEnd;

  ATags = (struct tag *) Tags;
  MemStart = 0xFFFFFFFFFFFFFFFF;
  MemEnd   = 0x0;

  if (ATags->hdr.tag != ATAG_CORE) {
    return FALSE;
  }

  //
  // Look for the highest available memory address
  //
  for (Tag = ATags; Tag->hdr.size; Tag = tag_next(Tag)) {
    if (Tag->hdr.tag == ATAG_MEM) {
      UINT64 End = ((UINT64)Tag->u.mem.start) + Tag->u.mem.size - 1;

      if (Tag->u.mem.start < MemStart)
        MemStart = Tag->u.mem.start;
      if (End > MemEnd)
        MemEnd = End;
    }
  }

  // this works around SMEM being removed from the regions
  MemStart = ROUNDDOWN(MemStart, 8*1024*1024);

  *SystemMemorySize = MemEnd - MemStart + 1;
  *SystemMemoryBase = MemStart;

  return TRUE;
}

BOOLEAN
FindMemnode (
  IN  VOID    *DeviceTreeBlob,
  OUT UINT64  *SystemMemoryBase,
  OUT UINT64  *SystemMemorySize
  )
{
  if (FindMemnodeFdt (DeviceTreeBlob, SystemMemoryBase, SystemMemorySize) == TRUE) {
    return TRUE;
  }

  if (FindMemnodeAtags (DeviceTreeBlob, SystemMemoryBase, SystemMemorySize) == TRUE) {
    return TRUE;
  }

  return FALSE;
}
