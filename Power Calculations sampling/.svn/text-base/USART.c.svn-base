#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "WAVEStringLibrary.h"

#define _PRICE 'P' //price packet ID
#define _RELAY 'R'//relay packet ID
#define _DATA 'D'//Data Packet ID
#define _ThisOutlet '2'	//outlet1
#define _RequestChar 'Q'	//request

void USART_init( unsigned int baud ){
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data,  1 stop bit */
	UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
	UCSRB |= (1 << RXCIE);//enable the Recieve USART Interrupt
}//USART_Init


void USART_tx(unsigned char data){
	while(!(UCSRA & (1<<UDRE)))
		;
	UDR = data;
}


unsigned char USART_rx(void){
	while(!(UCSRA & (1<<RXC)))
		;
	return UDR;
}

void sendPowerUsage(float newAverage)
{
	char convertedFloatToString[7] = {'0'};

	if(newAverage < 0)
		newAverage = 0.0;

	floatToCharArray(newAverage, convertedFloatToString);

	for(int k=0; k<7; k++)
	{
		USART_tx(convertedFloatToString[k]);
	}
	return;
}

void HandleWirelessData(unsigned char *incomingPacket, unsigned char *priceCharArray, unsigned char OnOff,float newAverage)
{
	
	char transmitPriceReply[9];
	TIMSK &= ~(1<<OCIE0);//turns off the counter while transmitting wireless data 
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
					if(PORTB & 0b00000001)
						OnOff = '1';
					else
						OnOff = '0';
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
					if (OnOff == '1')
						PORTB |= 0x01;
					else
						PORTB &= ~0x01;
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
				for(uint8_t k = 0; k<5; k++){
					priceCharArray[k] = incomingPacket[k+1];
				}
					
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
	for(uint8_t k = 0; k<9; k++)
		incomingPacket[k] = 0;	//clears packet so we know its fresh data
	TCNT0 = 0x00;			//resets the timer to start counting from zero
	TIMSK |= (1<<OCIE0);		//turns time counter back on
	return;
}
