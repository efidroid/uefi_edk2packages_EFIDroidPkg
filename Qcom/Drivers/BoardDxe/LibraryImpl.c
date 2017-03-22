#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomBoardLib.h>

#include "board_p.h"

QCOM_BOARD_PROTOCOL *gBoard = NULL;

STATIC QCOM_BOARD_PROTOCOL mInternalBoard = {
  board_platform_id,
  board_target_id,
  board_baseband,
  board_hardware_id,
  board_pmic_type,
  board_pmic_ver,
};

RETURN_STATUS
EFIAPI
BoardImplLibInitialize (
  VOID
  )
{
  gBoard = &mInternalBoard;

  board_init();

  return RETURN_SUCCESS;
}
