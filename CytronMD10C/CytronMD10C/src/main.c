/**
 * main.c
 * By Danial Mahmoud
 */

#include <asf.h>
#include "driving.h"
#include "pulse.h"

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
	int target_speed = 50;
	while (1)
	{
		set_direction(FORWARD);
		accelerate(target_speed);
		drive_forward(target_speed);
		delay_ms(5000);
		int start_speed = target_speed;
		decelerate(start_speed);
		set_direction(REVERSE);
		accelerate(target_speed);
		drive_backwards(50);
		delay_ms(5000);	
		decelerate(start_speed);
		delay_ms(1000);
	}

	/* Insert application code here, after the board has been initialized. */
}
