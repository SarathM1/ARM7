#include<lpc213x.h>
#include<stdlib.h>
#include "lcd.h"
#include "i2c.h"
#include "adc.h"
#include "uart0_inter.h"

char threshold[5];
int flag = 0;

void uart(void)__irq // ISR for UART0
{
	static int i = 0;
	char ch;
	ch = U0RBR; 		// Echo what is recieved
	
	uart_tx_char(ch);
	
	if(ch=='*')
	{
		i=0;
		flag=0;
	}
	else if(ch=='#')
	{
		threshold[4]='\0';
		flag=1;
	}
	else
	{
		threshold[i++]=ch;		
	}		
	
	VICVectAddr = 0;	// Compulsary in all ISR's
}


int main()
{
	//char* str;
	int adc_val,thresh_val;
	lcd_init();
	adc_init();
	uart_init();
	i2c_init();
	
	while(1)
	{
		if(flag ==1)
		{
			eeprom_write_str(threshold);
			flag=0;
		}
		
		adc_val = adc_read();
		cmd(0x8A);
		thresh_val = atoi(eeprom_read_str());
		//thresh_val = 1234;
		
		cmd(0xc0);
		lcd_int(adc_val);
		cmd(0xCA);
		lcd_int(thresh_val);

		if(adc_val == thresh_val)
		{
			cmd(0x80);
			lcd_str("ALARM!!");
		}
		else
		{
			cmd(0x80);
			lcd_str(". . .");
		}
		debug("done1");
	}			 
}
