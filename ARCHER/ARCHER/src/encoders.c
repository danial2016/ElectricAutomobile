/*
 * encoders.c
 *
 * Created: 2020-08-22 16:00:59
 *  Author: Anis
 */ 

#include "encoders.h"

uint32_t chA_motor_left = 0;
//uint32_t chB_motor_left = 0;
uint32_t chA_motor_right = 0;
//uint32_t chB_motor_right = 0;


void pulseCounter_handler_CHA_MOTOR_LEFT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHA_PIN_MOTOR_LEFT))
	{
		chA_motor_left++;
	}
}

/*
void pulseCounter_handler_CHB_MOTOR_LEFT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHB_PIN_MOTOR_LEFT))
	{
		chB_motor_left++;
	}
}*/

void pulseCounter_handler_CHA_MOTOR_RIGHT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHA_PIN_MOTOR_RIGHT))
	{
		chA_motor_right++;
	}
}

/*
void pulseCounter_handler_CHB_MOTOR_RIGHT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHB_PIN_MOTOR_RIGHT))
	{
		chB_motor_right++;
	}
}*/

void pulseCounter_configInterrupt(void){
	pmc_set_writeprotect(false);
	pmc_enable_periph_clk(ID_PIOD);
	pio_set_input(PIOD, PIO_PD1, PIO_PULLUP);
	//pio_set_input(PIOD, PIO_PD2, PIO_PULLUP);
	pio_set_input(PIOD, PIO_PD3, PIO_PULLUP);
	//pio_set_input(PIOD, PIO_PD6, PIO_PULLUP);
	pio_handler_set(PIOD, ID_PIOD, PIO_PD1, PIO_IT_EDGE, pulseCounter_handler_CHA_MOTOR_LEFT);
	//pio_handler_set(PIOD, ID_PIOD, PIO_PD2, PIO_IT_EDGE, pulseCounter_handler_CHB_MOTOR_LEFT);
	pio_handler_set(PIOD, ID_PIOD, PIO_PD3, PIO_IT_EDGE, pulseCounter_handler_CHA_MOTOR_RIGHT);
	//pio_handler_set(PIOD, ID_PIOD, PIO_PD6, PIO_IT_EDGE, pulseCounter_handler_CHB_MOTOR_RIGHT);
	pio_enable_interrupt(PIOD, PIO_PD1);
	//pio_enable_interrupt(PIOD, PIO_PD2);
	pio_enable_interrupt(PIOD, PIO_PD3);
	//pio_enable_interrupt(PIOD, PIO_PD6);
	NVIC_EnableIRQ(PIOD_IRQn);
}

void HardFault_Handler  ( void )
{
	printf("Hard fault");
}