#ifndef __LIBRARY_QCOM_BOARD_SECLIB_H__
#define __LIBRARY_QCOM_BOARD_SECLIB_H__

#include <Protocol/QcomBoard.h>

RETURN_STATUS
EFIAPI
BoardSecLibConstructor (
  VOID
  );

extern QCOM_BOARD_PROTOCOL *gBoard;

#endif
