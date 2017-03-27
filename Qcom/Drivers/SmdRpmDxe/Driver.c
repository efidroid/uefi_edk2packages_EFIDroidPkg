#include <PiDxe.h>
#include <Library/LKEnvLib.h>

#include <Protocol/QcomRpm.h>
#include <Library/UefiBootServicesTableLib.h>

#include "rpm-ipc.h"
#include "rpm-smd.h"

STATIC QCOM_RPM_PROTOCOL mInternalRpm = {
  rpm_send_data,
  rpm_clk_enable,
};

EFI_STATUS
EFIAPI
RpmDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  rpm_smd_init();

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomRpmProtocolGuid,      &mInternalRpm,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
