#include <PiDxe.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/QcomSsbi.h>
#include <Library/LKEnvLib.h>
#include "ssbi.h"

STATIC QCOM_SSBI_PROTOCOL gSSBI = {
  i2c_ssbi_read_bytes,
  i2c_ssbi_write_bytes,
  pa1_ssbi2_read_bytes,
  pa1_ssbi2_write_bytes,
  pa2_ssbi2_read_bytes,
  pa2_ssbi2_write_bytes,
};

EFI_STATUS
EFIAPI
SsbiDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomSsbiProtocolGuid,      &gSSBI,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
