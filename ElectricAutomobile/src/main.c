/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include <asf.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "pulse.h"
#include "driving.h"

static void configureConsole(void);
static void test_pulse(void);

static void test_pulse(void)
{
	//Generating PWM signal on Digital pin 2 using a Timer Counter WORKS PERFECTLY ----> SAVE THIS PIECE OF TEST CODE! 
	double pwm_value = 25;
	pulse_start(EnA, pwm_value);
	//pulse_start(EnB, pwm_value);
	while (1)
	{
		uint32_t ra_val = tc_read_ra(TC0, TC_WAVE_CHANNEL_0);
		uint32_t rc_val = tc_read_rc(TC0, TC_WAVE_CHANNEL_0);
		printf("RA-value: %lu ----- RC-value: %lu\n", ra_val, rc_val);
		delay_ms(100);
	}			
}

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
	test_pulse();
	//setup_motor_pins();
	/*
	while (1)
	{
		accelerate(50);
		delay_ms(2000);
		decelarate(50);
		turn_off_motors();
		delay_ms(2000);
	}*/
}
