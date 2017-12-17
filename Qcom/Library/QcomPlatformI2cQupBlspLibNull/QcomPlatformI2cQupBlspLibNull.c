#include <PiDxe.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomPlatformI2cQupBlspLib.h>

UINTN
EFIAPI
LibQcomPlatformI2cQupGetBlspQupIrq (
  UINT8 BlspId,
  UINT8 QupId
  )
{
  ASSERT(FALSE);
  return 0;
}

UINTN
EFIAPI
LibQcomPlatformI2cQupGetBlspQupBase (
  UINT8 BlspId,
  UINT8 QupId
  )
{
  ASSERT(FALSE);
  return 0;
}

VOID
EFIAPI
LibQcomPlatformI2cQupBlspGpioConfig (
  UINT8 BlspId,
  UINT8 QupId
  )
{
  ASSERT(FALSE);
}

VOID
EFIAPI
LibQcomPlatformI2cQupBlspClockConfig (
  UINT8 BlspId,
  UINT8 QupId
  )
{
  ASSERT(FALSE);
}

EFI_STATUS
EFIAPI
LibQcomPlatformI2cQupAddBlspBusses (
  REGISTER_BLSP_I2C_DEVICE RegisterI2cDevice
  )
{
  return EFI_SUCCESS;
}

