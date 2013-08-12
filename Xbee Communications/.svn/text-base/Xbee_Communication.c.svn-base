#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <MSOE/bit.c>
#include <MSOE/lcd.c>
#include <MSOE/delay.c>
#include <stdio.h>

#define _PRICE 'P' //price packet ID
#define _RELAY 'R'//relay packet ID
#define _DATA 'D'//Data Packet ID
#define _ThisOutlet '1'	//outlet1
#define _RequestChar 'Q'	//request

void USART_init(unsigned int baud);
unsigned char USART_rx(void);
void USART_tx(unsigned char data);

float price = 0.0;
unsigned char OnOff = '0';
unsigned char priceCharArray[5] = {0};
unsigned char incomingPacket[9] = {0};
unsigned int wirelessNew = 0;
unsigned int _TransmitDelay = 500;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(USART_RXC_vect)
{
	cli();											//turn off interrupts so we can receive the whole packet

	incomingPacket[0] = UDR;

	for(uint8_t k = 1; k<9; k++)
		incomingPacket[k] = USART_rx();
	wirelessNew = 1;
	sei();
}

int main(void)				//Main program start
{
	DDRB = 0xFF;			//PORTB output for flasing led test
	lcd_init();				//initialize lcd
	lcd_clear();
	lcd_home();
	USART_init(51);			//usart initialize with 9600baud
	sei();					//enables all interrups	
		
	while(1)
	{
		char transmitPriceReply[9];
		if(wirelessNew=1)
		{
			switch (incomingPacket[0])
			{
				case _DATA:
					if(_ThisOutlet == incomingPacket[1])	//if we are the WHO
					{
						if(_RequestChar == incomingPacket[2])	//if REQUEST (should always be true)
						{
							lcd_print_string("Data");
							USART_tx(_DATA);//reply data
							//delay_ms(_TransmitDelay);
							USART_tx(_ThisOutlet);//who
							for (uint8_t k = 0; k<7; k++)
							{
							//	delay_ms(_TransmitDelay);
								USART_tx(k+48);	//transmits wattage data (eventually)
							}//for
						}//if
					}//if
					break;

				case _RELAY:
					lcd_clear();
					lcd_home();
					if(_ThisOutlet == incomingPacket[1])	//if we are the WHO
					{
						lcd_print_string("relay read");
						//read button was pushed
						if(incomingPacket[2] == _RequestChar)	//if is a REQUEST
						{
							USART_tx(_RELAY);		//reply relay
						//	delay_ms(_TransmitDelay);
							USART_tx(_ThisOutlet);		//who
						//	delay_ms(_TransmitDelay);

							//need if(port b pin # is on or off)
								// then change OnOff accordingly
							USART_tx(OnOff);	//on/off current status
							for(uint8_t k = 0; k<6; k++)
								USART_tx('0');
						}//if
						//commit button was pushed
						else
						{
							OnOff = incomingPacket[2];
							lcd_print_char(OnOff);
							DDRB = OnOff;	//get on/off signal
							delay_ms(500);
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
						lcd_print_string("price ");
						for(uint8_t k = 0; k<5; k++)
							priceCharArray[k] = incomingPacket[k+1];
						
						
						transmitPriceReply[0] = 'P';
						transmitPriceReply[1] = '1';
						for(uint8_t k = 0; k<5; k++)
							transmitPriceReply[k+2] = priceCharArray[k];
						transmitPriceReply[7] = '0';
						transmitPriceReply[8] = '0';
						
						lcd_clear();
						lcd_home();
						for(uint8_t k = 0; k<9; k++)
							lcd_print_char(transmitPriceReply[k]);
						
						//delay_ms(2000);
					}//else broadcasted price
		
					break;

				default:
					break;
			}//end packetID switch
			wirelessNew = 0;		//sets that there is no New wireless packet
			for(uint8_t k = 0; k<9; k++)
				incomingPacket[k] = 0;	//clears packet so we know its fresh data
		}//if wirelessNew
	}//end while
}//end main

//////////////////////////////////////
void USART_init( unsigned int baud ){
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data,  1 stop bit */
	UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
	UCSRB |= (1 << RXCIE)|(1<<TXCIE);//enable the Recieve USART Interrupt
}//USART_Init
void USART_tx(unsigned char data)
{
	while(!(UCSRA & (1<<UDRE)))
		;
	UDR = data;
}
unsigned char USART_rx(void)
{
	while(!(UCSRA & (1<<RXC)))
		;
	return UDR;
}
