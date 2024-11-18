#include <stdint.h>

/* This uses the 16-bit timer in "normal mode" where it continuously counts
 * from 0 to 0xFFFF, wrapping around to 0. It runs at clkIO/1024. The expected
 * use-case is to reset the clock and then poll the `timer_read` function to
 * monitor the passage of time. */

void timer_init(void);
void timer_reset(void);
uint16_t timer_read(void);
