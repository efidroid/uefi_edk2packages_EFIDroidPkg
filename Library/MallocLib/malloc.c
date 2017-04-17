#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MallocLib.h>

typedef struct {
  VOID                  *Memory;
  UINTN                 Pages;
  LIST_ENTRY            Link;
} ALLOCATION_ENTRY;

STATIC LIST_ENTRY mAllocations = INITIALIZE_LIST_HEAD_VARIABLE(mAllocations);

VOID *malloc(UINTN size)
{
  return AllocatePool (size);
}

VOID *memalign(UINTN boundary, UINTN size)
{
  // allocate memory
  UINTN Pages = EFI_SIZE_TO_PAGES(size);
  VOID *Memory = AllocateAlignedPages(Pages, boundary);
  if (Memory==NULL)
    return NULL;

  // allocate node structure
  ALLOCATION_ENTRY *NewNode = AllocatePool (sizeof (ALLOCATION_ENTRY));
  if (NewNode == NULL) {
    ASSERT (FALSE);
    goto FreePages;
  }

  NewNode->Memory     = Memory;
  NewNode->Pages      = Pages;

  InsertTailList (&mAllocations, &NewNode->Link);

  return Memory;

FreePages:
  FreeAlignedPages (Memory, Pages);
  return NULL;
}

VOID *calloc(UINTN count, UINTN size)
{
  return AllocateZeroPool (count * size);
}

VOID free(VOID *Memory)
{
  LIST_ENTRY        *Link;
  ALLOCATION_ENTRY  *Node;

  if (Memory == NULL) {
    return;
  }

  // free aligned allocation
  for (Link = mAllocations.ForwardLink; Link != &mAllocations; Link = Link->ForwardLink) {
    Node = BASE_CR (Link, ALLOCATION_ENTRY, Link);
    if (Node->Memory == Memory) {
      FreeAlignedPages (Node->Memory, Node->Pages);
      RemoveEntryList (&Node->Link);
      FreePool (Node);
      return;
    }
  }

  // we don't know about this pointer, so assume this was a pool allocation
  FreePool (Memory);
}
