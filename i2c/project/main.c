#include<lpc213x.h>
#include<stdlib.h>
#include "lcd.h"
#include "uart0_inter.h"
#include "adc.h"
#include "i2c.h"

	
void uart(void)__irq // ISR for UART0
{
	char ch;
	char threshold[5];
	static int i;

	ch = U0RBR;	

	if(ch == '*')
	{
		i = 0;
	}
	else if(ch == '#')
	{
		threshold[i] = '\0';
		flag = 0;	 					// Blocking eeprom_read_str in main
		eeprom_write_str(threshold);
		debug_str("Written to eeprom!!");
		delay(1000);
		cmd(0x01);
		flag = 1;					// Resuming main
	}
	else
	{
		threshold[i] = ch;
		i++;
	}
	
	VICVectAddr = 0;
}

int main()
{
	int thresh_val;
	int adc_val;
	adc_init();
	i2c_init();
	uart_init();
	lcd_init();
	
	while(1)
	{
			if(flag == 1)
			{
				thresh_val = atoi(eeprom_read_str()); 		// Reading from eeprom, convert to integer
				uart_tx_int(thresh_val);
				uart_tx_str("\r\n");
	
				adc_val = adc_read();
				cmd(0x8A);
				lcd_int(adc_val);
	
				if(adc_val > thresh_val)
				{
					cmd(0x80);
					lcd_str("ALARM!!");
				}
				else
				{
					cmd(0x80);
					lcd_str(". . . .");
				}
	
				delay(100);
			}
	}
}


