/*
 * adc.c
 *
 * Created: 2018-03-28 10:42:30
 *  Author: Daniel
 */ 

#include <asf.h>
#include "pmc.h"
#include "afec.h"




void afec_setup(){
	pmc_set_writeprotect(0); // disable write protect of PMC registers
	pmc_enable_periph_clk(ID_AFEC0); //peripheral ID of Analog Front End Controller 0 is 30
	afec_enable(AFEC0);
	struct afec_config afec_cfg; 
	afec_get_config_defaults(&afec_cfg); 
	afec_init(AFEC0, &afec_cfg); 
	afec_set_trigger(AFEC0, AFEC_TRIG_SW);
	afec_channel_enable(AFEC0, AFEC_CHANNEL_5); 
}

