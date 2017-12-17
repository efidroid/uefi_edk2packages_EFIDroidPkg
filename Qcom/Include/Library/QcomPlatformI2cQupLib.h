#ifndef __LIBRARY_QCOM_PLATFORM_I2C_QUP_LIB_H__
#define __LIBRARY_QCOM_PLATFORM_I2C_QUP_LIB_H__

typedef EFI_STATUS (EFIAPI *REGISTER_I2C_DEVICE)(UINT8 gsbi_id, UINTN clk_freq, UINTN src_clk_freq);

VOID
EFIAPI
LibQcomPlatformI2cQupClockConfig (
  UINT8 Id,
  UINTN Frequency
  );

VOID
EFIAPI
LibQcomPlatformI2cQupGpioConfig (
  UINT8 Id
  );

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiBase (
  UINT8 Id
  );

UINTN
EFIAPI
LibQcomPlatformI2cQupGetQupBase (
  UINT8 Id
  );

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiQupIrq (
  UINT8 Id
  );

EFI_STATUS
EFIAPI
LibQcomPlatformI2cQupAddBusses (
  REGISTER_I2C_DEVICE RegisterI2cDevice
  );

#endif
