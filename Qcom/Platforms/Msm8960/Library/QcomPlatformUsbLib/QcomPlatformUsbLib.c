#include <Base.h>
#include <Library/LKEnvLib.h>
#include <Library/QcomClockLib.h>
#include <Library/QcomBoardLib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/QcomPlatformUsbLib.h>
#include <Target/board.h>

STATIC VOID apq8064_ext_3p3V_enable(VOID)
{
  gGpioTlmm->SetFunction(77, 0);
  gGpioTlmm->SetDriveStrength(77, 8);
  gGpioTlmm->SetPull(77, GPIO_PULL_NONE);
  gGpioTlmm->DirectionOutput(77, 1);
}

/* Do target specific usb initialization */
STATIC VOID target_usb_init(target_usb_iface_t *iface)
{
  if(gBoard->board_target_id() == LINUX_MACHTYPE_8064_LIQUID)
  {
    apq8064_ext_3p3V_enable();
  }
}

STATIC VOID hsusb_clock_init(target_usb_iface_t *iface)
{
  gClock->clk_get_set_enable("usb_hs_clk", 60000000, 1);
}

EFI_STATUS LibQcomPlatformUsbGetInterface(target_usb_iface_t *iface)
{
  iface->controller = L"ci";
  iface->usb_init   = target_usb_init;
  iface->clock_init = hsusb_clock_init;

  return EFI_SUCCESS;
}

