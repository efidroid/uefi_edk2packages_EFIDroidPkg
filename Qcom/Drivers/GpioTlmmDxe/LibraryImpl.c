#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/QcomPlatformGpioTlmmLib.h>

QCOM_GPIO_TLMM_PROTOCOL *gGpioTlmm = NULL;

STATIC QCOM_GPIO_TLMM_PROTOCOL mInternalGpioTlmm = {
  gpio_tlmm_config,
  gpio_set,
};

RETURN_STATUS
EFIAPI
GpioTlmmImplLibInitialize (
  VOID
  )
{
  gGpioTlmm = &mInternalGpioTlmm;

  return RETURN_SUCCESS;
}
