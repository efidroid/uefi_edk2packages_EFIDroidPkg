#include <PiDxe.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomClockLib.h>
#include <Library/QcomGpioTlmmLib.h>

/* Configure UART clock based on the UART block id*/
VOID LibQcomPlatformUartDmClockConfig(UINT8 id)
{
  int ret;

  ret = gClock->clk_get_set_enable("uart2_iface_clk", 0, 1);
  if(ret)
  {
    dprintf(CRITICAL, "failed to set uart2_iface_clk ret = %d\n", ret);
    ASSERT(0);
  }

  ret = gClock->clk_get_set_enable("uart2_core_clk", 7372800, 1);
  if(ret)
  {
    dprintf(CRITICAL, "failed to set uart2_core_clk ret = %d\n", ret);
    ASSERT(0);
  }
}

/* Configure gpio for blsp uart 2 */
VOID LibQcomPlatformUartDmGpioConfig(UINT8 id)
{
  switch(id)
  {
  case 2:
    /* Configure GPIOs for BLSP1 UART2. */
    /* configure rx gpio */
    gGpioTlmm->gpio_tlmm_config(5, 2, GPIO_INPUT, GPIO_NO_PULL,
                         GPIO_8MA, GPIO_DISABLE);

    /* configure tx gpio */
    gGpioTlmm->gpio_tlmm_config(4, 2, GPIO_OUTPUT, GPIO_NO_PULL,
                         GPIO_8MA, GPIO_DISABLE);
    break;
  default:
    dprintf(CRITICAL, "No gpio config found for uart id = %d\n", id);
  }
}
