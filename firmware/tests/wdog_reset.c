// wdog_reset — the always-on watchdog resets the chip (SoC reset test).
//
// Boot 1 (power-on): let pwrmgr turn the aon_timer watchdog bite into a reset
//   request, start the watchdog (bite after BITE_CYCLES of the 200 kHz AON
//   clock = 100 us), and do not pet it.
// Boot 2: RESET_INFO must say HW_REQ for request 1 (aon_timer) and nothing
//   else. Pass.
//
// The UVM test (fw_reset_test) also checks there were exactly two boots.

#include "titan.h"

#define BITE_CYCLES 20         // x 5 us
#define BARK_CYCLES 1000       // later than the bite: no bark interrupt
#define WDOG_REQ    kTopEarlgreyPowerManagerResetRequestsAonTimerAonAonTimerRstReq

int main(void) {
  uint32_t info = rstmgr_reset_info();
  titan_printf("wdog_reset: RESET_INFO = 0x%x\n", info);

  if (info & RESET_INFO_POR) {
    CHECK(info == RESET_INFO_POR, "first boot: expected only POR, got 0x%x", info);
    rstmgr_clear_reset_info();
    pwrmgr_enable_reset_request(WDOG_REQ);
    timer_init_us();
    aon_wdog_start(BARK_CYCLES, BITE_CYCLES);
    titan_puts("wdog_reset: watchdog started, not petting it\n");
    delay_us(2000);                       // the bite comes after ~100 us
    CHECK(false, "still running 2 ms after starting the watchdog: no reset happened");
  }

  CHECK(info == RESET_INFO_HW_REQ(WDOG_REQ),
        "second boot: expected only HW_REQ[aon_timer] (0x%x), got 0x%x",
        RESET_INFO_HW_REQ(WDOG_REQ), info);
  titan_puts("wdog_reset: came back from a watchdog reset\n");
  return 0;
}
