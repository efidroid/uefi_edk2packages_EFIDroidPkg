#ifndef __LIBRARY_QCOM_PLATFORM_I2C_QUP_GSBI_LIB_H__
#define __LIBRARY_QCOM_PLATFORM_I2C_QUP_GSBI_LIB_H__

typedef EFI_STATUS (EFIAPI *REGISTER_GSBI_I2C_DEVICE)(UINTN device_id, UINT8 gsbi_id, UINTN clk_freq, UINTN src_clk_freq);

VOID
EFIAPI
LibQcomPlatformI2cQupGsbiClockConfig (
  UINT8 Id,
  UINTN Frequency
  );

VOID
EFIAPI
LibQcomPlatformI2cQupGsbiGpioConfig (
  UINT8 Id
  );

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiBase (
  UINT8 Id
  );

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiQupBase (
  UINT8 Id
  );

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiQupIrq (
  UINT8 Id
  );

EFI_STATUS
EFIAPI
LibQcomPlatformI2cQupAddGsbiBusses (
  REGISTER_GSBI_I2C_DEVICE RegisterI2cDevice
  );

#endif
