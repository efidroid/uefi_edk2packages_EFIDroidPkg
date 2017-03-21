#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomSmemSecLib.h>

#include "smem_p.h"
#include "Protocol.c"

QCOM_SMEM_PROTOCOL *gSMEM = NULL;

RETURN_STATUS
EFIAPI
SmemSecLibConstructor (
  VOID
  )
{
  gSMEM = &mSMEM;
  return RETURN_SUCCESS;
}
