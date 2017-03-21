#ifndef __QCOM_PROTOCOL_BOARD_H__
#define __QCOM_PROTOCOL_BOARD_H__

#include <Chipset/board.h>

#define QCOM_BOARD_PROTOCOL_GUID \
  { 0x470d9ff7, 0x23d0, 0x4f25, { 0x91, 0xaf, 0xe4, 0x5f, 0x3f, 0x6f, 0xe6, 0xb4 } }

typedef struct _QCOM_BOARD_PROTOCOL   QCOM_BOARD_PROTOCOL;

typedef uint32_t (EFIAPI *board_platform_id_t)(void);
typedef uint32_t (EFIAPI *board_target_id_t)(void);
typedef uint32_t (EFIAPI *board_baseband_t)(void);
typedef uint32_t (EFIAPI *board_hardware_id_t)(void);
typedef uint32_t (EFIAPI *board_pmic_type_t)(void);
typedef uint32_t (EFIAPI *board_pmic_ver_t)(void);

struct _QCOM_BOARD_PROTOCOL {
  board_platform_id_t board_platform_id;
  board_target_id_t board_target_id;
  board_baseband_t board_baseband;
  board_hardware_id_t board_hardware_id;
  board_pmic_type_t board_pmic_type;
  board_pmic_ver_t board_pmic_ver;
};

extern EFI_GUID gQcomBoardProtocolGuid;

#endif
