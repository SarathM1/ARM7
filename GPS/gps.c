#include "uart0_inter.h"
#include "lcd.h"

char string[]="GPRMC";

int gprmc = 0;
int comma  = 0;
int dataValid = -1;
int x = 0;
char latitude[20];
int latValid = 0;
char ns=0;
char longitude[20];
int longValid = 0;
char ew=0;

void uart(void)__irq // ISR for UART0
{
	char ch;
	ch= U0RBR;

	if(ch=='$')
	{
		gprmc = 0;
		comma = 0;
		dataValid = -1;
		latValid=0;
		x=0;
		latitude[0]='\0';
		longitude[0]='\0';
		ns=0;
		longValid=0;
		ew=0;
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
			if(ch!=',')
			{
				latitude[x++]=ch;
				latitude[x+1]='\0';
			}
			break;
		
		case 4:
			latValid = 1;
			x=0;			//Reset x to reuse for longitude
			if(ch!=',')
			{
				ns=ch;
			}
			break;
		
		case 5:
			if(ch!=',')
			{
				longitude[x++]=ch;
				longitude[x+1]='\0';
			}
			break;
		
		case 6:
			longValid = 1;
			if(ch!=',')
			{
				ew=ch;
			}
			break;

	}
	VICVectAddr = 0;
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
			lcd_str("G ");

			if(dataValid==1)
			{
				cmd(0x82);
				lcd_str("A ");
			}
			else if(dataValid==0)
			{
				cmd(0x82);
				lcd_str("V ");
			}

			if(latValid)
			{
				cmd(0x84);
				lcd_str(latitude);  
			}

			if(ns!=0)
			{
				lcd_char(ns);
			}

			if(longValid)
			{
				cmd(0xc0);
				lcd_str(longitude);
			}

			if(ew!=0)
			{
				lcd_char(ew);
			}
		}
	}
}
