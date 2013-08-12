#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <MSOE/bit.c>
#include <MSOE/lcd.c>
#include <MSOE/delay.c>

void initCounter (void);
void initADC (void);
void initiADC (void);
void init200iADC(void);         //MUX loading for 200X gain
void init10iADC(void);          //MUX loading for 10X gain
void init1iADC(void);           //MUX loading for 1X gain
void initmaxfinder(void);       //for finding the MAX value in the current waveforms
void initsumofpoweraverages(void);
void initgainstatemachine(void);  //subroutine to decide the propergain of the 75 samples

uint8_t gain = 1;    // sets the initial gain checked parameter to be 200X gain when the program starts
uint8_t v=0;   				//global variable for indexing voltage samples
uint8_t i=0;   				//global variable for indexing current samples

int vsamples[75]={0};   //initialize a vector 1 by 75 to store vlotage samples
int isamples[75]={0};   //initialize a vector 1 by 75 to store current samples
int isamplesorig[75]={0};  //for untouched samples straight from the ADC

float vcalc[75]={0};    //printing DC values into the ADC for voltage readings
float icalc[75]={0};    // printing DC values into the ADC for current readings
int printer;
int imax = 0;         //variable for a single average current sample
float iavg = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(ADC_vect)									   //ADC conversion complete interrupt 
{ 
	cli(); //turns off interrupts
    vsamples[v]= ADC;                           //store ADC sample into vsamples array
	ADCSRA &= ~(1<<ADEN);  //disables ADC
 	printer = ADC;

	initiADC();                                   //go to the current adc sampling function
	if (vsamples[v] > 512)                       //if sample taken is greater than 512 bits, do 2's compliment and make it negative
	{
		vsamples[v] = (~(vsamples[v] - 1)) & 0x03FF;               //2's compliment of voltage samples
		vcalc[v]= ((-(float)vsamples[v]*3.3/512));      //attempted algorithm for converting the adc values into readable values
	    vcalc[v]= ((float)(vcalc[v]+2.5));   //trendline equation for voltage
	}// end if
	else										         // otherwise take sample as it normally appears
	{	
	    vcalc[v]= (((float)vsamples[v]*3.3/512));        // algorithm from datasheet to convert from ADC value into a human accepted voltage level
        vcalc[v]= ((float)(vcalc[v]+2.5));    //trendline equation
	}//end else
	
	if(v<74)               		//if number of samples is less than 75
		v++;            		//increment the pointer in the array 
	else
		v=0;                    //otherwise set the pointer back to 0.

	
	initADC();                  //re-initialize the voltage ADC for taking voltage values again
	sei();	

	
}//end ADC ISR

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_COMP_vect)       //timer counter compare match vector
{
	ADCSRA |= (1<<ADSC);    //**Note this is now in ADCSRA initialization
	TCNT0 = 0x00;	        //resets the timer to start counting from zero
	TIFR |=(1<<OCF0);       //clears the interrupt flag	
	
}//end Timer counter ISR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)     		//Main program start
{
    DDRB = 0xFF;     	//PORTB output for flasing led test
	lcd_init(); 		//initialize lcd
	initCounter();	    //initialize counter
    initADC();			//initialize ADC
	sei();				//enables all interrups
		
	while(1)
	{
		lcd_home();
		//lcd_printf("%i",printer);
		//lcd_goto_xy(0,7);
		lcd_print_float(vcalc[v]);
		lcd_goto_xy(1,1);
		lcd_print_float(icalc[i]);                         //prints average value into ADC for current reading purposes
		delay_ms(50);
	}//end while
}//end main
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initCounter(void)           // Setup for Timer Counter registers
{
		TCCR0 = 0b00010011;      //Normal Mode, Toggle OCO on Compare Match, Clk Prescaller 64
		TIMSK |= (1<<OCIE0);     //Enables Compare Match Interrupts
    	OCR0 = 0b01010010;       // 82 gives 667us between periods 
}//end Timer counter subroutine
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initADC(void)                                              //subroutine intializing ADC registers for voltage samples
{
	ADMUX = 0x00;
	ADCSRA = 0x00;
	ADMUX |= (1<<REFS0) | (1<<MUX4) | (1<<MUX3);                //setting up the admux for the "voltmeter" (AVCC reference w/ ext. cap. , ADCH most sig. bit , 1X Gain, diff. input for ADC0 input ADC2 ref.)
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1);  //initializing the ADC status register, ADEN - adc enable,also adjusting prescaler to 64
}//end ADC subroutine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initiADC(void)                  //subroutine for setting up ADC registers for current samples
{	
	if(gain == 200)   //changed frrom 200 to 10              //if gain is determined to be x200
	{
		init200iADC();              //set up ADMUX for x200 changed to 10iadc
	}	
	else if (gain == 10)                  //if gain is determined to be x10
	{
		init10iADC();               //set up ADMUX for x10
	}
    else if(gain == 1)                    //if gain is determined to be x1
	{
		init1iADC();                  //set up gain to be x1
	}     
	ADCSRA |= (1<<ADEN);        //re enable ADC
	ADCSRA |= (1<<ADSC);                                //ADC start conversion
	while(ADCSRA & (1 << ADSC))                         //waits until the conversion is completed before it continues and saves the value.
		; 
	isamples[i]= ADC;
	ADCSRA &= ~(1<<ADEN);  //disables ADC                                         //stores the current 'i' sample from the ADC
	    

		

	if (isamples[i] > 512)                         //if sample taken is greater than 512 bits, do 2's compliment and make it negative
	{
		isamples[i] = (~(isamples[i] - 1)) & 0x03FF;               //2's compliment of current samples
		icalc[i]= ((-((float)isamples[i]*3.3/(gain*512)))+2.5);      //attempted algorithm for converting the adc values into readable values		

	}// end if
	else										            // otherwise take sample as it normally appears
	{	
	    icalc[i] = ((((float)isamples[i]*3.3/(gain*512)))+2.5);        // algorithm from datasheet to convert from ADC value into a human accepted voltage level
	}//end else		
	if(i<74)                   //if number of samples is less than 74
		i++;                   //increment the pointer in the array 
	else
	{	
		i=0;                   //otherwise set the pointer back to 0.
		initmaxfinder();          //go to averaging function to get a single average of current
		initgainstatemachine();    //go to state machine that will decide the proper gain of the 75 samples and set it for 1,2 or 3 depending on the the 
	
	}
	initADC(); 
     
}//end current sampling ADC subroutine

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init200iADC(void)
{
	ADMUX = 0x40;                 //clear admux keeping REFS0 high
	ADMUX |= (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0); //setup ADMUX for x200 gain 01111
}//end x200 ADC setup
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init10iADC(void)
{
	ADMUX = 0x40;             //clear admux keeping REFS0 high
	ADMUX |= (1<<MUX3) | (1<<MUX2) | (1<<MUX0);            //setup ADMUX for x10 gain 01101
}//end x10 ADC setup
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init1iADC(void)
{
	ADMUX = 0x40;                      //clear admux keeping REFS0 high
	ADMUX |= (1<<MUX4) | (1<<MUX3) | (1<<MUX1) | (1<<MUX0);      //setup admux for x1 gain 11011          
}//end x1 ADC setup
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initgainstatemachine(void)
{
	if((gain == 200) && (imax >= 479))  //**changed 200 to 10 //if gain is at x200 and value is overflowing OR gain is x1 and value is too small
	{
		gain = 10;                                                                     //use x10 gain setting
	}//if
	else if((gain == 1) && (imax <= 49))
	{
		gain =10; 
	}//if
	else if ((gain == 10) && (imax <= 24))          //if gain is x10 and value is too small
	{
		gain = 200;           //**changed 200 to 10    //use x200 gain setting changed from 200 to 10 (may revmove 10 gain)
	}//if
	
	else if((gain == 10) && (imax >= 480))           //if gain is x10 and value is too large
	{
		gain = 1;                                     // use x1 gain setting
	}//if	
}//end gain state machine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initmaxfinder(void)               //subroutine for finding the maximum value of the top of the 3 periods
{
	imax = 0;                          //this subroutine executes every 3 periods and clears the maximum every time
	for(uint8_t k=0; k<74; k++)
	{
		if (imax < isamples[k])       //if current maximum is less than new maximum
		{
			imax = isamples[k];        //save the new maximum sample
		}//if
	}//for
}//max finder function
