/*
Author:-	Embedded Laboratory

External EEPROM24C02 Test Program

FOSC = 12MHz				//Crystal Connected with the Board
CCLK = 60MHz				//FOSC X MSEL here MSEL = 5 hence Core is Running at 60MHz
PCLK = CCLK/4 = 15MHz
*/

#include <LPC214X.H>
#include "Delays.h"
#include "LCD.h"
#include "UART.h"
#include "I2C.h"

/**************Function Prototypes************/
void EEPROM24C_Write(unsigned char data,unsigned char address);
unsigned char EEPROM24C_Read(unsigned char address);
/*********************************************/


int main()
{
	unsigned char read_eeprom;
//	unsigned char i;
	
	UART0_Init();
	Lcd_Init();	
	Delay_ms(10);
	Lcd_Cmd(LCD_CLEAR);
	Lcd_Cmd(LCD_FIRST_ROW);
	Lcd_Write_Text("Writing... A");
	EEPROM24C_Write('A',0x00);
	//Lcd_Cmd(LCD_SECOND_ROW);
	read_eeprom = EEPROM24C_Read(0x00);
	Lcd_Write_Text("Reading... ");
	Lcd_Write(read_eeprom);	
	while(1);
}


/****************Function Definition**********/
void EEPROM24C_Write(unsigned char data,unsigned char address)
{
	I2C0_Init();
	I2C0_Start();	
	I2C0_Write(0xA0,0x18);
	I2C0_Write(address,0x28);	
	I2C0_Write(data,0x28);
	I2C0_Stop();
}
unsigned char EEPROM24C_Read(unsigned char address)
{
	unsigned char read;
	I2C0_Init();
	I2C0_Start();	
	I2C0_Write(0xA0,0x18);	
	I2C0_Write(address,0x28);
	I2C0_Stop();		
	I2C0_Start();	
	I2C0_Write(0xA1,0x40);	
	read = I2C0_Read();
	I2C0_Stop();
	return (read);
}

/*********************************************/
