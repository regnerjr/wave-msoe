#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>




void ConvertCurrentSamplesToAmps(int *inputSample, float *amps, uint8_t gain)
{
	for(uint8_t k = 0 ; k<75 ; k++)
	{
		if (inputSample[k] > 512)									//if sample taken is greater than 512 bits, do 2's compliment and make it negative
		{
			int negative = (~(inputSample[k] - 1)) & 0x03FF;		//2's compliment of current samples
			inputSample[k] = -negative;
		}// end if

		
		if(gain == 10 || gain == 200)
			amps[k] = 0.00242*inputSample[k]-0.044699;
		else
			amps[k] = 0.084236*inputSample[k]- 0.368884;
	}//for

	//float dcOffset = 0.0;
	//for(int k = 0; k<75; k++)
	//	dcOffset += amps[k];	
	//dcOffset = dcOffset/75;

	//for(int k = 0; k<75; k++)
	//	amps[k] = amps[k] - dcOffset;
}

void FindNewGainValue(uint8_t* gain, int imax)
{
	if((*gain == 200) && (imax >= 479))		//if gain is at x200 and value is overflowing OR gain is x1 and value is too small
		*gain = 10;							//use x10 gain setting
	else if((*gain == 1) && (imax <= 49))
		*gain =10; 
	else if ((*gain == 10) && (imax <= 24))	//if gain is x10 and value is too small
		*gain = 200;							//use x200 gain setting changed from 200 to 10 (may revmove 10 gain)
	else if((*gain == 10) && (imax >= 480))	//if gain is x10 and value is too large
		*gain = 1;							// use x1 gain setting
}//end gain state machine



int TakeCurrentSample(void)
{
	ADCSRA |= (1<<ADEN);								//re enable ADC
	ADCSRA |= (1<<ADSC);								//ADC start conversion
	while(ADCSRA & (1 << ADSC))							//waits until the conversion is completed before it continues and saves the value.
		;
	int newCurrentSample= ADC;							//stores the current 'i' sample from the ADC
	return newCurrentSample;
}



int FindMaxCurrentSample(int *currentSampleArray)					//subroutine for finding the maximum value of the top of the 3 periods
{
	int imax = 0;								//this subroutine executes every 3 periods and clears the maximum every time
	for(uint8_t k=0; k<74; k++)
		if (imax < currentSampleArray[k])				//if current maximum is less than new maximum
			imax = currentSampleArray[k];				//save the new maximum sample
	return imax;
}//max finder function



void SetADCMUX200GainCurrent(void)
{
	ADMUX = 0x40;											//clear admux keeping REFS0 high
	ADMUX |= (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);	//setup ADMUX for x200 gain 01111
}//end x200 ADC setup



void SetADCMUX10GainCurrent(void)
{
	ADMUX = 0x40;											//clear admux keeping REFS0 high
    ADMUX |= (1<<MUX3) | (1<<MUX2) | (1<<MUX0);				//setup ADMUX for x10 gain 01101
}//end x10 ADC setup


void SetADCMUX1GainCurrent(void)
{
	ADMUX = 0x40;											//clear admux keeping REFS0 high
	ADMUX |= (1<<MUX4) | (1<<MUX3) | (1<<MUX1) | (1<<MUX0);	//setup admux for x1 gain 11011
}//end x1 ADC setup

void InitCurrentADC(uint8_t gain)				//subroutine for setting up ADC registers for current samples
{
	if(gain == 200)								//if gain is determined to be x200
		SetADCMUX200GainCurrent();				//set up ADMUX for x200 changed to 10iadc
	else if (gain == 10)						//if gain is determined to be x10
		SetADCMUX10GainCurrent();				//set up ADMUX for x10
	else if(gain == 1)							//if gain is determined to be x1
		SetADCMUX1GainCurrent();				//set up gain to be x1

}//end current sampling ADC subroutine
