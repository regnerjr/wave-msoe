#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <MSOE/bit.c>
#include <MSOE/lcd.c>
#include <MSOE/delay.c>

float instpower[75]={.9,.9,.9,.9,.9,.9,.9,.9,.9,.9,//10
					.9,.9,.9,.9,.9,.9,.9,.9,.9,.9,//20
					.9,.9,.9,.9,.9,.9,.9,.9,.9,.9,//30
					.9,.9,.9,.9,.9,.9,.9,.9,.9,.9,//40
					.9,.9,.9,.9,.9,.9,.9,.9,.9,.9,//50
					.9,.9,.9,.9,.9,.9,.9,.9,.9,.9,//60
					.9,.9,.9,.9,.9,.9,.9,.9,.9,.9,//70
					.9,.9,.9,.9,.9};//75
float avgpower = 0;

void calculateavgpower(void);

int main(void)
{
	lcd_init();
	lcd_home();

	while(1)
	{
	
		calculateavgpower();
		lcd_print_float(avgpower);
		delay_ms(5000);
		lcd_clear();
	}

}

void calculateavgpower(void)
{
	avgpower=0;
  	for(int k=0; k<75; k++)
	    	avgpower += instpower[k]/75.0; 		//add the divided samples 
      
	 // avgpower = avgpower/75.0;
}//calcualte power subroutine
