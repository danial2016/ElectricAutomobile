/*
 * pulse.c
 * 
 * A TC channel can be used for pulse width modulation (PWM). This module drives the PWM signal using a Timer Counter. 
 * A TC channel can operate in Waveform-mode which provides wave generation. Each channel has two user-configurable
 * multi-purpose I/O signals (TIOA and TIOB). A pin can be driven by the PIO Controller or by an on-chip peripheral.
 * These pins are multiplexed with PIO lines, and hence have to be assigned to their peripheral functions. The drive 
 * of the I/O lines is this controlled by the peripheral In Waveform-mode TIOA is configured as an output. Our conf-
 * -iguration, which is set out in detail in the comments in the code, causes the TC- channel to generate one or two 
 * PWM signals with the same frequency and independently programmable duty cycles.
 *
 * Note: Using the TC-module to generate a PWM wave is quite an unorthodox method. It may be more convenient and formal
 * to use the inbuilt PWM-peripheral circuitry of the chip.
 *
 * Created: 2019-02-25 11:01:22
 *  Author: Daniel
 */ 

 #include <asf.h>
 #include "pulse.h"
 #include "driving.h"

 static void pulse_init(void);
 static double rc = 279; //656;
 
 static void pulse_init(void)
 {
	 ioport_init();
	 
	 /* 
	  * First we assign the I/O line to the Peripheral B function. Then we
	  * disable the PIO from controlling the corresponding pin. This enables 
	  * peripheral control of the pin. 
	  * 
	  * Note: Peripheral functions of I/O pins can be found in pages pp. 858-859 of SAM3X datasheet
	  */
	 ioport_set_pin_mode(PIO_PB25_IDX, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP); // PB_25 (Digital pin 2) is PWM pin of Motor A
	 ioport_disable_pin(PIO_PB25_IDX);
	 
	 ioport_set_pin_mode(PIO_PC28_IDX, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP); // PC_28 (Digital pin 3) is PWM pin of Motor B
	 ioport_disable_pin(PIO_PC28_IDX);
	 
	 pmc_set_writeprotect(false);
	 
	 /* Configure the PMC to enable the TC module. 
	  * In the links below you can find information about how I/O lines A and B are routed to the pins on the Arduino Board.
	  * You will also find information about which I/O line is connected to which Timer Counter Channel. In addition, the 
	  * second link contains a nice table over the PMC id:s of every Timer Counter channel.
	  *    (1) https://github.com/ivanseidel/DueTimer/blob/master/TimerCounter.md
	  *    (2) http://ko7m.blogspot.com/2015/01/arduino-due-timers-part-1.html
	  */
	 // TIOA0 = PB_25 = Digital pin 2 on Arduino Due board, TC0 channel 0 (peripheral function B)
	 pmc_enable_periph_clk(ID_TC0); // PMC ID of TC0 ch0 = ID_TC0
	 
	 // TIOA7 = PC_28 = Digital pin 3 on Arduino Due board, TC2 channel 1 (peripheral function B)
	 pmc_enable_periph_clk(ID_TC7); // PMC ID of TC2 ch1 = ID_TC7
	 
	 
	 /* 
	  * Configuring TC_CMR (TC Channel Mode Register):
	  * 
	  * Bit(s)                Value     Name of bit(s)                 Description
	  * ------------          -----     --------------                 -----------
	  * TCCLKS (bits 0-2)       3       [Clock Selection]              The selected clock is internal MCK/128 clock signal (from PMC) 
	  * WAVESEL (bits 13-14)    2       [Waveform Selection]           UP mode with automatic trigger on RC Compare (see below)
	  * CPCTRG (bit 14) (alt)   1       [RC Compare Trigger Enable]    RC Compare resets the counter and starts the counter clock
	  * WAVE (bit 15)           1       [Waveform Mode]                Waveform mode is enabled
	  * ACPA (bits 16-17)       1       [RA Compare Effect on TIOA]    TIOA is SET on RA Compare
	  * ACPC (bits 18-19)       2       [RC Compare Effect on TIOA]    TIOA is CLEARED on RC Compare 
	  *
	  * Note: MCK is the master clock (84 MHz for the Arduino Due)
	  */
	 	 
	 /*
	  * When WAVSEL = 10, the value of TC_CV is incremented from 0 to the value
	  * of RC, then automatically resets on a RC Compare. Once the value of TC_CV
	  * has been reset, it is then incremented and so on. On compare match with RA
	  * the output of TIOA goes HIGH. On RC compare the output of TIOA goes low.
	  * 
	  * Now we are going to deduce the formula for the PWM frequency. The frequency 
	  * of the selected clock is MCK/128. This means that for one count of the 
	  * counter it is equal to (in time): 
	  *
	  *     1/clk_freq = clk_period (time/count)
	  *
	  * The time it takes for the counter to reach the value of RC = RC * clk_period
	  * Therefore, the frequency of the TC wave (i.e. PWM wave) equals:
	  *
	  *     1/(RC * clk_period) = 1/(RC * 1/clk_freq) = 1/(RC * 1/(MCK/presc)) =
	  *     1/(RC * presc/MCK) = MCK/(RC * presc)
	  * 
	  * In our case the PWM frequency = MCK/(RC * presc) = 84 MHz/(656 * 128) = 1 kHz
	  * 
	  */
	 tc_init(TC0, TC_WAVE_CHANNEL_0, TC_CMR_TCCLKS_TIMER_CLOCK4 
								| TC_CMR_WAVSEL_UP_RC 
								| TC_CMR_WAVE 
								| TC_CMR_ACPA_SET 
								| TC_CMR_ACPC_CLEAR); // digital pin 2
								
	tc_init(TC2, TC_WAVE_CHANNEL_1, TC_CMR_TCCLKS_TIMER_CLOCK4 
								| TC_CMR_WAVSEL_UP_RC 
								| TC_CMR_WAVE 
								| TC_CMR_ACPA_SET 
								| TC_CMR_ACPC_CLEAR); // digital pin 3
 }
 
 /*
  * Starts the PWM wave generation with an initial user requested duty cycle.
  *
  * /param  duty_cycle - has to be in procent (0 < duty_cycle < 100)
  * /return void
  */
 void pulse_start(int pin, double duty_cycle)
 {
	pulse_init();
	
	if (pin == EnA)
	{
		pulse_set_duty_cycle(EnA, duty_cycle);
		tc_start(TC0, TC_WAVE_CHANNEL_0);
	}
	if (pin == EnB)
	{
		pulse_set_duty_cycle(EnB, duty_cycle);
		tc_start(TC2, TC_WAVE_CHANNEL_1);
	}
 }
 
 void pulse_set_duty_cycle(int pin, double duty_cycle)
 {
	 double ra = 0;
	 ra = rc * (1 - duty_cycle/100);
	 
	 /*
	  * RA Compare is used to control the TIOA output. The value of RA will regulate the
	  * duty cycle. The signal will go high on RA compare and get cleared on RC compare.
	  */	
	 if (pin == EnA)
	 {
		 tc_write_ra(TC0, TC_WAVE_CHANNEL_0, (uint32_t) ra);
		 tc_write_rc(TC0, TC_WAVE_CHANNEL_0, (uint32_t) rc);
	 }
	 if (pin == EnB)
	 {
		 tc_write_ra(TC2, TC_WAVE_CHANNEL_1, (uint32_t) ra);
		 tc_write_rc(TC2, TC_WAVE_CHANNEL_1, (uint32_t) rc);
	 }
 }
 
 void pulse_stop(void)
 {
	 tc_stop(TC0, TC_WAVE_CHANNEL_0); //stop Motor A
	 tc_stop(TC2, TC_WAVE_CHANNEL_1); //stop Motor B
 }
 