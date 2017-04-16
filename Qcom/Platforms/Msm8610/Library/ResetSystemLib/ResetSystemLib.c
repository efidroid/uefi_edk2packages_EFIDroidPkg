#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/dload_util.h>
#include <Library/PlatformIdLib.h>
#include <Protocol/QcomPm8x41.h>
#include <Protocol/QcomScm.h>
#include <Platform/iomap.h>
#include <Chipset/reboot.h>

STATIC QCOM_PM8X41_PROTOCOL *mPm8x41 = NULL;
STATIC QCOM_SCM_PROTOCOL *mScm = NULL;

STATIC VOID shutdown_device(VOID)
{
  /* Configure PMIC for shutdown */
  mPm8x41->pm8x41_reset_configure(PON_PSHOLD_SHUTDOWN);

  /* Drop PS_HOLD for MSM */
  writel(0x00, MPM2_MPM_PS_HOLD);

  mdelay(5000);

  DEBUG((EFI_D_ERROR, "shutdown failed\n"));

  ASSERT(0);
}

STATIC VOID reboot_device(UINTN reboot_reason)
{
  int ret = 0;

  writel(reboot_reason, RESTART_REASON_ADDR);

  /* Configure PMIC for warm reset */
  mPm8x41->pm8x41_reset_configure(PON_PSHOLD_WARM_RESET);

  ret = mScm->scm_halt_pmic_arbiter();
  if (ret)
    DEBUG((EFI_D_ERROR, "Failed to halt pmic arbiter: %d\n", ret));

  /* Drop PS_HOLD for MSM */
  writel(0x00, MPM2_MPM_PS_HOLD);

  mdelay(5000);

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

  Status = gBS->LocateProtocol (&gQcomPm8x41ProtocolGuid, NULL, (VOID **)&mPm8x41);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID **)&mScm);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

