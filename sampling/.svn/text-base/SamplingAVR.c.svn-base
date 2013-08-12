/*
Joel Mertes, John Regner, Jack Sobol & Aaron VandenHeuvel
Samples the ADC using an interrupt to measure instantaneous voltage and current 
these sample will be multiplied to find the instantaneous (real) power.
*/
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
void USART_Init(unsigned int);
void USART_Transmit( unsigned char);

int gain = 1;    // sets the initial gain checked parameter to be 200X gain when the program starts
int v=0;   				//global variable for indexing voltage samples
int i=0;   				//global variable for indexing current samples
int p=0;   				//global variable for indexing power samples
int k=0;
int vsamples[75]={0};   //initialize a vector 1 by 75 to store vlotage samples
int isamples[75]={0};   //initialize a vector 1 by 75 to store current samples
int isamplesorig[75]={0};  //for untouched samples straight from the ADC
int psamples[75]={0};   //initialize a vector 1 bt 75 to store the instantaneous power samples   
int pavg = 0;
float psamplediv75[75]={0}; 
float vcalc[75]={0};    //printing DC values into the ADC for voltage readings
float icalc[75]={0};    // printing DC values into the ADC for current readings
float vline[75]={0};
//float idevice[75]={0};
float iavg = 0;
int imax = 0;         //variable for a single average current sample

float somenewarray[20] = {0}; 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(ADC_vect)									   //ADC conversion complete interrupt 
{ 
  PORTB=0x00;				//turn off port b for simultaneus sampling 
 // ADCSRA &= ~(1<<ADEN);  //disables ADC
  //  lcd_printf("%i", ADC);						// prints number coming into the ADC
  //  lcd_goto_xy(0,1);
  	  vsamples[v]= ADC;                           //store ADC sample into vsamples array
     // PORTB=0xFF;               //flash port b when done to signal ADC sample taken and conversion complete.
//	  initiADC();                                   //go to the current adc sampling function
	if (vsamples[v] > 512)                       //if sample taken is greater than 512 bits, do 2's compliment and make it negative
	{
		vsamples[v] = (vsamples[v] - 1);               //2's compliment of voltage samples
		vsamples[v] = ~(vsamples[v]);	               // 2's compliment of voltage samples
		vsamples[v] = vsamples[v] & 0x03FF;            //and it with some zeros for removing extra bits out front
		vcalc[v]= ((-(float)vsamples[v]*3.3/512));      //attempted algorithm for converting the adc values into readable values
		vcalc[v]= ((float)(vcalc[v]+2.5+.1058)/1.034);


 
	}// end if
	else										         // otherwise take sample as it normally appears
	{	
	   vcalc[v]= (((float)vsamples[v]*3.3/512));        // algorithm from datasheet to convert from ADC value into a human accepted voltage level
	   vcalc[v]= ((float)(vcalc[v]+2.5+.1058)/1.034);
	//	lcd_print_float(vcalc[v]);                       //prints the positive adc samples
	//	lcd_printf(" [V]");
	//	delay_ms(100);
	//	lcd_clear();
	//	lcd_home();	
	}//end else
	vline[v] = 130.49*vcalc[v] + 1.7182;
	if(v<75)                   //if number of samples is less than 75
		v++;                   //increment the pointer in the array 
	else
		v=0;                   //otherwise set the pointer back to 0.
	
	sei();                         //re set global interrupts

}//end ADC ISR

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_COMP_vect)       //timer counter compare match vector
{
	ADCSRA |= (1<<ADSC);    //**Note this is now in ADCSRA initialization
	TCNT0 = 0x00;	        //resets the timer to start counting from zero
	TIFR |=(1<<OCF0);       //clears the interrupt flag	
//	PORTB= 0x00;
}//end Timer counter ISR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)         //Main program start
{
    DDRB = 0xFF;       //PORTB output for flasing led test
	lcd_init();        //initialize lcd
	initCounter();	    //initialize counter
    initADC();			//initialize ADC
	sei();//enables all interrups
		
	while(1)
	{
		for(int i = 48; i<57 ; i++ )
		{
		  //data[i] = USART_Receive();
		  //lcd_printf("%c", i);
			USART_Transmit(i);
		  //delay_ms(500);
		}//end for
		lcd_printf("%i", ADC);
		lcd_goto_xy(0,1);
  		lcd_print_float(vcalc[v]);
	    lcd_goto_xy(4,0);
	//	lcd_printf("%i", sizeof(uint16_t));
		delay_ms(100);

	//	lcd_printf("%i", isamples[i]);						   // prints number coming into the ADC	
	//	lcd_goto_xy(0,1);
	//	lcd_printf("%i" ,imax);
	//	lcd_goto_xy(6,0);
	//	lcd_printf("%i",gain);
	//	lcd_goto_xy(5,1);
	//	lcd_print_float( icalc[i]);
	//	delay_ms(50);
	
		lcd_clear();
		lcd_home();

		if( p < 75 )                     //checkig to  make sure power samples are less than 75 samples
		{
			//if( (v == 75) && (i == 75) )  //if both voltage and current have 75 samples taken
			//{
			
				psamples[p] = vsamples[v]*isamples[i];   //multiply the nth sample of each matrix (voltage/current) to obtain an "instantaneous power" matrix
				psamplediv75[p]=(float)psamples[p]/75;              //divides every current sample by 75 (in anticipation of averaging)	
			//}//if
			p++;
		}//if
		else
		{
			initsumofpoweraverages();							//calls the function to add all the 75 power samples together
			p=0;
			
		}//else
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
	//DDRA = 0x00;   //sets port A to input
	ADMUX |= (1<<REFS0) | (1<<MUX4) | (1<<MUX3);                //setting up the admux for the "voltmeter" (AVCC reference w/ ext. cap. , ADCH most sig. bit , 1X Gain, diff. input for ADC0 input ADC2 ref.)
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1);  //initializing the ADC status register, ADEN - adc enable,also adjusting prescaler to 64
}//end ADC subroutine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initiADC(void)                  //subroutine for setting up ADC registers for current samples
{
	cli();                          //turns off all interupts
	if(gain == 200)   //changed frrom 200 to 10              //if gain is determined to be x200
	{
		init200iADC();              //set up ADMUX for x200 changed to 10iadc
	}	
	if (gain == 10)                  //if gain is determined to be x10
	{
		init10iADC();               //set up ADMUX for x10
	}
    if(gain == 1)                    //if gain is determined to be x1
	{
		init1iADC();                  //set up gain to be x1
	}     
		ADCSRA |= (1<<ADEN);        //re enable ADC
		ADCSRA |= (1<<ADSC);                                //ADC start conversion
		while(ADCSRA & (1 << ADSC))                         //waits until the conversion is completed before it continues and saves the value.
		; 
		PORTB=0xFF;				//turn on LED port b for simultaneus sampling
		isamples[i]= ADC;                                         //stores the current 'i' sample from the ADC
	    
 if (isamples[i] > 512)                         //if sample taken is greater than 512 bits, do 2's compliment and make it negative
	{
		isamples[i] = (isamples[i] - 1);               //2's compliment of current samples
		isamples[i] = ~(isamples[i]);	               // 2's compliment of current samples
		isamples[i] = isamples[i] & 0x03FF;            //and it with some zeros for removing extra bits out front
		icalc[i]= (((float)isamples[i]*3.3/(gain*512)));      //attempted algorithm for converting the adc values into readable values
//		lcd_goto_xy(5,1);
//		lcd_print_float(-icalc[i]);                    //prints negative ADC samples
//		lcd_printf(" [V]");
//		delay_ms(50); 	
		
	}// end if
		else										            // otherwise take sample as it normally appears
	{	
//	    lcd_goto_xy(5,1);
	    icalc[i] = (((float)isamples[i]*3.3/(gain*512)));        // algorithm from datasheet to convert from ADC value into a human accepted voltage level
//		lcd_print_float(icalc[i]);                          //prints the positive adc samples
//		lcd_printf(" [V]");
//		delay_ms(50);		
	}//end else
//		lcd_home();
//		lcd_printf("%i", isamples[i]);						   // prints number coming into the ADC	
//		lcd_goto_xy(0,1);							  //move to new line on LCD
		
//				lcd_printf("%i" ,imax);            //printing imax
//				lcd_goto_xy(6,0);
//				lcd_printf("%i",gain);             //printing gain
//				delay_ms(50); 	
//				lcd_clear();
//				lcd_home();
		
			if(i<75)                   //if number of samples is less than 75
				i++;                   //increment the pointer in the array 
			else
			{
				i=0;                   //otherwise set the pointer back to 0.

			initmaxfinder();          //go to averaging function to get a single average of current
			initgainstatemachine();    //go to state machine that will decide the proper gain of the 75 samples and set it for 1,2 or 3 depending on the the 
			}
			initADC();                     //re-initialize the voltage ADC for taking voltage values again

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

	if((gain == 200) && (imax >= 479))   //if gain is at x200 and value is overflowing OR gain is x1 and value is too small
	{
		gain = 10;                                                                     //use x10 gain setting
	   
	}//if
	else if((gain == 1) && (imax <= 49))
	{
		gain =10;
     	                                       
	}//if
	else if ((gain == 10) && (imax <= 24))          //if gain is x10 and value is too small
	{
		gain = 200;                                  //use x200 gain setting changed from 200 to 10 (may revmove 10 gain)
	   
	}//if
	
	else if((gain == 10) && (imax >= 480))           //if gain is x10 and value is too large
	{
		gain = 1;                                     // use x1 gain setting
	}//if	

}//end gain state machine
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initsumofpoweraverages(void)
{
	for (int k=0; k<75; k++)
	{
		pavg = psamplediv75[k] + pavg;                                 //takes all "divided by 75" current samples and turns into a single averaged sample we can use to determine the gain with 
	}//for															   /TOTAL AVERAGE OF CURRENT (SINGLE VALUE)
}// sum of averages of current matrix
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initmaxfinder(void)               //subroutine for finding the maximum value of the top of the 3 periods
{
//PORTB=0x00;
	imax = 0;                          //this subroutine executes every 3 periods and clears the maximum every time
	for(k=0; k<75; k++)
	{
		if (imax < isamples[k])       //if current maximum is less than new maximum
		{
			imax = isamples[k];        //save the new maximum sample
		}//if
		
	}//for
//PORTB=0xFF;
}//max finder function
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
}//end USART initilization
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
		;
	/* Put data into buffer, sends the data */
	UDR = data;
}//end USART transmission              
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
