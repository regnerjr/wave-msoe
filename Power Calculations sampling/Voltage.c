#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>


void initVoltageADC(void)										//subroutine intializing ADC registers for voltage samples
{
	ADMUX = 0x00;
	ADCSRA = 0x00;

	ADMUX |= (1<<REFS0) | (1<<MUX4) | (1<<MUX3);				//setting up the admux for the "voltmeter" (AVCC reference w/ ext. cap. , ADCH most sig. bit , 1X Gain, diff. input for ADC0 input ADC2 ref.)
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS0);	//initializing the ADC status register, ADEN - adc enable,also adjusting prescaler to 32
}//end ADC subroutine


void ConvertVoltageSamplesToVolts(int *inputSample, float *lineVoltage)
{
	for(uint8_t k = 0 ; k<75; k++)
	{	
		if (inputSample[k] > 512)			//if sample taken is greater than 512 bits, do 2's compliment and make it negative
		{
			inputSample[k] = (~((inputSample[k]) - 1)) & 0x03FF;		//2's compliment of voltage samples		
			inputSample[k] = -inputSample[k];
		}

		lineVoltage[k] = 0.797*inputSample[k]-0.6369;
	}//for loop
	return;
}
