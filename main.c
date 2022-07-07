
#include "lpc17xx.h"
#include "ADF4351.h"
/*
test code for sweeping 118 to 137 MHz with ADF4351
*/
static void delay(uint32_t delay)
{
	uint32_t i, j;
	for(i=0; i<delay; i++)
	{
		for(j=0; j<14300; j++);
	}
}
int main()
{
	uint32_t Frequency= 118000000;
	ADF4351_Init();
	while(1)
	{
		
		Frequency+=25000;
		SetPLLFREQ(Tx, PLLEN, CarrOff_0, CHS25, Frequency);
		delay(1000);
		if(Frequency== 137000000)
			Frequency= 118000;
	}
	
	
	
}

