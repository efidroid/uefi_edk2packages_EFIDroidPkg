#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Chipset/board.h>
#include <Chipset/baseband.h>
#include <Chipset/smem.h>
#include <Target/board.h>
#include <Library/QcomTargetBoardLib.h>

/* Detect the target type */
void target_detect(struct board_data *board)
{
	uint32_t platform;
	uint32_t platform_hw;
	uint32_t target_id;

	platform = board->platform;
	platform_hw = board->platform_hw;

	/* Detect the board we are running on */
	if ((platform == MSM8960) || (platform == MSM8960AB) ||
		(platform == APQ8060AB) || (platform == MSM8260AB) ||
		(platform == MSM8660AB) ||(platform == MSM8660A) ||
		(platform == MSM8260A) || (platform == APQ8060A)) {
		switch (platform_hw) {
		case HW_PLATFORM_SURF:
			target_id = LINUX_MACHTYPE_8960_CDP;
			break;
		case HW_PLATFORM_MTP:
			target_id = LINUX_MACHTYPE_8960_MTP;
			break;
		case HW_PLATFORM_FLUID:
			target_id = LINUX_MACHTYPE_8960_FLUID;
			break;
		case HW_PLATFORM_LIQUID:
			target_id = LINUX_MACHTYPE_8960_LIQUID;
			break;
		default:
			target_id = LINUX_MACHTYPE_8960_CDP;
		}
	} else if ((platform == MSM8130)           ||
			   (platform == MSM8130AA) || (platform == MSM8130AB) ||
			   (platform == MSM8230)   || (platform == MSM8630)   ||
			   (platform == MSM8930)   || (platform == MSM8230AA) ||
			   (platform == MSM8630AA) || (platform == MSM8930AA) ||
			   (platform == MSM8930AB) || (platform == MSM8630AB) ||
			   (platform == MSM8230AB) || (platform == APQ8030AB) ||
			   (platform == APQ8030) || platform == APQ8030AA) {
		switch (platform_hw) {
		case HW_PLATFORM_SURF:
			target_id = LINUX_MACHTYPE_8930_CDP;
			break;
		case HW_PLATFORM_MTP:
			target_id = LINUX_MACHTYPE_8930_MTP;
			break;
		case HW_PLATFORM_FLUID:
			target_id = LINUX_MACHTYPE_8930_FLUID;
			break;
		case HW_PLATFORM_QRD:
			target_id = LINUX_MACHTYPE_8930_EVT;
			break;
		default:
			target_id = LINUX_MACHTYPE_8930_CDP;
		}
	} else if ((platform == MSM8227) || (platform == MSM8627) ||
			   (platform == MSM8227AA) || (platform == MSM8627AA)) {
		switch (platform_hw) {
		case HW_PLATFORM_SURF:
			target_id = LINUX_MACHTYPE_8627_CDP;
			break;
		case HW_PLATFORM_MTP:
			target_id = LINUX_MACHTYPE_8627_MTP;
			break;
		default:
			target_id = LINUX_MACHTYPE_8627_CDP;
		}
	} else if (platform == MPQ8064) {
		switch (platform_hw) {
		case HW_PLATFORM_SURF:
			target_id = LINUX_MACHTYPE_8064_MPQ_CDP;
			break;
		case HW_PLATFORM_HRD:
			target_id = LINUX_MACHTYPE_8064_MPQ_HRD;
			break;
		case HW_PLATFORM_DTV:
			target_id = LINUX_MACHTYPE_8064_MPQ_DTV;
			break;
		default:
			target_id = LINUX_MACHTYPE_8064_MPQ_CDP;
		}
	} else if ((platform == APQ8064) || (platform == APQ8064AA)
					 || (platform == APQ8064AB)) {
		switch (platform_hw) {
		case HW_PLATFORM_SURF:
			target_id = LINUX_MACHTYPE_8064_CDP;
			break;
		case HW_PLATFORM_MTP:
			target_id = LINUX_MACHTYPE_8064_MTP;
			break;
		case HW_PLATFORM_LIQUID:
			target_id = LINUX_MACHTYPE_8064_LIQUID;
			break;
		default:
			target_id = LINUX_MACHTYPE_8064_CDP;
		}
	} else {
		dprintf(CRITICAL, "platform (%d) is not identified.\n",
			platform);
		ASSERT(0);
	}
	board->target = target_id;
}

/* Detect the modem type */
void target_baseband_detect(struct board_data *board)
{
	uint32_t baseband;
	uint32_t platform;
	uint32_t platform_subtype;

	platform         = board->platform;
	platform_subtype = board->platform_subtype;

	/* Check for baseband variants. Default to MSM */
	if (platform_subtype == HW_PLATFORM_SUBTYPE_MDM)
		baseband = BASEBAND_MDM;
	else if (platform_subtype == HW_PLATFORM_SUBTYPE_SGLTE)
		baseband = BASEBAND_SGLTE;
	else if (platform_subtype == HW_PLATFORM_SUBTYPE_DSDA)
		baseband = BASEBAND_DSDA;
	else if (platform_subtype == HW_PLATFORM_SUBTYPE_DSDA2)
		baseband = BASEBAND_DSDA2;
	else if (platform_subtype == HW_PLATFORM_SUBTYPE_SGLTE2)
		baseband = BASEBAND_SGLTE2;
	else {
		switch(platform) {
		case APQ8060:
		case APQ8064:
		case APQ8064AA:
		case APQ8064AB:
		case APQ8030AB:
		case MPQ8064:
		case APQ8030:
		case APQ8030AA:
			baseband = BASEBAND_APQ;
			break;
		default:
			baseband = BASEBAND_MSM;
		};
	}
	board->baseband = baseband;
}
