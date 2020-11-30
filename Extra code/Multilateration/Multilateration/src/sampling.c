/*
* sampling.c
*
* Created: 2018-04-03 11:46:58
*  Author: Daniel
*/

#include <inttypes.h>
#include <asf.h>
#include <string.h>
#include <float.h>
#include <stdio.h>
#include "sampling.h"
#include "amplitude_trigger.h"
#include "dec_string.h"
#include "adc_with_pdc.h"
#include "configure_tc.h"
#include "dec_string.h"
#include "tdoa.h"

static float get_average_dcOffset(int16_t *buffer_array);
static float zero_crossings(uint32_t dc_offset, int index_sample_high, int index_sample_low, int16_t * buffer_array);
static void determine_frequency(int startIndex, int endIndex, float *frequency, int16_t *buffer_array);
static void determine_sdoa(int16_t * buffer_arary);

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

static void determine_frequency(int startIndex, int endIndex, float *frequency, int16_t *buffer_array)
{
	uint32_t dc_offset = (uint32_t)get_average_dcOffset(buffer_array);
	int k = 0;
	int zeros = 0;
	float zeros_array[21];
	for (int i = startIndex; i < endIndex; i++)
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
					float period = (float)(zeros_array[20]-zeros_array[0])/10; //average over 15 cycles/periods
					*frequency = (float)(1/period);
					break; //terminates for-loop
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
					float period = (float)(zeros_array[20]-zeros_array[0])/10; //average over 10 cycles/periods
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

static void determine_sdoa(int16_t * buffer_arary)
{
	static int flag_beacon_1, flag_beacon_2, flag_beacon_3, flag_beacon_4 = 0; 
	static int startIndex_40kHz, startIndex_50kHz, startIndex_60kHz, startIndex_55kHz = 0; //l, j, k, i
	static int startIndex = 0;
	for (int i = 0; i < ADC_BUFFER_SIZE; i++) //iterate through buffer and determine frequency, index by index
	{
		startIndex = i;
		float frequency = 0;
		determine_frequency(startIndex, ADC_BUFFER_SIZE, &frequency, buffer_arary);
		if ((flag_beacon_4 == 0) && (frequency >= 35500 && frequency <= 42500))
		{
			flag_beacon_4 = 1;
			startIndex_40kHz = startIndex;
			printf("Frequency: %s Hz, index = %i \n", get_decimal_string(frequency), startIndex);
		}
		else if ((flag_beacon_2 == 0) && (frequency > 47500 && frequency <= 52500))
		{
			flag_beacon_2 = 1;
			startIndex_50kHz = startIndex;
			printf("Frequency: %s Hz, index = %i \n", get_decimal_string(frequency), startIndex);
		}
		else if ((flag_beacon_3 == 0) && (frequency > 57500 && frequency <= 67500))
		{
			flag_beacon_3 = 1;
			startIndex_60kHz = startIndex;
			printf("Frequency: %s Hz, index = %i \n", get_decimal_string(frequency), startIndex);
		} 
		else if((flag_beacon_1 == 0) && (frequency > 52500 && frequency <= 57500))
		{
			flag_beacon_1 = 1;
			startIndex_55kHz = startIndex;
			printf("Frequency: %s Hz, index = %i \n", get_decimal_string(frequency), startIndex);
		}
	}
	//if all frequencies have been found
	if (flag_beacon_1 && flag_beacon_2 && flag_beacon_3 && flag_beacon_4)
	{
		struct SDOA sdoa_t;
		sdoa_t.sij = startIndex_55kHz-startIndex_50kHz;
		sdoa_t.sik = startIndex_55kHz-startIndex_60kHz;
		sdoa_t.skj = startIndex_60kHz-startIndex_50kHz;
		sdoa_t.skl = startIndex_60kHz-startIndex_40kHz;
		//set_sdoa(); //this should do everything, calculate the positions
		printf("sij: %i \n", sdoa_t.sij);
		printf("sik: %i \n", sdoa_t.sik);
		printf("skj: %i \n", sdoa_t.skj);
		printf("skl: %i \n", sdoa_t.skl);
		set_sdoa(startIndex_55kHz, startIndex_50kHz, startIndex_60kHz, startIndex_40kHz);
		double x, y, z = 0;
		calculate_position(&x, &y, &z);
		printf("x-coord: %s", get_decimal_string(x));
		printf("y-coord: %s", get_decimal_string(y));
		printf("z-coord: %s", get_decimal_string(z));
		flag_beacon_1, flag_beacon_2, flag_beacon_3, flag_beacon_4 = 0;
	}else{
		puts("SH*T don't work");
	}
}


/**
*  Interrupt handler for TC0 interrupt. Timer frequency corresponds to sample frequency.
*  Take one sample of input signal every time routine runs.
*/
void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;
	static int i = 0;
	
	/* Clear status bit to acknowledge interrupt */
	ul_dummy = tc_get_status(TC0, 0);	//The compare bit is cleared by reading the register, manual p. 915
	/* Avoid compiler warning */
	UNUSED(ul_dummy);
	
	/* Start sampling (i.e. convert one sample value) */
	adc_start(ADC);
	//while((adc_get_status(ADC) & 0x1<<24)==0);  //Wait until DRDY get high
	
	uint32_t invalue = adc_get_latest_value(ADC);
	
	adc_buffer_sample_values[i] = invalue;
	if (i >= ADC_BUFFER_SIZE)
	{
		tc_stop(TC0, 0);
		i = 0;
		/*
		float frequency = 0;
		determine_frequency(0, ADC_BUFFER_SIZE, &frequency, adc_buffer_sample_values);
		printf("Frequency: %s Hz\n", get_decimal_string(frequency));
		*/
		determine_sdoa(adc_buffer_sample_values);
		memset((void *)&adc_buffer_sample_values, 0, sizeof(adc_buffer_sample_values));	// clear sample buffer
		//tc_start(TC0,0);
		pio_enable_interrupt(PIOB, PIO_PB26); //re-enable amplitude-level detection
	}else{
		dacc_write_conversion_data(DACC, invalue);
		i++;
	}
}