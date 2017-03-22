#include <Library/LKEnvLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/QcomBoardLib.h>
#include <Chipset/smem.h>

void apq8064_ext_3p3V_enable(void)
{
	/* Configure GPIO for output */
	gGpioTlmm->gpio_tlmm_config(77, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_8MA, GPIO_ENABLE);

	/* Output High */
	gGpioTlmm->gpio_set(77, 2);
}

/* Returns 1 if target supports continuous splash screen. */
int target_cont_splash_screen(void)
{
	switch(gBoard->board_platform_id())
	{
	case MSM8960:
	case MSM8960AB:
	case APQ8060AB:
	case MSM8260AB:
	case MSM8660AB:
		return 1;

	default:
		return 0;
	}
}
