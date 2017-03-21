#ifndef _SMEM_PRIVATE_H
#define _SMEM_PRIVATE_H

#include <Protocol/QcomSmem.h>

extern QCOM_SMEM_PROTOCOL *gSMEM;

void board_init(void);
uint32_t board_platform_id(void);
uint32_t board_target_id(void);
uint32_t board_baseband(void);
uint32_t board_hardware_id(void);
uint32_t board_pmic_type(void);
uint32_t board_pmic_ver(void);

#endif // _SMEM_PRIVATE_H
