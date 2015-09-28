#include<lpc213x.h>
#include "lcd.h"
#include "uart0_inter.h"

int nextAt=0;
void uart(void)__irq // ISR for UART0
{
	char ch;
	ch = U0RBR;
	
	if(ch == resp[flag])
	{	
		lcd_char(ch);
		flag++;
	}
	
	if(flag == 2)
	{
		flag = 0;
		nextAt++;	
	}
	
	VICVectAddr = 0;
}

void sendAt(char *cmd)
{
	unsigned char i;
	
		
	for(i=0;cmd[i]!='\0';i++)
	{
		uart_tx_char(cmd[i]);
	}
	uart_tx_str("\r\n");
	delay(250);
	
}

int main()
{
	uart_init();
	lcd_init();

	while(1)
	{
	
		switch(nextAt)
		{
			case 0: sendAt("AT");
					break;
			case 1: sendAt("ATE0");
					break;
			case 2: sendAt("AT+CMGF=1");
					break;					
		}
			
	}

}
