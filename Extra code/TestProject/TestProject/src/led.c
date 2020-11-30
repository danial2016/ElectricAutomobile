/*
 * led.c
 *
 * Created: 2018-03-21 12:04:23
 *  Author: Daniel
 */ 
#include "led.h"
#include <asf.h>

#define WPKEY_PMC_WPMR      0x504D43U
#define WPKEY_PIO_WPMR      0x50494FU
#define WPEN (uint32_t)0
#define PMC_WPMR            0x400E04E4U /* PMC Write Protection Mode Register */
#define PMC_PCER0           0x400E0410U /* PMC Peripheral Clock Enable Register 0 */
#define PIO_WPMR_PIOA       0x400E0EE4U /* PIOA Write Protection Mode Register */
#define PIO_PER_PIOA        0x400E0E00U /* PIO Enable Register */
#define PIO_OER_PIOA        0x400E0E10U /* PIOA Output Enable Register */
#define PIO_SODR_PIOA       0x400E0E30U /* PIO Set Output Data Register */
#define PIO_CODR_PIOA       0x400E0E34U /* PIO Clear Output Data Register */

/* Pointers to access hardware register */
uint32_t *const p_PMC_WPMR = (uint32_t *) PMC_WPMR;
uint32_t *const p_PMC_PCER0 = (uint32_t *) PMC_PCER0;
uint32_t *const p_PIO_WPMR_PIOA = (uint32_t *) PIO_WPMR_PIOA; 
uint32_t *const p_PIO_PER_PIOA = (uint32_t *) PIO_PER_PIOA;
uint32_t *const p_PIO_OER_PIOA = (uint32_t *) PIO_OER_PIOA;
uint32_t *const p_PIO_SODR_PIOA = (uint32_t *) PIO_SODR_PIOA;
uint32_t *const p_PIO_CODR_PIOA = (uint32_t *) PIO_CODR_PIOA;

void disable_write_protect()
{
	*p_PMC_WPMR = 0;
	*p_PMC_WPMR = WPKEY_PMC_WPMR;
	*p_PMC_WPMR &= ~(1 << WPEN); // clear WPEN: disables the write protection if WPKEY corresponds to 0x504D43 (“PMC” in ASCII).
	*p_PMC_PCER0 |= (1 << 9);    // Enables the peripheral clock of Peripheral ID 9, i.e. PIOA Controller 

	*p_PIO_WPMR_PIOA = 0;
	*p_PIO_WPMR_PIOA = WPKEY_PIO_WPMR;
	*p_PIO_WPMR_PIOA &= ~(1 << WPEN); //Disables the write protection if WPKEY corresponds to 0x50494F (“PIO” in ASCII).
	
}

void enable_pin(){
	*p_PIO_PER_PIOA |= (1<<1); // Enable pin PA1 to be controlled
	*p_PIO_OER_PIOA |= (1<<1); // Enables the output on the I/O line PA1
}

void set_output(){
	*p_PIO_SODR_PIOA |= (1<<1); // Sets output data on I/O line PA1
}

void clear_output(){
	*p_PIO_CODR_PIOA |= (1<<1); // Clears output data on I/O line PA1
}









