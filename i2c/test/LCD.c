
/*
Author:-	Embedded Laboratory
LCD Functions C File
*/

#include "LCD.h"
#include "Delays.h"

/****************LCD's Function***************/
void Lcd_Init(void)
{
	Delay_ms(30);
	/*
	LCD Needs Some time to Properly Initialize
	Thats why some delay is provided in startup.
	*/
	
	IO1DIR |= 0x00FE0000;	
	/*Pins P1.17 to P.23 as Output Pin*/	
	Write_Cmd(0x30<<16);
	Delay_ms(20);
	Write_Cmd(0x30<<16);
	Delay_ms(20);
	Write_Cmd(0x30<<16);
	Delay_ms(10);
	Write_Cmd(0x20<<16);
	Delay_ms(20);
	/*These are the Commands for LCD Initialization in 4-Bit Mode*/
	Lcd_Cmd(LCD_CLEAR);
	Delay_ms(10);
	Lcd_Cmd(0x06);
	Delay_ms(10);
	Lcd_Cmd(LCD_CURSOR_OFF);
	Delay_ms(10);
	Lcd_Cmd(LCD_FIRST_ROW);
	Delay_ms(10);
}
void Lcd_Cmd(unsigned int data)
{
	Write_Cmd(data<<16);
	Write_Cmd(data<<20);
}
void Lcd_Write(unsigned int data)
{
	Write_Data(data<<16);
	Write_Data(data<<20);
}
void Lcd_Write_Text(unsigned char *msg)
{
	while(*msg)
	{
		Lcd_Write(*msg);
		msg++;
	}
}
void Write_Cmd(unsigned int value)
{
	/*First of all Clear the LCD Data Pins*/
	IO1CLR |= 0x00F00000;
	/*To Write RW = 0*/
	IO1CLR |= (1<<LCD_RW);
	/*Write to Command Register RS = 0*/
	IO1CLR |= (1<<LCD_RS);
	/*Write to Pins*/
	IO1SET |= 0x00F00000 & value;
	Enable_Pulse();
}

void Write_Data(unsigned int value)
{
	/*First of all Clear the LCD Data Pins*/
	IO1CLR |= 0x00F00000;
	/*To Write RW = 0*/
	IO1CLR |= (1<<LCD_RW);
	/*Write to Data Register RS = 1*/
	IO1SET |= (1<<LCD_RS);
	/*Write to Pins*/
	IO1SET |= 0x00F00000 & value;
	Enable_Pulse();
}
/*********************************************/
void debug()
{
	Delay_ms(100);
	Lcd_Cmd(LCD_CLEAR);
	Lcd_Write_Text("DONE!!");
	Delay_ms(100);
	Lcd_Cmd(LCD_CLEAR);
}
