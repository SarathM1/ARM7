#include "uart0_inter.h"
#include "lcd.h"

int flagD = 0;
int comma = 0;
int gprmc = 0;
char flagPack[]="GPRMC";

char latitude[15];
char longitude[15];
int x=0;
int y=0;
char NS;
char EW;

int dataValid = 0;
int latValid=0;
int longValid = 0;
int nsValid =0;
int ewValid	= 0;

void uart(void)__irq // ISR for UART0
{
	char ch;
	ch= U0RBR;

	U0THR = ch;
	while(!(U0LSR&0x20));

	if(ch=='$')
	{
		flagD =1;
		comma=0;
		gprmc=0;
		x=0;	
		y=0;
		dataValid = 0;
		latValid=0;
		longValid = 0;
		nsValid =0;
		ewValid	= 0;
	}

	if( (ch == flagPack[gprmc]) && (flagD == 1) )
	{
		gprmc++;
	}

	if(gprmc==5)
	{
		if(ch == ',')
		{
			comma++;
		}

		switch(comma)
		{
			case 0:case 1:case 2:break;

			case 3: 
				if(ch=='A')
				{
					dataValid = 1;				
				}
				else 
				{
					dataValid = 0;
				}
				break;

			case 4:
				latitude[x++]=ch;
				latitude[x+1]='\0';
				break;

			case 5:
				NS = ch;
				break;

			case 6:
				longitude[y++]=ch;
				longitude[y+1]='\0';
				break;

			case 7:
				EW = ch	;
				break;	
			
			case 8:case 9:case 10:case 11:case 12:
				break;
			
			default:
				flagD=0;		
				break;
		
		}
	}

	VICVectAddr = 0;
}


int main()
{
	int i;
	uart_init();
	lcd_init();
	

	while(1)
	{
		if(!dataValid)
		{
			cmd(0x01);
			lcd_str("Data Invalid!!");		
		}
		
		else
		{
			if(latValid)
			{
				cmd(0x80);
		 		for(i=0;i<9;i++)
					lcd_char(latitude[i]);
			}
			
			if(nsValid)
			{
			
				cmd(0x8C);
				lcd_char(NS);
			}

			if(longValid)
			{
				cmd(0xc0);
				for(i=0;i<9;i++)
					lcd_char(longitude[i]);
			}
			if(ewValid)
			{
				cmd(0xcc);
			 	lcd_char(EW);
			}
		}
	}
	
}
