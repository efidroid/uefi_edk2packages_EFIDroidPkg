#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/QcomPm8921.h>
#include <Platform/iomap.h>

STATIC QCOM_PM8921_PROTOCOL *mPm8921 = NULL;

STATIC VOID shutdown_device(VOID)
{
  mPm8921->pm8921_config_reset_pwr_off(0);

  // Actually reset the chip
  writel(0, MSM_PSHOLD_CTL_SU);
  mdelay(5000);

  DEBUG((EFI_D_ERROR, "Shutdown failed.\n"));
}

STATIC VOID reboot_device(UINTN reboot_reason)
{
  writel(reboot_reason, RESTART_REASON_ADDR);

  // Actually reset the chip
  mPm8921->pm8921_config_reset_pwr_off(1);
  writel(0, MSM_PSHOLD_CTL_SU);
  mdelay(10000);

  DEBUG((EFI_D_ERROR, "PSHOLD failed, trying watchdog reset\n"));
  writel(1, MSM_WDT0_RST);
  writel(0, MSM_WDT0_EN);
  writel(0x31F3, MSM_WDT0_BT);
  writel(3, MSM_WDT0_EN);
  dmb();
  writel(3, MSM_TCSR_BASE + TCSR_WDOG_CFG);
  mdelay(10000);

  DEBUG((EFI_D_ERROR, "Rebooting failed\n"));
}

EFI_STATUS
EFIAPI
LibResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
{
  switch (ResetType) {
  case EfiResetCold:
    reboot_device(0);
    break;

  case EfiResetWarm:
    reboot_device(0);
    break;

  case EfiResetShutdown:
    shutdown_device();
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  return EFI_DEVICE_ERROR;
}

EFI_STATUS
EFIAPI
LibInitializeResetSystem (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gQcomPm8921ProtocolGuid, NULL, (VOID **)&mPm8921);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

