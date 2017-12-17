#include <PiDxe.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomPlatformI2cQupGsbiLib.h>

VOID
EFIAPI
LibQcomPlatformI2cQupGsbiClockConfig (
  UINT8 Id,
  UINTN Frequency
  )
{
  ASSERT(FALSE);
}

VOID
EFIAPI
LibQcomPlatformI2cQupGsbiGpioConfig (
  UINT8 Id
  )
{
  ASSERT(FALSE);
}

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiBase (
  UINT8 Id
  )
{
  ASSERT(FALSE);
  return 0;
}

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiQupBase (
  UINT8 Id
  )
{
  ASSERT(FALSE);
  return 0;
}

UINTN
EFIAPI
LibQcomPlatformI2cQupGetGsbiQupIrq (
  UINT8 Id
  )
{
  ASSERT(FALSE);
  return 0;
}

EFI_STATUS
EFIAPI
LibQcomPlatformI2cQupAddGsbiBusses (
  REGISTER_GSBI_I2C_DEVICE RegisterI2cDevice
  )
{
  return EFI_SUCCESS;
}

