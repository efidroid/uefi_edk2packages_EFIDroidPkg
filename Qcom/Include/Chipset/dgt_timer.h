#ifndef __PLATFORM_MSM_SHARED_DGT_TIMER_H
#define __PLATFORM_MSM_SHARED_DGT_TIMER_H

#define DGT_REG(off)        (((UINTN)PcdGet64(PcdMsmDgtBase)) + (off))

#define DGT_MATCH_VAL        DGT_REG((UINTN)PcdGet64(PcdMsmDgtMatchValOffset))
#define DGT_COUNT_VAL        DGT_REG((UINTN)PcdGet64(PcdMsmDgtCountValOffset))
#define DGT_ENABLE           DGT_REG((UINTN)PcdGet64(PcdMsmDgtEnableOffset))
#define DGT_CLEAR            DGT_REG((UINTN)PcdGet64(PcdMsmDgtClearOffset))
#define DGT_CLK_CTL          DGT_REG((UINTN)PcdGet64(PcdMsmDgtClkCtlOffset))

#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

#define SPSS_TIMER_STATUS ((UINTN)PcdGet64(PcdSpssTimerStatusReg))
#define SPSS_TIMER_STATUS_DGT_EN    (1 << 0)

#endif
