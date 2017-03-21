#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomPlatformGpioTlmmLib.h>
#include <Library/QcomGpioTlmmSecLib.h>

#include "Protocol.c"

QCOM_GPIO_TLMM_PROTOCOL *gGpioTlmm = NULL;

RETURN_STATUS
EFIAPI
GpioTlmmSecLibConstructor (
  VOID
  )
{
  gGpioTlmm = &mGpioTlmm;
  return RETURN_SUCCESS;
}
