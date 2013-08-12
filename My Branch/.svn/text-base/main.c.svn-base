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

#define _PRICE 'P' //price packet ID
#define _RELAY 'R'//relay packet ID
#define _DATA 'D'//Data Packet ID
#define _ThisOutlet '1'	//outlet1
#define _RequestChar 'Q'	//request

#define numberOfSetsInMovingAvgPower 100

//***Function Definitions***//
float calculateavgpower(void);	//subroutine to do anything involving the power calculations
float movingAveragePower(float averagePower);
void TransmitAllSamples(int* sampleArray);
void TransmitConvertedValues(float *ConvertedSampleArray);

//***Global Variables***//
uint8_t gain = 1;				//sets the initial gain checked parameter to be 200X gain when the program starts
uint8_t v = 0;					//global variable for indexing voltage samples
uint8_t i = 0;					//global variable for indexing current samples

int vsamples[90] = {0};			//initialize a vector 1 by 75 to store vlotage samples
int isamples[90] = {0};			//initialize a vector 1 by 75 to store current samples
float vline[75] = {0};			// for calculating line voltage using the voltmeter (analog circuit) trendline
float iload[75] = {0};

float averagePowerArray[numberOfSetsInMovingAvgPower] = {0};

float price = 0.0;
float powerUsage = 0.0;
//unsigned char who = 0;
unsigned char OnOff = 0;
unsigned char priceCharArray[5] = {0};
unsigned char incomingPacket[9] = {0};
unsigned int wirelessNew = 0;
unsigned int _TransmitDelay = 500;


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
	PORTB |= 0x04;
	cli();										//turns off interrupts
	
	vsamples[v] = ADC;

	ADCSRA &= ~(1<<ADEN);						//disables ADC
	
	InitCurrentADC(gain);						//Sets up the ADC for Taking Current Samples
	isamples[i] = TakeCurrentSample();
	ADCSRA &= ~(1<<ADEN);						//disables ADC
	
	initVoltageADC();							//initialize ADC
	//ADCSRA &= ~(1<<ADEN);
	v++;										//increment the sample indices
	i++;	
	
	sei();
	PORTB &= ~0x04;
	
}//end ADC ISR

ISR(TIMER0_COMP_vect)		//timer counter compare match vector
{
	TCNT0 = 0x00;			//resets the timer to start counting from zero
	//TIFR |=(1<<OCF0);		//clears the interrupt flag
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);	//**Note this is now in ADCSRA initialization
}//end Timer counter ISR



int main(void)				//Main program start
{
	DDRB = 0xFF;			//PORTB output for flasing led test
	lcd_init();				//initialize lcd
	initCounter();			//initialize counter
	initVoltageADC();		//initialize ADC
	USART_init(51);			//usart initialize with 9600baud
	sei();					//enables all interrups
	
	uint8_t numberOfSetsTaken = 0;
	float newAverage = 0;
	
	while(1)
	{	
		if( numberOfSetsTaken < numberOfSetsInMovingAvgPower)
		{
			
			//waiting for 75 samples
			
			if(v == 75)
			{
				
				TIMSK &= ~(1<<OCIE0);

				
				//TransmitAllSamples(isamples);
				//USART_tx('I');
				TransmitAllSamples(vsamples);
				USART_tx('V');
				
				int newCurrentSampleMax = FindMaxCurrentSample(isamples);		//go to averaging function to get a single average of current
				FindNewGainValue(&gain, newCurrentSampleMax);					//Sets new Gain Value 1, 10 or 200, based on 75 previous Current Samples 
					

				ConvertCurrentSamplesToAmps(isamples, iload, gain);
				ConvertVoltageSamplesToVolts(vsamples, vline);

				//TransmitConvertedValues(iload);
				//USART_tx('i');
				TransmitConvertedValues(vline);
				USART_tx('v');


				powerUsage = calculateavgpower();
				newAverage = movingAveragePower(powerUsage);
				
				for (uint8_t k =0; k<75; k++)
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
				TIMSK |= (1<<OCIE0);

			}//if(v==74)
		}
		else
		{
			TIMSK &= ~(1<<OCIE0);
			lcd_home();
			lcd_clear();
			lcd_print_float(newAverage);
			delay_ms(5000);
			numberOfSetsTaken = 0;
			TCNT0 = 0x00;			//resets the timer to start counting from zero
			TIMSK |= (1<<OCIE0);		//turns time counter back on
		}
		
		
		
		//***** Wireless Data Being Handled *****
		char transmitPriceReply[9];
		if(wirelessNew == 1)
		{
			TIMSK &= ~(1<<OCIE0);
			switch (incomingPacket[0])
			{
				case _DATA:
					if(_ThisOutlet == incomingPacket[1])	//if we are the WHO
					{
						if(_RequestChar == incomingPacket[2])	//if REQUEST (should always be true)
						{
							USART_tx(_DATA);//reply data
							USART_tx(_ThisOutlet);//who
							sendPowerUsage(newAverage);
						}//if
					}//if
					break;

				case _RELAY:
					if(_ThisOutlet == incomingPacket[1])	//if we are the WHO
					{
						//read button was pushed
						if(incomingPacket[2] == _RequestChar)	//if is a REQUEST
						{
							USART_tx(_RELAY);		//reply relay
							USART_tx(_ThisOutlet);		//who
							USART_tx(OnOff);	//on/off current status
							
							for(uint8_t k = 0; k<6; k++)
								USART_tx('0');
						}//if
						//commit button was pushed
						else
						{
							OnOff = incomingPacket[2];
						}//else
					}//if
					break;

				case _PRICE:
					if(incomingPacket[1] == _RequestChar)	//if it is a REQUEST
					{
						if(_ThisOutlet == incomingPacket[2])	//if we are the WHO
						{
							for(uint8_t k = 0; k<9; k++)
								USART_tx(transmitPriceReply[k]);
						}//if REQUEST
					}//if WHO
		
					//if the second char received isn't a REQUEST, it is
					//automatically defined as a broadcasted price ->save it
					else
					{
						for(uint8_t k = 0; k<5; k++)
							priceCharArray[k] = incomingPacket[k+1];
						
						//set flag to say we have a full price at outlet
						transmitPriceReply[0] = 'P';
						transmitPriceReply[1] = '1';
						for(uint8_t k = 0; k<5; k++)
							transmitPriceReply[k+2] = priceCharArray[k];
						transmitPriceReply[7] = '0';
						transmitPriceReply[8] = '0';
					}//else broadcasted price
		
					break;

				default:
					break;
			}//end packetID switch
			wirelessNew = 0;		//sets that there is no New wireless packet
			for(uint8_t k = 0; k<9; k++)
				incomingPacket[k] = 0;	//clears packet so we know its fresh data

			TCNT0 = 0x00;			//resets the timer to start counting from zero
			TIMSK |= (1<<OCIE0);		//turns time counter back on
		}//if wirelessNew	
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
 
void TransmitAllSamples(int* sampleArray) 
{
	char sampleCharArray[5] = {0}; 	//biggest possible sample is 2^9 plus signed bit so -512 is 4 digits

	for (int k = 0; k<75; k++) 		//for each of the passed samples
	{
		if (sampleArray[k] > 512)									//if sample taken is greater than 512 bits, do 2's compliment and make it negative
		{
			int negative = (~(sampleArray[k] - 1)) & 0x03FF;		//2's compliment of current samples
			sampleArray[k] = -negative;
		}// end if


		itoa(sampleArray[k], sampleCharArray, 10); //converts sample from signed int to string

		for(int k = 0; k<5; k++)
			USART_tx(sampleCharArray[k]); //sends each bit of the sample
		USART_tx(',');
	}
	return;
}


void TransmitConvertedValues(float *ConvertedSampleArray)
{
	char ConvertedCharArray[9] = {0};// - 3digits . 3 digits

	for (int k = 0; k<75; k++) //for each of the passed samples
	{	//http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html#ga060c998e77fb5fc0d3168b3ce8771d42

		//		Value					, width	, precision , string
		dtostrf(ConvertedSampleArray[k], 5		,3			,ConvertedCharArray ); //converts float to string
		
		USART_tx(ConvertedCharArray[0]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[1]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[2]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[3]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[4]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[5]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[6]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[7]); //sends each bit of the sample
		USART_tx(ConvertedCharArray[8]); //sends each bit of the sample
		USART_tx(',');
	}

}
