#include <PiDxe.h>
#include <Library/LKEnvLib.h>
#include <Chipset/gsbi.h>
#include <Chipset/smem.h>
#include <Library/QcomClockLib.h>
#include <Library/QcomBoardLib.h>
#include <Library/QcomGpioTlmmLib.h>

/* Configure UART clock - based on the gsbi id */
VOID LibQcomPlatformUartDmClockConfig(UINT8 id)
{
  CHAR8 gsbi_uart_clk_id[64];
  CHAR8 gsbi_p_clk_id[64];

  snprintf(gsbi_uart_clk_id, 64,"gsbi%u_uart_clk", id);
  gClock->clk_get_set_enable(gsbi_uart_clk_id, 1843200, 1);

  snprintf(gsbi_p_clk_id, 64,"gsbi%u_pclk", id);
  gClock->clk_get_set_enable(gsbi_p_clk_id, 0, 1);
}

/* Configure gpio for uart - based on gsbi id */
VOID LibQcomPlatformUartDmGpioConfig(UINT8 id)
{
	if(gBoard->board_platform_id() == MPQ8064)
	{
		switch (id) {

		case GSBI_ID_5:
			/* configure rx gpio */
			gGpioTlmm->SetFunction(52, 2);
			gGpioTlmm->SetDriveStrength(52, 8);
			gGpioTlmm->SetPull(52, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(52);

			/* configure tx gpio */
			gGpioTlmm->SetFunction(51, 2);
			gGpioTlmm->SetDriveStrength(51, 8);
			gGpioTlmm->SetPull(51, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(51);
			break;

		default:
			ASSERT(0);
		}
	}
	else if((gBoard->board_platform_id() == APQ8064) ||
			(gBoard->board_platform_id() == APQ8064AA) ||
			(gBoard->board_platform_id() == APQ8064AB))
	{
		switch (id) {

		case GSBI_ID_1:
			/* configure rx gpio */
			gGpioTlmm->SetFunction(19, 1);
			gGpioTlmm->SetDriveStrength(19, 8);
			gGpioTlmm->SetPull(19, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(19);

			/* configure tx gpio */
			gGpioTlmm->SetFunction(18, 1);
			gGpioTlmm->SetDriveStrength(18, 8);
			gGpioTlmm->SetPull(18, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(18);
			break;


		case GSBI_ID_7:
			/* configure rx gpio */
			gGpioTlmm->SetFunction(83, 1);
			gGpioTlmm->SetDriveStrength(83, 8);
			gGpioTlmm->SetPull(83, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(83);

			/* configure tx gpio */
			gGpioTlmm->SetFunction(82, 2);
			gGpioTlmm->SetDriveStrength(82, 8);
			gGpioTlmm->SetPull(82, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(82);
			break;

		default:
			ASSERT(0);
		}
	}
	else
	{
		switch (id) {

		case GSBI_ID_3:
			/* configure rx gpio */
			gGpioTlmm->SetFunction(15, 1);
			gGpioTlmm->SetDriveStrength(15, 8);
			gGpioTlmm->SetPull(15, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(15);

			/* configure tx gpio */
			gGpioTlmm->SetFunction(14, 1);
			gGpioTlmm->SetDriveStrength(14, 8);
			gGpioTlmm->SetPull(14, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(14);
			break;

		case GSBI_ID_5:
			/* configure rx gpio */
			gGpioTlmm->SetFunction(23, 1);
			gGpioTlmm->SetDriveStrength(23, 8);
			gGpioTlmm->SetPull(23, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(23);

			/* configure tx gpio */
			gGpioTlmm->SetFunction(22, 1);
			gGpioTlmm->SetDriveStrength(22, 8);
			gGpioTlmm->SetPull(22, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(22);
			break;

		case GSBI_ID_8:
			/* configure rx gpio */
			gGpioTlmm->SetFunction(35, 1);
			gGpioTlmm->SetDriveStrength(35, 8);
			gGpioTlmm->SetPull(35, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(35);

			/* configure tx gpio */
			gGpioTlmm->SetFunction(34, 1);
			gGpioTlmm->SetDriveStrength(34, 8);
			gGpioTlmm->SetPull(34, GPIO_PULL_NONE);
			gGpioTlmm->DirectionInput(34);
			break;

		default:
			ASSERT(0);
		}
	}
}
