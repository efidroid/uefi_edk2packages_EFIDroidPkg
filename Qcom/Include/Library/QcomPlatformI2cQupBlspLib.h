#ifndef __LIBRARY_QCOM_PLATFORM_I2C_QUP_BLSP_LIB_H__
#define __LIBRARY_QCOM_PLATFORM_I2C_QUP_BLSP_LIB_H__

typedef EFI_STATUS (EFIAPI *REGISTER_BLSP_I2C_DEVICE)(UINTN device_id, UINT8 blsp_id, UINT8 qup_id, UINTN clk_freq, UINTN src_clk_freq);

UINTN
EFIAPI
LibQcomPlatformI2cQupGetBlspQupIrq (
  UINT8 BlspId,
  UINT8 QupId
  );

UINTN
EFIAPI
LibQcomPlatformI2cQupGetBlspQupBase (
  UINT8 BlspId,
  UINT8 QupId
  );

VOID
EFIAPI
LibQcomPlatformI2cQupBlspGpioConfig (
  UINT8 BlspId,
  UINT8 QupId
  );

VOID
EFIAPI
LibQcomPlatformI2cQupBlspClockConfig (
  UINT8 BlspId,
  UINT8 QupId
  );

EFI_STATUS
EFIAPI
LibQcomPlatformI2cQupAddBlspBusses (
  REGISTER_BLSP_I2C_DEVICE RegisterI2cDevice
  );

#endif
