#ifndef __LIBRARY_QCOM_TARGET_MMC_LIB_H__
#define __LIBRARY_QCOM_TARGET_MMC_LIB_H__

#include <Chipset/mmc.h>

VOID LibQcomTargetMmcInit(MMC_PLATFORM_CALLBACK_API *Callback);
VOID LibQcomTargetMmcCaps(struct mmc_host *host);

#endif
