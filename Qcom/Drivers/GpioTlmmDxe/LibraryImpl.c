#include <Base.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/QcomPlatformGpioTlmmLib.h>

#include "gpio_p.h"

QCOM_GPIO_TLMM_PROTOCOL *gGpioTlmm = NULL;

STATIC QCOM_GPIO_TLMM_PROTOCOL mInternalGpioTlmm = {
  gpio_tlmm_config,
  gpio_set,
  tlmm_set_hdrive_ctrl,
  tlmm_set_pull_ctrl,
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
