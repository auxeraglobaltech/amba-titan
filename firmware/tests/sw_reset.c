// sw_reset — software-requested chip reset (SoC reset test).
//
// Boot 1 (cause: power-on): check RESET_INFO says POR, clear it, request a
//   chip reset through rstmgr RESET_REQ.
// Boot 2 (cause: software): RESET_INFO must say SW_RESET and nothing else.
//   Pass.
//
// RESET_INFO lives in the always-on domain, so it survives the reset it
// records. The UVM test (fw_reset_test) also checks the chip really booted
// twice, so the firmware cannot pass by skipping the reset.

#include "titan.h"

int main(void) {
  uint32_t info = rstmgr_reset_info();
  titan_printf("sw_reset: RESET_INFO = 0x%x\n", info);

  if (info & RESET_INFO_POR) {
    CHECK(info == RESET_INFO_POR, "first boot: expected only POR, got 0x%x", info);
    rstmgr_clear_reset_info();
    titan_puts("sw_reset: requesting a software reset\n");
    rstmgr_request_sw_reset();           // does not return
  }

  CHECK(info == RESET_INFO_SW_RESET, "second boot: expected only SW_RESET, got 0x%x", info);
  titan_puts("sw_reset: came back from a software reset\n");
  return 0;
}
