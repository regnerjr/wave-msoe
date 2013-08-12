#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <MSOE/bit.c>
#include <MSOE/lcd.c>
#include <MSOE/delay.c>

#include "Current.h"
#include "Voltage.h"
#include "Counters.h"
#include "USART.h"

#define numberOfSetsInMovingAvgPower 30
#define DelayBetweenSampleGroupsSeconds  10

//***Function Definitions***//
float calculateavgpower(void);	//subroutine to do anything involving the power calculations
float movingAveragePower(float averagePower);

//***Global Variables***//
uint8_t gain = 1;				//sets the initial gain checked parameter to be 200X gain when the program starts
uint8_t v = 0;					//global variable for indexing voltage samples
uint8_t i = 0;					//global variable for indexing current samples

int vsamples[75] = {0};			//initialize a vector 1 by 75 to store vlotage samples
int isamples[75] = {0};			//initialize a vector 1 by 75 to store current samples
float vline[75] = {0};			// for calculating line voltage using the voltmeter (analog circuit) trendline
float iload[75] = {0};

float averagePowerArray[numberOfSetsInMovingAvgPower] = {0};

float price = 0.0;
float powerUsage = 0.0;
unsigned char OnOff = '0';
unsigned char priceCharArray[5] = {'0','0','0','0','0'};
unsigned char incomingPacket[9] = {'0','0','0','0','0','0','0','0','0'};
unsigned int wirelessNew = 0;



ISR(USART_RXC_vect)
{	
	cli();											//turn off interrupts so we can receive the whole packet
	incomingPacket[0] = UDR;
	for(uint8_t k = 1; k<9; k++)
		incomingPacket[k] = USART_rx();
	wirelessNew = 1;
	sei();
}

ISR(ADC_vect)									//ADC conversion complete interrupt 
{
	cli();										//turns off interrupts
	vsamples[v]= ADC;							//store ADC sample into vsamples array
	ADCSRA &= ~(1<<ADEN);						//disables ADC

	InitCurrentADC(gain);						//Sets up the ADC for Taking Current Samples
	isamples[i] = TakeCurrentSample();
	ADCSRA &= ~(1<<ADEN);						//disables ADC
	if (isamples[i] > 512)									//if sample taken is greater than 512 bits, do 2's compliment and make it negative
	{
		int negative = (~(isamples[i] - 1)) & 0x03FF;		//2's compliment of current samples
		isamples[i] = -negative;
	}// end if		

	initVoltageADC();							//initialize ADC
	ADCSRA &= ~(1<<ADEN);
	v++;										//increment the sample indices
	i++;
	sei();
}//end ADC ISR

ISR(TIMER0_COMP_vect)		//timer counter compare match vector
{
	TCNT0 = 0x00;			//resets the timer to start counting from zero
	TIFR |=(1<<OCF0);		//clears the interrupt flag
	//this flag was not needed in the branch
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);	//**Note this is now in ADCSRA initialization
}//end Timer counter ISR



int main(void)				//Main program start
{
	DDRB = 0xFF;			//PORTB output for flasing led test
	PORTB |= 0x01;			//turns the outlet on initially
	lcd_init();				//initialize lcd
	initCounter();			//initialize counter
	initVoltageADC();		//initialize ADC
	USART_init(51);			//usart initialize with 9600 baud
	sei();					//enables all interrups
	uint8_t numberOfSetsTaken = 0;
	float newAverage = 0;
	float newAveragePricePerMonth;
	float currentsum;
	int newCurrentSampleMax;
	
	while(1)
	{
		
		if( numberOfSetsTaken < numberOfSetsInMovingAvgPower)
		{
			//waiting for 75 samples
			if(v == 75)
			{
				TIMSK &= ~(1<<OCIE0);
				
				
				newCurrentSampleMax = FindMaxCurrentSample(isamples);		//go to averaging function to get a single average of current
				FindNewGainValue(&gain, newCurrentSampleMax);					//Sets new Gain Value 1, 10 or 200, based on 75 previous Current Samples 
				
				ConvertCurrentSamplesToAmps(isamples, iload, gain);
				ConvertVoltageSamplesToVolts(vsamples, vline);
			
				powerUsage = calculateavgpower();
				//newAverage = powerUsage;

				for(int k = 0; k<75; k++)
					iload[k] = iload[k]*iload[k];

				for(int k = 0; k<75; k++)
					currentsum += iload[k];

				currentsum = sqrt(currentsum/75);
				newAverage = movingAveragePower(powerUsage);
				
				if(newAverage < 0)
					newAverage = 0.0;
				
				float price = 0.0;
				price = (priceCharArray[0]-48)*0.1+
					(priceCharArray[1]-48)*0.01+
					(priceCharArray[2]-48)*0.001+
					(priceCharArray[3]-48)*0.0001+
					(priceCharArray[4]-48)*0.00001;
				newAveragePricePerMonth = newAverage/1000*720*price;

				for (uint8_t k = 0; k < 75; k++)
				{
					vline[k]=0;			//return all arrays to zero
					iload[k]=0;
					vsamples[k]=0;
					isamples[k]=0;
				}
				
				v = 0;      //reset sample indices to zero
				i = 0;
				numberOfSetsTaken++;
				TCNT0 = 0x00;			//resets the timer to start counting from zero
				TIMSK |= (1<<OCIE0);		//turns time counter back on
			
			}//if(v==74)
		}
		else
		{
			TIMSK &= ~(1<<OCIE0); 
			lcd_clear();
			lcd_home();
			lcd_print_int16((int)newAverage);
			lcd_print_string(" Watts");
			lcd_goto_xy(0,1);
			lcd_print_string("$");
			lcd_print_float(newAveragePricePerMonth);
			lcd_print_string("/mo");

			for(int delay = 0 ; delay <= DelayBetweenSampleGroupsSeconds ; delay++)
			{
				delay_ms(1000);
			 	delay++;
				//***** Wireless Data Being Handled *****
				if(wirelessNew == 1)
				{
					HandleWirelessData(incomingPacket, priceCharArray, OnOff, newAverage);
					wirelessNew = 0;		//sets that there is no New wireless packet
				}//if wirelessNew
			}//for delay		
			
			numberOfSetsTaken = 0;
			TCNT0 = 0x00;			//resets the timer to start counting from zero
			TIMSK |= (1<<OCIE0);		//turns time counter back on
		}//end else
	}//end while
}//end main


float calculateavgpower(void)
{
	float avgpower=0;
	float instPower[75] = {0};

	for(int k=0; k<75; k++)
		instPower[k] = (vline[k]*iload[k]);
	
	for(int k=0; k<75; k++)
		avgpower += instPower[k]; 		//add the divided samples 
	
	avgpower = avgpower/75.0;
	
	return avgpower;
}

float movingAveragePower(float averagePower)
{
	for(uint8_t k = numberOfSetsInMovingAvgPower; k>1 ; k--)
		averagePowerArray[k-1] = averagePowerArray[k-2];
	
	averagePowerArray[0] = averagePower;
	
	float sumOfAverages = 0;
	for(uint8_t k = 0; k <numberOfSetsInMovingAvgPower ; k++)
	{
		sumOfAverages +=averagePowerArray[k];
	}

	sumOfAverages = sumOfAverages/(float)numberOfSetsInMovingAvgPower;

	return sumOfAverages;
}
