#include "uart0_inter.h"
#include "lcd.h"

char string[]="GPRMC";

int gprmc = 0;
int comma  = 0;
int dataValid = 0;
int x = 0;
char latitude[10];
int latValid = 0;

void uart(void)__irq // ISR for UART0
{
	char ch;
	ch= U0RBR;

	U0THR = ch;
	while(!(U0LSR&0x20));

	

	if(ch=='$')
	{
		gprmc = 0;
		comma = 0;
		dataValid = 0;
		latValid=0;
		x=0;
	}
	
	if(string[gprmc]==ch)
	{
		gprmc++;
	}

	if(ch==',')
	{
		comma++;
	}

	switch(comma)
	{
		case 2:
			if(ch=='A')
				dataValid=1;
			else if(ch=='V')
				dataValid=0;
			break;
		case 3:
			latitude[x++]=ch;
			latitude[x+1]='\0';
			break;
		case 4:
			
			latValid = 1;
			break;
					
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

			//cmd(0xC0);
			//dispInt(comma);

			if(dataValid)
			{
				cmd(0x85);
				lcd_char('A');
			}
			else if(dataValid==0)
			{
				cmd(0x85);
				lcd_char('V');
			}

			if(latValid)
			{
				cmd(0xC0);
				lcd_str(latitude);
			}
		}
	}
}
