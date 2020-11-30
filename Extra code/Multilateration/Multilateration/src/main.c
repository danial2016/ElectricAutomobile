/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include <asf.h>
#include <stdio_serial.h>
#include <inttypes.h>
#include "conf_board.h"
#include "adc_with_pdc.h"
#include "amplitude_trigger.h"
#include "configure_tc.h"
#include "dec_string.h"
#include "sampling.h"

/**
 * \brief Configure UART for debug message output.
 */
static void configureConsole(void)
/* Enables feedback through the USB-cable back to terminal within Atmel Studio */
/* Note that  the baudrate, parity and other parameters must be set in conf/conf_uart_serial.h */
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	printf("Console ready\n");
	printf("=============\n");
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	configureConsole();
	
	/* Insert application code here, after the board has been initialized. */
	
	ioport_init();
	configure_tc();
	adc_setup();
	dac_setup();
	init_amplitude_trigger();
}
