
/*
Author:-	Embedded Laboratory
I2C Functions C File
*/

#include <LPC214X.H>
#include "I2C.h"

/****************Function Definition**********/
void I2C0_Init(void)
{	
	PINSEL0 |= (1<<4)|(1<<6);		/*Initialize Pin Connect Block P0.2 as SCL0 P0.3 as SDA0*/
	I2C0CONCLR = 0x2C;	/*Clear AA, I2C Interrupt Flag, Start Bit*/
	I2C0CONSET = 0x44;	/*Enable I2C0*/
	/*
	PCLK = 15MHz
	I2C Frequency = 100KHz
	Formula:
	I2C Frequency = PCLK/((I2CSCLH + I2CSCLL)
	(I2CSCLH + I2CSCLL) = 15000/100 = 150	
	*/
	I2C0SCLH = 75;
	I2C0SCLL = 75;
	/*Bit Rate 100Khz*/	
}
void I2C0_Start(void)
{
	I2C0CONSET |= 0x20;			/*Set the Start Bit*/
	while(I2C0STAT!=0x08);	/*Wait for the Status Set*/
}
void I2C0_Stop(void)
{
	I2C0CONSET |= 0x14;			/*Stop I2C*/
  I2C0CONCLR  = 0x08;
}
void I2C0_Write(unsigned char data,unsigned char status)
{
	I2C0DAT = data;
  I2C0CONSET = 0X04;
  I2C0CONCLR = 0X28;
  while(I2C0STAT!=status);		  /* Wait for Status Set */
}
unsigned char I2C0_Read(void)
{
	I2C0CONSET = 0X04;
  I2C0CONCLR = 0X28;
  while (I2C0STAT!=0x00000050);	/* Wait for Status Set - 0x50 */
  return(I2C0DAT);
}
/*********************************************/
