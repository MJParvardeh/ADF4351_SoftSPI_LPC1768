
#include "ADF4351.h"
#include "SoftSPI.h"
SoftSPIConfiq_s SPIS;
void ADF4351_Init()
{
	SPIS.SCK.GPIOPORT=	SCK_PORT;
	SPIS.SCK.GPIOPIN= 	SCK_PIN;
	SPIS.SCS.GPIOPORT= 	SCS_PORT;
	SPIS.SCS.GPIOPIN= 	SCS_PIN;
	SPIS.SDO.GPIOPORT=	SDO_PORT;
	SPIS.SDO.GPIOPIN=		SDO_PIN;
	SoftSPIConfig(SPIS);
	SET_ADF4351_LD(LD_PORT,LD_PIN);
}
/*
Direct programming of ADF4351 registers
for more information refer to ADF4351.h and ADF4351 Datasheet
*/
uint8_t ADF4351_DirectSet(ADF_REG_t PLLREG)
{
	uint32_t	timeout;
	SoftSPIData_s PLLS;
	PLLS.SPIMode.Modebits.CLKMode=RISING_EDGE;
	PLLS.SPIMode.Modebits.LEMode=0;
	PLLS.SPIMode.Modebits.LSBMode=MSB_FIRST;		 
	PLLS.size=32;	
	PLLS.SPIType=SPIS;
	PLLS.data=PLLREG.REG0;
	SoftSPISend(PLLS);	
	PLLS.data=PLLREG.REG1;
	SoftSPISend(PLLS);
	PLLS.data=PLLREG.REG2;
	SoftSPISend(PLLS);
	PLLS.data=PLLREG.REG3;
	SoftSPISend(PLLS);	
	PLLS.data=PLLREG.REG4;
	SoftSPISend(PLLS);
	while((READ_LD_VALUE==0)&&(timeout<60000))	timeout++;
	if(timeout==60000)
		return	ADF4351_ERROR;
	else
		return	ADF4351_OK;	
}

static uint32_t SetADF4351REG0(uint8_t Mode,uint8_t CO,uint8_t CHS, uint32_t FREQ) {
  uint32_t freq;
  float N1, F1, FR; 
  uint16_t N, F;
  uint32_t Reg0;	
	freq=FREQ;
	if((Mode==Tx)&&(CHS==CHS25))
	{
		switch(CO)
		{
			case CarrOff_P7500:
				freq=freq+750;
				break;
			case CarrOff_P5000:
				freq=freq+500;
				break;
			case CarrOff_P2500:
				freq=freq+250;
				break;
			case CarrOff_0:
				break;
			case CarrOff_M2500:
				freq=freq-250;
				break;
			case CarrOff_M5000:
				freq=freq-500;
				break;
			case CarrOff_M7500:
				freq=freq-750;
				break;
			default:
				break;
		}
		
	}
		
  FR = (float) freq / 100000;
	
  if (CHS == CHS833) {
		if(Mode==Tx)
		{
			N1 = (FR * 32) / 9.6;
			N = (uint16_t) N1;
			F1 = ((N1 - N) * 36);
			F = (uint16_t) F1;
			if((F1-F)>0.5)
			F=F+1;
			Reg0 = (N << 15) | (F << 3);
		}
		else
		{
			N1 = (FR * 16) / 9.6;
			N = (uint16_t) N1;
			F1 = ((N1 - N) * 72);
			F = (uint16_t) F1;
			if((F1-F)>0.5)
			F=F+1;
			Reg0 = (N << 15) | (F << 3);			
		}
  } 
	else //25 KHz
	{
		if(Mode==Tx)
		{
			
			if(CO==CarrOff_0)
			{
				N1 = (FR * 32) / 9.6;
				N = (uint16_t) N1;
				F1 = ((N1 - N) * 12);
				F = (uint16_t) F1;
				if((F1-F)>0.5)
					F=F+1;
			}
			else
			{
				
				N1 = (FR * 32) / 9.6;
				N = (uint16_t) N1;
				F1 = ((N1 - N) * 120);
				F = (uint16_t) F1;
				if((F1-F)>0.5)
					F=F+1;				
			}
		}
		else
		{
			N1 = (FR * 16) / 9.6;
			N = (uint16_t) N1;
			F1 = ((N1 - N) * 24);
			F = (uint16_t) F1;
			if((F1-F)>0.5)
				F=F+1;
		}
	
    Reg0 = (N << 15) | (F << 3);

  }

  return Reg0;

}



static uint32_t SetADF4351REG1(uint32_t Mode,uint8_t CO,uint8_t CHS) {
 
		uint32_t Reg1;
		if(CHS==CHS833)
		{
			if(Mode==Rx)
				Reg1=0x8008241;
			else
				Reg1=0x8008121;
		}
		else//25KHz
		{
			if(Mode==Rx)
				Reg1=0x80080C1;
			else
			{
				if(CO==CarrOff_0)
					Reg1=0x8008061;
				else
					Reg1=0x80083C1;	
			}
		}

		return Reg1;

}

/*
SetPLLFREQ is a function for generatin carrier frequency for VHF tranceiver 118-137 MHz
TRMode: 			Transceiver mode
PowerMode:		Enable or Disable RF Output power 
CO:						Channel offset from -7.5 KHz to +7.5 KHz
CHS:					Channel Spacing is 8KHz or 25 KHz
FREQ:					Frequency to Hz				
*/                          
uint8_t SetPLLFREQ(uint8_t TRMode,uint8_t PowerMode,uint8_t CO,uint8_t CHS, uint32_t FREQ)
{
		 uint16_t timeout;
     uint32_t REG0,REG1;
		 SoftSPIData_s PLLS;
		 
		 PLLS.SPIMode.Modebits.CLKMode=RISING_EDGE;
		 PLLS.SPIMode.Modebits.LEMode=0;
     PLLS.SPIMode.Modebits.LSBMode=MSB_FIRST;		 
		 PLLS.size=32;
		 PLLS.SPIType=SPIS;		 
     if(TRMode==Rx)
		 {
				REG0 = SetADF4351REG0(Rx,CO,CHS, FREQ+3885000);
				REG1=SetADF4351REG1(Rx,CO,CHS);
				PLLS.data=REG0;
				SoftSPISend(PLLS); 
				PLLS.data=REG1;			
				SoftSPISend(PLLS);	
				PLLS.data=0x8E42;
				SoftSPISend(PLLS);
				PLLS.data=0x4B3;
				SoftSPISend(PLLS);			 
				if(PowerMode==PLLEN)
					PLLS.data=0xC4D03C;
				else
					PLLS.data=0xc4d01C;			
				SoftSPISend(PLLS);			 
			 
			}
			if(TRMode==Tx)
		 {
				REG0 = SetADF4351REG0(Tx,CO,CHS, FREQ);
				REG1=SetADF4351REG1(Tx,CO,CHS);
				PLLS.data=REG0;
				SoftSPISend(PLLS); 
				PLLS.data=REG1;			
				SoftSPISend(PLLS);
			  SoftSPISend(PLLS);
				PLLS.data=0x8E42;
				SoftSPISend(PLLS);
				PLLS.data=0x4B3;
				SoftSPISend(PLLS);			 
				if(PowerMode==PLLEN)
					PLLS.data=0xD4D03C;
				else
					PLLS.data=0xD4d01C;			
				SoftSPISend(PLLS);			 
						 
		}
		
		
		PLLS.data=0x580005;
		SoftSPISend(PLLS);
		
		GPIO_PinWrite(PLLS.SPIType.SCS.GPIOPORT, PLLS.SPIType.SCS.GPIOPIN,0);
		while((READ_LD_VALUE==0)&&(timeout<60000))	timeout++;
		if(timeout==60000)
			return	ADF4351_ERROR;
		else
			return	ADF4351_OK;
			
			
}



