#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomScmLib.h>
#include <Library/UefiBootServicesTableLib.h>

QCOM_SCM_PROTOCOL *gScm = NULL;

RETURN_STATUS
EFIAPI
ScmLibConstructor (
  VOID
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID **)&gScm);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
