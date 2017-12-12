/** @file
*
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

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include <libfdt.h>

// Number of Virtual Memory Map Descriptors
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS          20

EFI_PHYSICAL_ADDRESS
ArmGetPhysAddrTop (
  VOID
  );

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU
  on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR
                                    describing a Physical-to-Virtual Memory
                                    mapping. This array must be ended by a
                                    zero-filled entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  VOID                          *Fdt;
  INTN                          Index = 0;
  INTN                          IndexDram = -1;

  ASSERT (VirtualMemoryMap != NULL);

  VirtualMemoryTable = AllocatePages (
                         EFI_SIZE_TO_PAGES (
                           sizeof (ARM_MEMORY_REGION_DESCRIPTOR)
                           * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS
                           )
                         );

  if (VirtualMemoryTable == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Error: Failed AllocatePages()\n", __FUNCTION__));
    return;
  }

  // System DRAM
  VirtualMemoryTable[Index].PhysicalBase = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = PcdGet64 (PcdSystemMemorySize);
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  IndexDram = Index++;

  DEBUG ((EFI_D_INFO, "%a: Dumping System DRAM Memory Map:\n"
      "\tPhysicalBase: 0x%lX\n"
      "\tVirtualBase: 0x%lX\n"
      "\tLength: 0x%lX\n",
      __FUNCTION__,
      VirtualMemoryTable[IndexDram].PhysicalBase,
      VirtualMemoryTable[IndexDram].VirtualBase,
      VirtualMemoryTable[IndexDram].Length));

  // Peripheral space before DRAM
  if (VirtualMemoryTable[IndexDram].PhysicalBase != 0) {
    VirtualMemoryTable[Index].PhysicalBase = 0x0;
    VirtualMemoryTable[Index].VirtualBase  = 0x0;
    VirtualMemoryTable[Index].Length       = VirtualMemoryTable[IndexDram].PhysicalBase;
    VirtualMemoryTable[Index++].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  }

  // Peripheral space after DRAM
  VirtualMemoryTable[Index].PhysicalBase = VirtualMemoryTable[IndexDram].PhysicalBase + VirtualMemoryTable[IndexDram].Length;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = MIN (1ULL << FixedPcdGet8 (PcdPrePiCpuMemorySize),
                                         ArmGetPhysAddrTop ()) -
                                       VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index++].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // Remap the FD region as normal executable memory
  VirtualMemoryTable[Index].PhysicalBase = PcdGet64 (PcdFdBaseAddress);
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = FixedPcdGet32 (PcdFdSize);
  VirtualMemoryTable[Index++].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  // Remap 'remove-completely' regions as device memory to prevent the hypervisor from rebooting the device
  Fdt = (VOID*)(UINTN)PcdGet64 (PcdDeviceTreeInitialBaseAddress);
  if (Fdt != NULL && fdt_check_header (Fdt) == 0) {
    INT32         MemoryNode;
    INT32         Offset;
    INT32         AddressCells = 1;
    INT32         SizeCells = 1;
    INT32         Length;
    CONST INT32   *Prop;

    MemoryNode = fdt_path_offset (Fdt, "/memory");
    if (MemoryNode <= 0) {
      goto END_OF_TABLE;
    }

    Prop = fdt_getprop (Fdt, MemoryNode, "#address-cells", &Length);
    if (Length == 4) {
      AddressCells = fdt32_to_cpu (*Prop);
    }

    Prop = fdt_getprop (Fdt, MemoryNode, "#size-cells", &Length);
    if (Length == 4) {
      SizeCells = fdt32_to_cpu (*Prop);
    }

    for (Offset = fdt_first_subnode(Fdt, MemoryNode); Offset>=0; Offset = fdt_next_subnode(Fdt, Offset)) {
      UINT64 LocalBase;
      UINT64 LocalSize;

      Prop = fdt_getprop (Fdt, Offset, "linux,remove-completely", &Length);
      if (Prop == NULL) {
        continue;
      }

      Prop = fdt_getprop (Fdt, Offset, "reg", &Length);
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

      ASSERT(Index<MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS-1);
      VirtualMemoryTable[Index].PhysicalBase = LocalBase;
      VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
      VirtualMemoryTable[Index].Length       = LocalSize;
      VirtualMemoryTable[Index++].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
    }
  }

END_OF_TABLE:
  // End of Table
  ZeroMem (&VirtualMemoryTable[Index++], sizeof (ARM_MEMORY_REGION_DESCRIPTOR));

  *VirtualMemoryMap = VirtualMemoryTable;
}
