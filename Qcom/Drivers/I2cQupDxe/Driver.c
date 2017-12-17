#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/QcomI2cQup.h>
#include <Chipset/gsbi.h>
#include <Library/QcomPlatformI2cQupLib.h>

#include "i2c_qup_p.h"

typedef struct {
  LIST_ENTRY            Link;
  UINT8                 Id;
  struct qup_i2c_dev    *Device;
} DEVICE_LIST_NODE;

STATIC LIST_ENTRY mDevices = INITIALIZE_LIST_HEAD_VARIABLE (mDevices);

STATIC struct qup_i2c_dev * qup_i2c_get_dev(UINT8 Id) {
  DEVICE_LIST_NODE  *Node;
  LIST_ENTRY        *Link;

  Node = NULL;
  for (Link = mDevices.ForwardLink; Link != &mDevices; Link = Link->ForwardLink) {
    Node = BASE_CR (Link, DEVICE_LIST_NODE, Link);
    if (Node->Id == Id) {
      return Node->Device;
    }
  }

  return NULL;
}

STATIC EFIAPI EFI_STATUS RegisterI2cDevice(UINT8 gsbi_id, UINTN clk_freq, UINTN src_clk_freq) {
  EFI_STATUS        Status;
  DEVICE_LIST_NODE  *NewNode;

  struct qup_i2c_dev *Device = qup_i2c_init(gsbi_id, clk_freq, src_clk_freq);
  if (Device==NULL) {
    return EFI_DEVICE_ERROR;
  }

  NewNode = AllocatePool (sizeof (DEVICE_LIST_NODE));
  if (NewNode == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto DEVICE_DEINIT;
  }

  NewNode->Id         = gsbi_id;
  NewNode->Device     = Device;

  InsertTailList (&mDevices, &NewNode->Link);

  return EFI_SUCCESS;

DEVICE_DEINIT:
  qup_i2c_deinit(Device);

  return Status;
}

STATIC EFIAPI VOID qup_i2c_iterate (qup_i2c_iterate_cb_t cb) {
  DEVICE_LIST_NODE  *Node;
  LIST_ENTRY        *Link;

  Node = NULL;
  for (Link = mDevices.ForwardLink; Link != &mDevices; Link = Link->ForwardLink) {
    Node = BASE_CR (Link, DEVICE_LIST_NODE, Link);
    cb (Node->Id);
  }
}

STATIC QCOM_I2C_QUP_PROTOCOL mI2cQup = {
  qup_i2c_get_dev,
  qup_i2c_xfer,
  qup_i2c_iterate,
};


EFI_STATUS
EFIAPI
I2cQupDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  LibQcomPlatformI2cQupAddBusses (RegisterI2cDevice);

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gQcomI2cQupProtocolGuid,      &mI2cQup,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}

EFI_STATUS
EFIAPI
I2cQupDxeUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  DEVICE_LIST_NODE  *Node;

  while (!IsListEmpty (&mDevices)) {
    Node = BASE_CR (mDevices.ForwardLink, DEVICE_LIST_NODE, Link);
    RemoveEntryList (&Node->Link);

    qup_i2c_deinit (Node->Device);
    FreePool (Node);
  }

  return EFI_SUCCESS;
}
