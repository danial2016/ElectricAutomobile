/*
 * encoders.c
 *
 * Created: 2020-08-22 16:00:59
 *  Author: Anis
 */ 

#include "encoders.h"

uint32_t chA_motor_left = 0;
uint32_t chB_motor_left = 0;
uint32_t chA_motor_right = 0;
uint32_t chB_motor_right = 0;

uint32_t *const p_IER = (uint32_t*) 0xE000E100;
uint32_t *const p_HARD = (uint32_t*) 0xE000ED2C;

void pulseCounter_handler_CHA_MOTOR_LEFT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHA_PIN_MOTOR_LEFT))
	{
		chA_motor_left++;
	}
}

void pulseCounter_handler_CHB_MOTOR_LEFT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHB_PIN_MOTOR_LEFT))
	{
		chB_motor_left++;
	}
}

void pulseCounter_handler_CHA_MOTOR_RIGHT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHA_PIN_MOTOR_RIGHT))
	{
		chA_motor_right++;
	}
}

void pulseCounter_handler_CHB_MOTOR_RIGHT(const uint32_t id, const uint32_t index)
{
	if (ioport_get_pin_level(CHB_PIN_MOTOR_RIGHT))
	{
		chB_motor_right++;
	}
}

void pulseCounter_configInterrupt(void){
	DUE PINS ARE NOT 5V TOLERANT, SO YOU BUSTED THEM WITH THE ENCODER WHICH OUTPUTS A 5 V SQUARE WAVE.
	THATS WHY THE PROCESSOR ENTERS A HARD FAULT EXCEPTION!!!!!!!!!!!!!!!!!!!!!!!!!!
	pmc_set_writeprotect(false);
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOD);
	pio_set_input(PIOB, PIO_PB26, PIO_PULLUP);
	pio_set_input(PIOA, PIO_PA14, PIO_PULLUP);
	pio_set_input(PIOA, PIO_PA15, PIO_PULLUP);
	pio_set_input(PIOD, PIO_PD0, PIO_PULLUP);
	pio_handler_set(PIOB, ID_PIOB, PIO_PB26, PIO_IT_EDGE, pulseCounter_handler_CHA_MOTOR_LEFT);
	pio_handler_set(PIOA, ID_PIOA, PIO_PA14, PIO_IT_EDGE, pulseCounter_handler_CHB_MOTOR_LEFT);
	pio_handler_set(PIOA, ID_PIOA, PIO_PA15, PIO_IT_EDGE, pulseCounter_handler_CHA_MOTOR_RIGHT);
	pio_handler_set(PIOD, ID_PIOD, PIO_PD0, PIO_IT_EDGE, pulseCounter_handler_CHB_MOTOR_RIGHT);
	pio_enable_interrupt(PIOB, PIO_PB26);
	pio_enable_interrupt(PIOA, PIO_PA14);
	pio_enable_interrupt(PIOA, PIO_PA15);
	pio_enable_interrupt(PIOD, PIO_PD0);
	printf("HEJ");
	//*p_IER |= 1 << (uint32_t)(PIOB_IRQn);
	//NVIC->ISER[0] = (1 << ((uint32_t)(PIOB_IRQn) & 0x1F));
	//NVIC_EnableIRQ(PIOB_IRQn);
	//NVIC_EnableIRQ(PIOA_IRQn);
	//NVIC_EnableIRQ(PIOD_IRQn);
	printf("HEJ igen");
}

void HardFault_Handler  ( void )
{
	uint32_t what = 0;
	what = *p_HARD & (1 << 30);
	printf("HArd fault: %lu\n", what);
}