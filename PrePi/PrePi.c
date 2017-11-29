/** @file
*
*  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
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

#include <Library/PrePiLib.h>
#include <Library/PrintLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Library/PrePiHobListPointerLib.h>
#include <Library/TimerLib.h>
#include <Library/PerformanceLib.h>
#include <Library/CacheMaintenanceLib.h>

#include <Ppi/GuidedSectionExtraction.h>
#include <Ppi/ArmMpCoreInfo.h>

#include "PrePi.h"

#ifdef FVMAIN_COMPRESSION_METHOD_LZMA
#include <Guid/LzmaDecompress.h>
#include "LzmaDecompress.h"
#elif defined(FVMAIN_COMPRESSION_METHOD_BROTLI)
#include "BrotliDecompress.h"
#elif defined(FVMAIN_COMPRESSION_METHOD_GZIP)
#include "GzipDecompress.h"
#endif

EFI_STATUS
EFIAPI
ExtractGuidedSectionLibConstructor (
  VOID
  );

#ifdef FVMAIN_COMPRESSION_METHOD_LZMA
EFI_STATUS
EFIAPI
LzmaDecompressLibConstructor (
  VOID
  );
#elif defined(FVMAIN_COMPRESSION_METHOD_BROTLI)
EFI_STATUS
EFIAPI
BrotliDecompressLibConstructor (
  VOID
  );
#elif defined(FVMAIN_COMPRESSION_METHOD_GZIP)
EFI_STATUS
EFIAPI
GzipDecompressLibConstructor (
  VOID
  );
#endif

EFI_STATUS
GetPlatformPpi (
  IN  EFI_GUID  *PpiGuid,
  OUT VOID      **Ppi
  )
{
  UINTN                   PpiListSize;
  UINTN                   PpiListCount;
  EFI_PEI_PPI_DESCRIPTOR  *PpiList;
  UINTN                   Index;

  PpiListSize = 0;
  ArmPlatformGetPlatformPpiList (&PpiListSize, &PpiList);
  PpiListCount = PpiListSize / sizeof(EFI_PEI_PPI_DESCRIPTOR);
  for (Index = 0; Index < PpiListCount; Index++, PpiList++) {
    if (CompareGuid (PpiList->Guid, PpiGuid) == TRUE) {
      *Ppi = PpiList->Ppi;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

VOID
PrePiMain (
  IN  UINTN                     UefiMemoryBase,
  IN  UINTN                     StacksBase,
  IN  UINT64                    StartTimeStamp
  )
{
  EFI_HOB_HANDOFF_INFO_TABLE*   HobList;
  EFI_STATUS                    Status;
  CHAR8                         Buffer[100];
  UINTN                         CharCount;
  UINTN                         StacksSize;

  // Initialize the architecture specific bits
  ArchInitialize ();

  // Declare the PI/UEFI memory region
  HobList = HobConstructor (
    (VOID*)UefiMemoryBase,
    FixedPcdGet32 (PcdSystemMemoryUefiRegionSize),
    (VOID*)UefiMemoryBase,
    (VOID*)StacksBase  // The top of the UEFI Memory is reserved for the stacks
    );
  PrePeiSetHobList (HobList);

  // Initialize the Serial Port
  SerialPortInitialize ();
  CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"UEFI firmware (version %s built at %a on %a)\n\r",
    (CHAR16*)PcdGetPtr(PcdFirmwareVersionString), __TIME__, __DATE__);
  SerialPortWrite ((UINT8 *) Buffer, CharCount);

  //
  // Ensure that the loaded image is invalidated in the caches, so that any
  // modifications we made with the caches and MMU off (such as the applied
  // relocations) don't become invisible once we turn them on.
  //
  InvalidateDataCacheRange((VOID *)(UINTN)PcdGet64 (PcdFdBaseAddress), PcdGet32 (PcdFdSize));

  // Initialize MMU and Memory HOBs (Resource Descriptor HOBs)
  Status = MemoryPeim (UefiMemoryBase, FixedPcdGet32 (PcdSystemMemoryUefiRegionSize));
  ASSERT_EFI_ERROR (Status);

  // Create the Stacks HOB (reserve the memory for all stacks)
  StacksSize = PcdGet32 (PcdCPUCorePrimaryStackSize);
  BuildStackHob (StacksBase, StacksSize);

  //TODO: Call CpuPei as a library
  BuildCpuHob (PcdGet8 (PcdPrePiCpuMemorySize), PcdGet8 (PcdPrePiCpuIoSize));

  // Set the Boot Mode
  SetBootMode (ArmPlatformGetBootMode ());

  // Initialize Platform HOBs (CpuHob and FvHob)
  Status = PlatformPeim ();
  ASSERT_EFI_ERROR (Status);

  // Now, the HOB List has been initialized, we can register performance information
  PERF_START (NULL, "PEI", NULL, StartTimeStamp);

  // SEC phase needs to run library constructors by hand.
  ExtractGuidedSectionLibConstructor ();
#ifdef FVMAIN_COMPRESSION_METHOD_LZMA
  LzmaDecompressLibConstructor ();
#elif defined(FVMAIN_COMPRESSION_METHOD_BROTLI)
  BrotliDecompressLibConstructor ();
#elif defined(FVMAIN_COMPRESSION_METHOD_GZIP)
  GzipDecompressLibConstructor ();
#endif

  // Build HOBs to pass up our version of stuff the DXE Core needs to save space
  BuildPeCoffLoaderHob ();
  BuildExtractSectionHob (
#ifdef FVMAIN_COMPRESSION_METHOD_LZMA
    &gLzmaCustomDecompressGuid,
    LzmaGuidedSectionGetInfo,
    LzmaGuidedSectionExtraction
#elif defined(FVMAIN_COMPRESSION_METHOD_BROTLI)
    &gBrotliCustomDecompressGuid,
    BrotliGuidedSectionGetInfo,
    BrotliGuidedSectionExtraction
#elif defined(FVMAIN_COMPRESSION_METHOD_GZIP)
    &gGzipCustomDecompressGuid,
    GzipGuidedSectionGetInfo,
    GzipGuidedSectionExtraction
#endif
    );

  // Assume the FV that contains the SEC (our code) also contains a compressed FV.
  Status = DecompressFirstFv ();
  ASSERT_EFI_ERROR (Status);

  // Load the DXE Core and transfer control to it
  Status = LoadDxeCoreFromFv (NULL, 0);
  ASSERT_EFI_ERROR (Status);
}

VOID
CEntryPoint (
  IN  UINTN                     MpId,
  IN  UINTN                     UefiMemoryBase,
  IN  UINTN                     StacksBase
  )
{
  UINT64   StartTimeStamp;

  // Initialize the platform specific controllers
  ArmPlatformInitialize (MpId);

  // give the platform a chance to initialize the global counter
  TimerEarlyInit ();

  if (PerformanceMeasurementEnabled ()) {
    // Initialize the Timer Library to setup the Timer HW controller
    TimerConstructor ();
    // We cannot call yet the PerformanceLib because the HOB List has not been initialized
    StartTimeStamp = GetPerformanceCounter ();
  } else {
    StartTimeStamp = 0;
  }

  // Data Cache enabled on Primary core when MMU is enabled.
  ArmDisableDataCache ();
  // Invalidate instruction cache
  ArmInvalidateInstructionCache ();
  // Enable Instruction Caches on all cores.
  ArmEnableInstructionCache ();

  PrePiMain (UefiMemoryBase, StacksBase, StartTimeStamp);

  // DXE Core should always load and never return
  ASSERT (FALSE);
}
