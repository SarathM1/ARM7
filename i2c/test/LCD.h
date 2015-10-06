
/*
Author:-	Embedded Laboratory
LCD Functions Header File
*/

#include <LPC214X.H>

/************LCD Pin and Macros*************/
#define LCD_EN								19
#define LCD_RS								17
#define LCD_RW								18
#define LCD_CLEAR							0x01
#define LCD_CURSOR_OFF				0x0C
#define LCD_FIRST_ROW					0x80
#define LCD_SECOND_ROW				0xC0
#define Enable_Pulse()				IOSET1|=1<<LCD_EN;Delay_ms(1);IOCLR1=1<<LCD_EN;Delay_ms(1);

void Lcd_Init(void);
void Lcd_Cmd(unsigned int data);
void Lcd_Write(unsigned int data);
void Lcd_Write_Text(unsigned char *msg);
void Write_Cmd(unsigned int value);
void Write_Data(unsigned int value);

/*******************************************/
