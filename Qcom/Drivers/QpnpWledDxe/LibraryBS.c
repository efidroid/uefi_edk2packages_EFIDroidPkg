#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomQpnpWledLib.h>
#include <Library/UefiBootServicesTableLib.h>

QCOM_QPNP_WLED_PROTOCOL *gQpnpWled = NULL;

RETURN_STATUS
EFIAPI
QpnpWledLibConstructor (
  VOID
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gQcomQpnpWledProtocolGuid, NULL, (VOID **)&gQpnpWled);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
