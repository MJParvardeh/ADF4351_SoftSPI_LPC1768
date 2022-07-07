#ifndef __ADF4351_H__
#define __ADF4351_H__
#include <stdint.h>
#include "GPIO_LPC17xx.h"

//ADF4351 Lock Detect Definitions
#define LD_PORT											2
#define LD_PIN											10
#define SET_ADF4351_LD(PORT,PIN)		GPIO_SetDir(PORT, PIN, GPIO_DIR_INPUT)
#define READ_LD_VALUE								GPIO_PinRead(LD_PORT, LD_PIN)

//ADF4351 CLK Definitions
#define SCK_PORT										2
#define SCK_PIN											11

//ADF4351 DATA Definitions
#define SDO_PORT										2
#define SDO_PIN											12

//ADF4351 LE Definitions
#define SCS_PORT										2
#define SCS_PIN											13

#define  CHS833     								1  //8.33 KHz chnnel space
#define  CHS25      								0  //25   KHz chnnel space

#define  Tx     										1
#define  Rx     									  0

#define PLLEN   										1
#define PLLDIS					  					0 

#define ADF4351_ERROR								1
#define ADF4351_OK									0

#define CarrOff_P7500 							1
#define CarrOff_P5000 							2
#define CarrOff_P2500	 							5
#define CarrOff_0 									7
#define CarrOff_M2500 							3
#define CarrOff_M5000 							4
#define CarrOff_M7500 							6
/*
register structs for parametric definition of 
ADF4351 registers
*/
typedef union 
{
	uint32_t	REG0_VAL;
	struct 
	{
		uint32_t	control							:3;
		uint32_t	fractional					:12;
		uint32_t	integer							:16;
		uint32_t	reserved						:1;		
	} Register0_s;	
} REG0_t;	
	
typedef union 	
{	
	uint32_t	REG1_VAL;	
	struct 	
	{	
		uint32_t	control							:3;
		uint32_t	modulus							:12;
		uint32_t	phase								:12;
		uint32_t	prescaller					:1;	
		uint32_t	phase_adjust				:1;	
		uint32_t	reserved						:3;		
	} Register1_s;
} REG1_t;

typedef union 
{
	uint32_t	REG2_VAL;
	struct 
	{
		uint32_t	control							:3;
		uint32_t	cnt_reset						:1;
		uint32_t	cp_3state						:1;
		uint32_t	power_down					:1;	
		uint32_t	pd_polarity 				:1;	
		uint32_t	ldp					 				:1;
		uint32_t	ldf					 				:1;
		uint32_t	chargepump_set 			:4;		
		uint32_t	double_buf					:1;
		uint32_t	r_counnter		 			:10;	
		uint32_t	rdv2							 	:1;
		uint32_t	reference_doubler 	:1;
		uint32_t	muxout 							:3;
		uint32_t	lr_ls_mode				 	:2;
		uint32_t	reserved					 	:1;				
	} Register2_s;
} REG2_t;

typedef union 
{
	uint32_t	REG3_VAL;
	struct 
	{
		uint32_t	control							:3;		
		uint32_t	clock_driver				:14;
		uint32_t	clk_div_mode				:2;
		uint32_t	reserved						:1;
		uint32_t	csr									:1;	
		uint32_t	reserved1 					:2;	
		uint32_t	charge_cancel		 		:1;
		uint32_t	abp					 				:1;
		uint32_t	band_select 				:1;		
		uint32_t	reserved2						:8;
	} Register3_s;
} REG3_t;

typedef union 
{
	uint32_t	REG4_VAL;
	struct 
	{
		uint32_t	control							:3;		
		uint32_t	output_power				:2;
		uint32_t	rf_output_enable		:1;
		uint32_t	aux_output_power		:2;
		uint32_t	aux_output_enable		:1;	
		uint32_t	aux_output_select 	:1;	
		uint32_t	mtld						 		:1;
		uint32_t	vcp_power_down	 		:1;
		uint32_t	band_select_driver	:8;		
		uint32_t	rf_div_select				:3;
		uint32_t	feedback_select			:1;
		uint32_t	reserved						:8;					
	} Register4_s;
} REG4_t;

typedef union 
{
	uint32_t	REG5_VAL;
	struct 
	{
		uint32_t	control							:3;		  
		uint32_t	reserved						:16;		
		uint32_t	reserved1						:2;			 
		uint32_t	reserved2						:1;			
		uint32_t	ld_pin_mode					:2;	
		uint32_t	reserved3					 	:8;							
	} Register5_s;
} REG5_t;

typedef  struct
{
	REG0_t	REG0;
	REG1_t	REG1;
	REG2_t	REG2;
	REG3_t	REG3;	
	REG4_t	REG4;
	REG5_t	REG5;
} ADF4351_REG_t;

uint8_t ADF4351_DirectSet(ADF4351_REG_t PLLREG);
void ADF4351_Init();
uint8_t SetPLLFREQ(uint8_t TRMode,uint8_t PowerMode,uint8_t CO,uint8_t CHS, uint32_t FREQ);

#endif
