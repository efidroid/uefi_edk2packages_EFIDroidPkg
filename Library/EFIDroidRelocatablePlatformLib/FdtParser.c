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
#include <Library/ATagLib.h>

BOOLEAN
FindMemnodeFdt (
  IN  VOID    *DeviceTreeBlob,
  OUT UINT64  *SystemMemoryBase,
  OUT UINT64  *SystemMemorySize
  )
{
  INT32         MemoryNode;
  INT32         RootNode;
  INT32         ReservedMemoryNode;
  INT32         AddressCells;
  INT32         SizeCells;
  INT32         AddressCellsRoot;
  INT32         SizeCellsRoot;
  INT32         Length;
  UINTN         NumMemoryRanges;
  UINTN         Index;
  CONST INT32   *Prop;
  UINT64        HighestNoMap;

  if (fdt_check_header (DeviceTreeBlob) != 0) {
    return FALSE;
  }

  //
  // Look for the root node
  //
  RootNode = fdt_path_offset (DeviceTreeBlob, "/");
  if (RootNode < 0) {
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
  // from the memory or root node, or use the default if not provided.
  //
  AddressCells = 1;
  SizeCells = 1;
  AddressCellsRoot = -1;
  SizeCellsRoot = -1;

  Prop = fdt_getprop (DeviceTreeBlob, RootNode, "#address-cells", &Length);
  if (Length == 4) {
    AddressCells = fdt32_to_cpu (*Prop);
    AddressCellsRoot = AddressCells;
  }

  Prop = fdt_getprop (DeviceTreeBlob, RootNode, "#size-cells", &Length);
  if (Length == 4) {
    SizeCells = fdt32_to_cpu (*Prop);
    SizeCellsRoot = SizeCells;
  }

  Prop = fdt_getprop (DeviceTreeBlob, MemoryNode, "#address-cells", &Length);
  if (Length == 4) {
    AddressCells = fdt32_to_cpu (*Prop);
  }

  Prop = fdt_getprop (DeviceTreeBlob, MemoryNode, "#size-cells", &Length);
  if (Length == 4) {
    SizeCells = fdt32_to_cpu (*Prop);
  }

  //
  // Now find the 'reg' property of the /memory node, and use the highest
  // range listed.
  //
  Prop = fdt_getprop (DeviceTreeBlob, MemoryNode, "reg", &Length);

  if (Length % ((AddressCells + SizeCells) * sizeof (INT32))) {
    return FALSE;
  }
  NumMemoryRanges = Length / ((AddressCells + SizeCells) * sizeof (INT32));

  for (Index=0; Index<NumMemoryRanges; Index++) {
    UINT64 LocalBase;
    UINT64 LocalSize;

    LocalBase = fdt32_to_cpu (Prop[0]);
    if (AddressCells > 1) {
      LocalBase = (LocalBase << 32) | fdt32_to_cpu (Prop[1]);
    }
    Prop += AddressCells;

    LocalSize = fdt32_to_cpu (Prop[0]);
    if (SizeCells > 1) {
      LocalSize = (LocalSize << 32) | fdt32_to_cpu (Prop[1]);
    }
    Prop += SizeCells;

    if (Index==0 || (*SystemMemoryBase)<LocalBase) {
      *SystemMemoryBase = LocalBase;
      *SystemMemorySize = LocalSize;
    }
  }

  //
  // no-map and no-map-fixup
  //
  HighestNoMap = 0;
  AddressCells = (AddressCellsRoot == -1) ? 1 : AddressCellsRoot;
  SizeCells = (SizeCellsRoot == -1) ? 1 : SizeCellsRoot;

  ReservedMemoryNode = fdt_path_offset (DeviceTreeBlob, "/reserved-memory");
  if (ReservedMemoryNode > 0) {
    UINT64 LocalBase;
    UINT64 LocalSize;
    INT32 Offset;

    Prop = fdt_getprop (DeviceTreeBlob, ReservedMemoryNode, "#address-cells", &Length);
    if (Length == 4) {
      AddressCells = fdt32_to_cpu (*Prop);
    }

    Prop = fdt_getprop (DeviceTreeBlob, ReservedMemoryNode, "#size-cells", &Length);
    if (Length == 4) {
      SizeCells = fdt32_to_cpu (*Prop);
    }

    for (Offset = fdt_first_subnode(DeviceTreeBlob, ReservedMemoryNode); Offset>=0; Offset = fdt_next_subnode(DeviceTreeBlob, Offset)) {
      CONST INT32 *PropNoMap;
      CONST INT32 *PropNoMapFixup;

      PropNoMap = fdt_getprop (DeviceTreeBlob, Offset, "no-map", &Length);
      PropNoMapFixup = fdt_getprop (DeviceTreeBlob, Offset, "no-map-fixup", &Length);
      if ((PropNoMap == NULL) && (PropNoMapFixup == NULL)) {
        continue;
      }

      Prop = fdt_getprop (DeviceTreeBlob, Offset, "reg", &Length);
      if (Length != ((AddressCells + SizeCells) * sizeof (INT32))) {
        continue;
      }

      LocalBase = fdt32_to_cpu (Prop[0]);
      if (AddressCells > 1) {
        LocalBase = (LocalBase << 32) | fdt32_to_cpu (Prop[1]);
      }
      Prop += AddressCells;

      LocalSize = fdt32_to_cpu (Prop[0]);
      if (SizeCells > 1) {
        LocalSize = (LocalSize << 32) | fdt32_to_cpu (Prop[1]);
      }
      Prop += SizeCells;

      if (HighestNoMap < LocalBase + LocalSize) {
        HighestNoMap = LocalBase +LocalSize;
      }
    }
  }

  if (*SystemMemoryBase < HighestNoMap) {
    *SystemMemorySize -= HighestNoMap - *SystemMemoryBase;
    *SystemMemoryBase = HighestNoMap;
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
  struct tag *HighestTag;

  ATags = (struct tag *) Tags;
  HighestTag = NULL;

  if (ATags->hdr.tag != ATAG_CORE) {
    return FALSE;
  }

  //
  // Look for the highest available memory address
  //
  for (Tag = ATags; Tag->hdr.size; Tag = tag_next(Tag)) {
    if (Tag->hdr.tag == ATAG_MEM) {
      if (HighestTag == NULL || Tag->u.mem.start > HighestTag->u.mem.start)
        HighestTag = Tag;
    }
  }

  *SystemMemorySize = (UINT64)HighestTag->u.mem.size;
  *SystemMemoryBase = (UINT64)HighestTag->u.mem.start;

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
