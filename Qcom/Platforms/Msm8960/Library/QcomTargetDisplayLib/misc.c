#include <Library/LKEnvLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/QcomBoardLib.h>
#include <Chipset/smem.h>

void apq8064_ext_3p3V_enable(void)
{
	/* Configure GPIO for output */
	gGpioTlmm->SetFunction(77, 0);
	gGpioTlmm->SetDriveStrength(77, 8);
	gGpioTlmm->SetPull(77, GPIO_PULL_NONE);
	gGpioTlmm->DirectionOutput(77, 1);
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
