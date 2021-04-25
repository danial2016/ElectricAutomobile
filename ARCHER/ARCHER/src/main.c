/**
 * main.c
 * By Danial Mahmoud
 */

#include <asf.h>
#include "driving.h"
#include "pulse.h"
#include "test_cases.h"
#include "encoders.h"
#include "timer_interrupt.h"

#define TEST_PIN    PIO_PC8_IDX

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
	pulseCounter_configInterrupt();
	cpu_irq_enable();
	//SysTick_Config(84000);
	//ioport_set_pin_dir(TEST_PIN, IOPORT_DIR_OUTPUT);
	
	float Kp_left = 0, Kp_right = 0;
	float u_left = 0, u_right = 0;
	uint32_t u_start = 30;
	uint32_t b = 300;
	
	/* 10 - 90 PWM duty cycle, 10 V max. 12 V seems unstable.*/
	uint32_t sec_left = 0, sec_right = 0;
	uint32_t start = 0, stop = 0, diff = 0;
	uint32_t Ts = 1000; // ms
	// Choose test case from test suite for any kind of robot testing
	//test_case_1(target_speed);
	set_direction(FORWARD);
	/*
	2021-04-24
	By the grace of God, we have found an amazing control strategy 
	One that maintains a steady output and constant speed! Not a 
	traditional PID, don't really know what to call it ...
	There has to be a start value. But this start value should be
	determined through fuzzy control instead.
	*/
	pulse_set_duty_cycle_left(u_start);
	pulse_set_duty_cycle_right(u_start);
	u_left = u_start;
	u_right = u_start;
	delay_ms(2000);
	while (1)
	{
		start = systicks;
		printf("%lu. %lu  ----- %lu. %lu\n", sec_left++, chA_motor_left, sec_right++, chA_motor_right);
		
		Kp_left = (b/((float)chA_motor_left));
		u_left = Kp_left*u_left;
		
		Kp_right = (b/((float)chA_motor_right));
		u_right = Kp_right*u_right;
		
		if(u_left > 90)
		{
			u_left = 90;
		}
		else if (u_left < 0)
		{
			u_left = 0;
		}
		
		if(u_right > 90)
		{
			u_right = 90;
		}
		else if (u_right < 0)
		{
			u_right = 0;
		}
		
		chA_motor_left = 0;
		chA_motor_right = 0;
		
		pulse_set_duty_cycle_left(u_left);
		pulse_set_duty_cycle_right(u_right);
		
		stop = systicks;
		diff = stop - start;
		if (diff < Ts)
		{
			delay_ms(Ts-diff);
		} 
		else
		{
			delay_ms(2*Ts-diff);
		}
	}
}

void SysTick_Handler(void)
{
	/* SysTick interrupt handler counts from 84000 to 0 in 1 ms, 
		so the variable gets incremented every 1 ms. Period has been confirmed using oscilloscope. */
	systicks++;
	//ioport_toggle_pin_level(TEST_PIN);
}