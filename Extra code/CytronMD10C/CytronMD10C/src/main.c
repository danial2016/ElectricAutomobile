/**
 * main.c
 * By Danial Mahmoud
 */

#include <asf.h>
#include "driving.h"
#include "pulse.h"
#include "test_cases.h"
#include "encoders.h"

/**
* \brief Configure UART for debug message output.
*/
static void configureConsole(void)
/* Enables feedback through the USB-cable back to terminal within Atmel Studio */
/* Note that the baudrate, parity and other parameters must be set in config/conf_uart_serial.h */
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
	sysclk_init();
	board_init();
	configureConsole();
	init_motors();
	cpu_irq_enable();
	pulseCounter_configInterrupt();
	int target_speed = 50;
	
	// Choose test case from test suite for any kind of robot testing
	test_case_1(target_speed);
	while (1)
	{
		printf("CHA Motor left: %lu", chA_motor_left);
		delay_ms(5000);
		chA_motor_left = 0;
	}

}
