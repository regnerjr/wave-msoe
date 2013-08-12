#include <math.h>

void floatToCharArray(float newAverage, char* string){ 

	int character[7] = {0};
	int e;
	e = log10(newAverage);

	if(e == 3)	//is in the thousands
	{
		character[0] = newAverage/1000;
		character[1] = newAverage/100 - character[0]*10;
		character[2] = newAverage/10 - character[0]*100 - character[1]*10;
		character[3] = newAverage - character[0]*1000 - character[1]*100 - character[2]*10;
		character[4] = -2;	//should be a decimal
		character[5] = newAverage*10 - character[0]*10000 - character[1]*1000 - character[2]*100 - character[3]*10;
		character[6] = newAverage*100 - character[0]*100000 - character[1]*10000 - character[2]*1000 - character[3]*100 - character[5]*10;
	}

	else if(e == 2)	//is in the thousands
	{
		character[0] = 0;
		character[1] = newAverage/100 - character[0]*10;
		character[2] = newAverage/10 - character[0]*100 - character[1]*10;
		character[3] = newAverage - character[0]*1000 - character[1]*100 - character[2]*10;
		character[4] = -2;	//should be a decimal
		character[5] = newAverage*10 - character[0]*10000 - character[1]*1000 - character[2]*100 - character[3]*10;
		character[6] = newAverage*100 - character[0]*100000 - character[1]*10000 - character[2]*1000 - character[3]*100 - character[5]*10;
	}

	else if(e == 1)	//is in the thousands
	{
		character[0] = 0;
		character[1] = 0;
		character[2] = newAverage/10 - character[0]*100 - character[1]*10;
		character[3] = newAverage - character[0]*1000 - character[1]*100 - character[2]*10;
		character[4] = -2;	//should be a decimal
		character[5] = newAverage*10 - character[0]*10000 - character[1]*1000 - character[2]*100 - character[3]*10;
		character[6] = newAverage*100 - character[0]*100000 - character[1]*10000 - character[2]*1000 - character[3]*100 - character[5]*10;
	}

	else if(e == 0)	//is in the thousands
	{
		character[0] = 0;
		character[1] = 0;
		character[2] = 0;
		character[3] = newAverage - character[0]*1000 - character[1]*100 - character[2]*10;
		character[4] = -2;	//should be a decimal
		character[5] = newAverage*10 - character[0]*10000 - character[1]*1000 - character[2]*100 - character[3]*10;
		character[6] = newAverage*100 - character[0]*100000 - character[1]*10000 - character[2]*1000 - character[3]*100 - character[5]*10;
	}

	else if(e == -1)	//is in the thousands
	{
		character[0] = 0;
		character[1] = 0;
		character[2] = 0;
		character[3] = 0;
		character[4] = -2;	//should be a decimal
		character[5] = newAverage*10 - character[0]*10000 - character[1]*1000 - character[2]*100 - character[3]*10;
		character[6] = newAverage*100 - character[0]*100000 - character[1]*10000 - character[2]*1000 - character[3]*100 - character[5]*10;
	}

	else if(e == -2)	//is in the thousands
	{
		character[0] = 0;
		character[1] = 0;
		character[2] = 0;
		character[3] = 0;
		character[4] = -2;	//should be a decimal
		character[5] = 0;
		character[6] = newAverage*100 - character[0]*100000 - character[1]*10000 - character[2]*1000 - character[3]*100 - character[5]*10;
	}
	for(int k=0; k<7; k++)
	{
		string[k] = character[k]+48;
	}
}
