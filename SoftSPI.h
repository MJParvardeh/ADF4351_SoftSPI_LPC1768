
#ifndef __SOFTSPI_H__
#define __SOFTSPI_H__

// pll soft SPI define
#include <stdint.h>
#include "GPIO_LPC17xx.h"

#define RISING_EDGE     1
#define FALLING_EDGE    0

#define MSB_FIRST       0 
#define LSB_FIRST       1



typedef struct
{

 struct {
uint8_t GPIOPORT ;
uint8_t GPIOPIN ;
} SCS,SCK,SDO;


}SoftSPIConfiq_s;

typedef union
{
uint8_t Modevalue;
struct 
{
	uint8_t CLKMode:1;   //CLKMode is   RISING_EDGE or FALLING_EDGE
  uint8_t LEMode:1;    // 1=> we have clk when LE is 1  0=> we have clk when LE is 0
	uint8_t LSBMode:1;  //LSBMODE is    MSB_FIRST   or LSB_FIRST
} Modebits;


} SPIMode_u;

typedef struct
{

uint32_t data;
uint32_t size;
SPIMode_u SPIMode;
SoftSPIConfiq_s SPIType;
} SoftSPIData_s;


void SoftSPISend(SoftSPIData_s SPIDType) ;
void SoftSPIinit(void);
void SoftSPIConfig(SoftSPIConfiq_s SPIType) ;

#endif
