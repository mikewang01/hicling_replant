/***************************************************************************//**
 * File: sysclk.c
 *
 * Description: System clock manager
 *
 *
 * Created on Feb 08, 2014
 *
 * 
 *****************************************************************************/

#include "main.h"

#include "standards.h"


/* Uses the 16-bit TimerA0 counter and the lower 16 bits of the system
 * clock to give a "fine-grained" time. */
I32U SYSCLK_GetFineTime(void)
{
	double tick_diff;
	I32U tick_now;
	double system_clock_in_ms;

	// Get the elapsed time
	tick_now = lld_evt_time_get();
	tick_diff = tick_now - cling.time.tick_count;
	system_clock_in_ms = cling.time.system_clock_in_sec*1000;
	system_clock_in_ms += tick_diff/1.6;

	N_SPRINTF("[SYSCLK]: sec: %d, n: %d, o:%d", cling.time.system_clock_in_sec, tick_now, cling.time.tick_count);
	// 32758 ticks per second, about 32 tick per ms.
	return (I32U)system_clock_in_ms;
}

/** @} */
