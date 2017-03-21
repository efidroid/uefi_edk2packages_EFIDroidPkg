#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomSmemSecLib.h>
#include <Library/QcomBoardSecLib.h>

#include "board_p.h"
#include "Protocol.c"

QCOM_BOARD_PROTOCOL *gBoard = NULL;

RETURN_STATUS
EFIAPI
BoardSecLibConstructor (
  VOID
  )
{
  gBoard = &mBoard;

  board_init();

  return RETURN_SUCCESS;
}
