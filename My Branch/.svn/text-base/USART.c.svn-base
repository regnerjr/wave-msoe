#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>


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

void sendPowerUsage(float newAverage)
{
//	char *UsageString[7]  = {0};
	uint8_t digits[7] = {0};
	int e = log10(newAverage);

	uint8_t index = 0;
	for(int k = 3; k>-3 ; k--)
	{
		if(e == k)
		{
			digits[index] = newAverage/pow(10,e);
			e--;
		}
		else
			digits[index] = 0;
		index++;
	}

	for (uint8_t k = 0; k< 7; k++)
		USART_tx(digits[k]);
}
