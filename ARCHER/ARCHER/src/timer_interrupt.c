/*
 * timer_interrupt.c
 *
 * Created: 2021-01-04 13:23:15
 *  Author: Danial Mahmoud
 */ 

#include "timer_interrupt.h"

uint32_t systicks = 0;

void configure_tc_interrupt(void)
{
	uint32_t rc = 42000; // MCK/(RC * presc) = 84 MHz/(42000*2) = 1 kHz
	
	/* Configure PMC */
	pmc_enable_periph_clk(ID_TC3);

	/** Configure TC for a 1 kHz frequency and trigger on RC compare. */
	tc_init(TC1, 0, TC_CMR_TCCLKS_TIMER_CLOCK2
			| TC_CMR_WAVSEL_UP_RC
			| TC_CMR_WAVE
			| TC_CMR_ACPA_SET
			| TC_CMR_ACPC_CLEAR);
	tc_write_rc(TC1, 0, rc);

	/* Configure and enable interrupt on RC compare */
	NVIC_EnableIRQ((IRQn_Type) ID_TC3);
	tc_enable_interrupt(TC1, 0, TC_IER_CPCS);
	tc_start(TC1, 0);
}


void TC3_Handler(void)
{
	// enters every 1 ms
	systicks++;
}