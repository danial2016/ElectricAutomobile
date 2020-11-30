/*
* adc_with_pdc.c
*
* Created: 2018-04-01 17:04:52
*  Author: Daniel
*/

/*
* The connected PDC channel to the ADC-peripheral is a Receive channel (i.e. a transfer from peripheral to memory)
* It receives/reads converted data from the ADC-sampling process and stores it in a memory space
*/
#include <asf.h>
#include "adc_with_pdc.h"
#include "configure_tc.h"
#include "dec_string.h"
#include <string.h>
#include <inttypes.h>

static float get_average_dcOffset(int16_t *buffer_array);
static float zero_crossings(uint32_t dc_offset, int index_sample_high, int index_sample_low, int16_t * buffer_array);
static void write_to_DAC(int16_t *buffer_array);
static void frequency_estimation(float *frequency, int16_t *buffer_array);

/* Read converted data through PDC channel. */
static uint32_t adc_read_buffer(Adc * p_adc, int16_t * p_s_buffer, uint32_t ul_size)
{
	/* Check if the first PDC bank is free: if so start transferring data to it.*/
	if ((p_adc->ADC_RCR == 0) && (p_adc->ADC_RNCR == 0)) {
		// The two lines of code below load the buffer address and size into the PDC associated channel register.
		p_adc->ADC_RPR = (uint32_t) p_s_buffer; /* Receive Pointer Register */
		p_adc->ADC_RCR = ul_size;               /* Receive Counter Register */
		
		/* Enables PDC receiver channel requests */
		p_adc->ADC_PTCR = ADC_PTCR_RXTEN; 

		return 1;
		} else {	/* Check if the second PDC bank is free. */
		if (p_adc->ADC_RNCR == 0)
		{
			p_adc->ADC_RNPR = (uint32_t) p_s_buffer; /* Receive Next Pointer Register */
			p_adc->ADC_RNCR = ul_size;               /* Receive Next Counter Register */
			return 1;
		}
		else {
			return 0;
		}
	}
}

/* Configure the ADC */
void adc_setup()
{
	pmc_enable_periph_clk(ID_ADC);
	/* Initialize ADC */
	//adc_init(ADC, sysclk_get_cpu_hz(), ADC_CLOCK, ADC_STARTUP_TIME_4);
	
	/* Set ADC timing */
	adc_init(ADC, sysclk_get_main_hz(), 20000000, 0); //adc_configure_timing(ADC, TRACKING_TIME, SETTLING_TIME, TRANSFER_TIME);
	adc_configure_timing(ADC, 0, 0, 0);
	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);
	adc_enable_channel(ADC, ADC_CHANNEL_0);			//PA2 - AD7 for Arduino Due
	/* Trigger conversion just by software */
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
	/* Start transferring converted data to buffer */
	//adc_read_buffer(ADC, adc_buffer_sample_values, ADC_BUFFER_SIZE);
	/* Enable PDC channel interrupt. */
	//adc_enable_interrupt(ADC, ADC_IER_RXBUFF);
	/* Enable ADC interrupt. */
	//NVIC_EnableIRQ(ADC_IRQn);
}

void dac_setup() 
{
	pmc_enable_periph_clk(ID_DACC);
	dacc_reset(DACC);
	dacc_set_transfer_mode(DACC, 0);
	dacc_set_timing(DACC,1,1,0);			//1 is shortest refresh period, 1 max. speed, 0 startup time
	
	dacc_set_channel_selection(DACC,0);		//Channel DAC0
	dacc_enable_channel(DACC, 0);			//enable DAC0
}

static float get_average_dcOffset(int16_t *buffer_array)
{
	float sum = 0;
	for (int i = 0; i < ADC_BUFFER_SIZE; i++)
	{
		sum+= (float)buffer_array[i];
	}
	float average = sum/ADC_BUFFER_SIZE;
	return average;
}

static float zero_crossings(uint32_t dc_offset, int index_sample_high, int index_sample_low, int16_t * buffer_array)
{
	float zero = 0;
	if (index_sample_high < index_sample_low){
		int a = buffer_array[index_sample_high];
		int b = dc_offset;
		int c = buffer_array[index_sample_low];
		float sample_period = (float)1/sample_frequency;
		float x = ((float)(b-c)/(a-c))*sample_period;
		float offsetTime = index_sample_high*sample_period;
		zero = offsetTime+(sample_period-x);
	}
	else{
		int a = buffer_array[index_sample_high];
		int b = dc_offset;
		int c = buffer_array[index_sample_low];
		float sample_period = (float)1/sample_frequency;
		float x = ((float)(b-c)/(a-c))*sample_period;
		float offsetTime = index_sample_low*sample_period;
		zero = offsetTime+x;//(sample_period-x);
	}
	
	return zero;
}

static void frequency_estimation(float *frequency, int16_t *buffer_array)
{
	uint32_t dc_offset = (uint32_t)get_average_dcOffset(buffer_array);
	int k = 0;
	int zeros = 0;
	float zeros_array[21];
	for (int i = 0; i < ADC_BUFFER_SIZE; i++)
	{
		if (zeros <= 21)
		{
			if ((buffer_array[i] <= dc_offset) && (buffer_array[i-1] >= dc_offset))
			{
				if (buffer_array[i] == dc_offset)
				{
					buffer_array[i] -= 1;
				}
				if (buffer_array[i-1] == dc_offset)
				{
					buffer_array[i-1] += 1;
				}
				int index_sample_high = i-1;
				int index_sample_low = i;
				float z = zero_crossings((uint32_t)dc_offset, index_sample_high, index_sample_low, buffer_array);
				zeros_array[k] = z;
				if (k >= 20)
				{
					k = 0;
					float period = (float)(zeros_array[20]-zeros_array[0])/10; //average over 10 periods
					*frequency = (float)(1/period);
					break;
				}
				k++;
				zeros++;
			}
			else if ((buffer_array[i] >= dc_offset) && (buffer_array[i-1] <= dc_offset))
			{
				if (buffer_array[i] == dc_offset)
				{
					buffer_array[i] += 1;
				}
				if (buffer_array[i-1] == dc_offset)
				{
					buffer_array[i-1] -= 1;
				}
				int index_sample_high = i;
				int index_sample_low = i-1;
				float z = zero_crossings((uint32_t)dc_offset, index_sample_high, index_sample_low, buffer_array);
				zeros_array[k] = z;
				if (k >= 20)
				{
					k = 0;
					float period = (float)(zeros_array[20]-zeros_array[0])/10; //average over 10 periods
					*frequency = (float)(1/period);
					break;
				}
				k++;
				zeros++;
			}
		}
		else{
			break;
		}
	}
}

static void write_to_DAC(int16_t *buffer_array)
{
	for(int i = 0; i < ADC_BUFFER_SIZE; i++){
		delay_us(2);
		dacc_write_conversion_data(DACC, buffer_array[i]);
	}
}

void ADC_Handler(void){
	/* Data transfer has been stopped (is complete) and RXBUFF status flag is set. Buffer of sample values is full. */
	if ((adc_get_status(ADC) & ADC_ISR_RXBUFF) == ADC_ISR_RXBUFF) {
		//tc_stop(TC0, 0); 
		write_to_DAC(adc_buffer_sample_values);
		/*
		for (int i = 0; i < ADC_BUFFER_SIZE; i++)
		{
			printf("Buffer values: %u, index: %u\n", adc_buffer_sample_values[i], i);
		}*/
		float frequency = 0;
		frequency_estimation(&frequency, adc_buffer_sample_values);
		printf("Frequency: %s\n", get_decimal_string(frequency));
		memset((void *)&adc_buffer_sample_values, 0, sizeof(adc_buffer_sample_values));	/* Clear sample buffer */
		adc_read_buffer(ADC, adc_buffer_sample_values, ADC_BUFFER_SIZE); /* Read new buffer data */
		//pio_enable_interrupt(PIOB, PIO_PB26); //re-enable edge-level detection 
	}
}


