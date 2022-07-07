#include "SoftSPI.h"






static void delay (uint32_t Time)
{
    uint32_t i;
    
    i = 0;
    while (Time--) {
        for (i = 0; i < 5000; i++);
    }
}



void SoftSPIinit()
{
		SoftSPIConfiq_s SPIS;
		//config DAC spi //
		SPIS.SCK.GPIOPORT=2;
    SPIS.SCK.GPIOPIN= 12;
		SPIS.SCS.GPIOPORT= 2;
		SPIS.SCS.GPIOPIN= 11;
		SPIS.SDO.GPIOPORT=2;
		SPIS.SDO.GPIOPIN=13;
		SoftSPIConfig(SPIS);
		

		//		
}




 
 
 
void SoftSPIConfig(SoftSPIConfiq_s SPIType)
{   
	GPIO_SetDir(SPIType.SCK.GPIOPORT, SPIType.SCK.GPIOPIN, GPIO_DIR_OUTPUT);
	GPIO_SetDir(SPIType.SCS.GPIOPORT, SPIType.SCS.GPIOPIN, GPIO_DIR_OUTPUT);
  GPIO_SetDir(SPIType.SDO.GPIOPORT, SPIType.SDO.GPIOPIN, GPIO_DIR_OUTPUT);
}

/*
   Send SPI data to slave device
  SPIType -> SPI configuration struct
	data    -> data to be send
	size    -> size of serial data (1 to 32)
	CLKMode -> CLKMode=RISING_EDGE =>data will send on rising edge of clock 
	LEMode	-> LEMode=1 => LE=1 when data is sending. LEMode=0 =>LE=0 when data is sending.

*/

void SoftSPISend(SoftSPIData_s SPIDType)

{ 
	uint32_t k;uint8_t i=SPIDType.size;
	if(SPIDType.SPIMode.Modebits.LEMode==1)
		GPIO_PinWrite(SPIDType.SPIType.SCS.GPIOPORT, SPIDType.SPIType.SCS.GPIOPIN, 1);
	else
		GPIO_PinWrite(SPIDType.SPIType.SCS.GPIOPORT, SPIDType.SPIType.SCS.GPIOPIN, 0);
		
	k=(SPIDType.SPIMode.Modebits.LSBMode)?1:1<<(SPIDType.size-1);	
	for(i=SPIDType.size;i>0;i--)
	{
	
			if((SPIDType.data)&(k))
				GPIO_PinWrite(SPIDType.SPIType.SDO.GPIOPORT, SPIDType.SPIType.SDO.GPIOPIN, 1);
			else
				GPIO_PinWrite(SPIDType.SPIType.SDO.GPIOPORT, SPIDType.SPIType.SDO.GPIOPIN, 0);
			
			if(SPIDType.SPIMode.Modebits.CLKMode==RISING_EDGE)
			{ 
			  delay(1);
				GPIO_PinWrite(SPIDType.SPIType.SCK.GPIOPORT, SPIDType.SPIType.SCK.GPIOPIN, 1);			
				delay(1);
				GPIO_PinWrite(SPIDType.SPIType.SCK.GPIOPORT, SPIDType.SPIType.SCK.GPIOPIN, 0);				
			}
			else
			{ 
			  delay(1);
				GPIO_PinWrite(SPIDType.SPIType.SCK.GPIOPORT, SPIDType.SPIType.SCK.GPIOPIN, 0);			
				delay(1);
				GPIO_PinWrite(SPIDType.SPIType.SCK.GPIOPORT, SPIDType.SPIType.SCK.GPIOPIN, 1);								
			}
			
						
			 			
		k=(SPIDType.SPIMode.Modebits.LSBMode)?(k*2):(k/2);	
	}
		delay(1);							
	if(SPIDType.SPIMode.Modebits.LEMode==1)
		GPIO_PinWrite(SPIDType.SPIType.SCS.GPIOPORT, SPIDType.SPIType.SCS.GPIOPIN, 0);
	else
		GPIO_PinWrite(SPIDType.SPIType.SCS.GPIOPORT, SPIDType.SPIType.SCS.GPIOPIN, 1);	
			 
  
}

