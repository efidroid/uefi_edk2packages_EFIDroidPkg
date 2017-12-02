#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MallocLib.h>

typedef enum {
  ALLOCTYPE_MALLOC,
  ALLOCTYPE_MEMALIGN,
} ALLOCATION_TYPE;

typedef struct {
  LIST_ENTRY            Link;
  ALLOCATION_TYPE       Type;
  VOID                  *Memory;
  UINTN                 Size;

  union {
    struct {
      UINTN Boundary;
      UINTN Pages;
    } memalign;
  } u;
} ALLOCATION_ENTRY;

STATIC LIST_ENTRY mAllocations = INITIALIZE_LIST_HEAD_VARIABLE(mAllocations);

VOID *malloc(UINTN size)
{
  // allocate memory
  VOID *Memory = AllocatePool (size);
  if (Memory==NULL)
    return NULL;

  // allocate node structure
  ALLOCATION_ENTRY *NewNode = AllocatePool (sizeof (ALLOCATION_ENTRY));
  if (NewNode == NULL) {
    ASSERT (FALSE);
    goto FreePages;
  }

  NewNode->Type       = ALLOCTYPE_MALLOC;
  NewNode->Memory     = Memory;
  NewNode->Size       = size;

  InsertTailList (&mAllocations, &NewNode->Link);

  return Memory;

FreePages:
  FreePool (Memory);
  return NULL;
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

  NewNode->Type       = ALLOCTYPE_MEMALIGN;
  NewNode->Memory     = Memory;
  NewNode->Size       = size;
  NewNode->u.memalign.Boundary = boundary;
  NewNode->u.memalign.Pages = Pages;

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
      if (Node->Type == ALLOCTYPE_MALLOC) {
        FreePool (Node->Memory);
      }
      else if (Node->Type == ALLOCTYPE_MEMALIGN) {
        FreeAlignedPages (Node->Memory, Node->u.memalign.Pages);
      }
      else {
        ASSERT(FALSE);
      }
      RemoveEntryList (&Node->Link);
      FreePool (Node);
      return;
    }
    else if (Memory >= Node->Memory && Memory < Node->Memory+Node->Size) {
      DEBUG((DEBUG_ERROR, "tried to free from the middle\n"));
    }
  }

  // we don't know about this pointer
  DEBUG((DEBUG_ERROR, "cant free %p\n", Memory));
  //ASSERT(FALSE);
}

VOID *realloc(VOID *Memory, UINTN size) {
  LIST_ENTRY        *Link;
  ALLOCATION_ENTRY  *Node;
  VOID *NewMem = NULL;

  if (Memory && !size) {
    free(Memory);
    return NULL;
  }

  if (!Memory)
    return malloc(size);

  for (Link = mAllocations.ForwardLink; Link != &mAllocations; Link = Link->ForwardLink) {
    Node = BASE_CR (Link, ALLOCATION_ENTRY, Link);
    if (Node->Memory == Memory) {
      if (Node->Type == ALLOCTYPE_MALLOC) {
        NewMem = malloc(size);
      }
      else if (Node->Type == ALLOCTYPE_MEMALIGN) {
        NewMem = memalign(Node->u.memalign.Boundary, size);
      }
      else {
        ASSERT(FALSE);
      }

      break;
    }
  }

  // error
  if (!NewMem)
    return NULL;

  ASSERT (Node);
  CopyMem(NewMem, Memory, MIN(size, Node->Size));
  free(Memory);

  return NewMem;
}
