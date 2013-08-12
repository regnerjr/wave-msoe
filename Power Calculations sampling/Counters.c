#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>


void initCounter(void)		// Setup for Timer Counter registers
{
	TCCR0 |= (1<<COM00)|(1<<CS00)|(1<<CS01);	//Normal Mode, Toggle OCO on Compare Match, Clk Prescaller 64
	TIMSK |= (1<<OCIE0);	//Enables Compare Match Interrupts
	TIMSK &= ~(1<<TOIE0);
	OCR0 = 0b01010010;		// 82 gives 667us between periods 
}//end Timer counter subroutine
