#include "uart0_inter.h"
#include "lcd.h"

char string[]="GPRMC";

int gprmc = 0;
int comma  = 0;


void uart(void)__irq // ISR for UART0
{
	char ch;
	ch= U0RBR;

	U0THR = ch;
	while(!(U0LSR&0x20));

	

	if(ch=='$')
	{
		cmd(0x01);
		gprmc = 0;
		
	}
	
	if(string[gprmc]==ch)
	{
		gprmc++;
	}

	if(ch==',')
	{
		comma++;
		cmd(0xc0);
		dispInt(comma);
	}

	

	
}

int main()
{
	uart_init();
	lcd_init();

	while(1)
	{
		if(gprmc==5)
		{
			cmd(0x80);
			lcd_str("GPRMC");
		}
	}
}
