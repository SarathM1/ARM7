#include<lpc213x.h>
#include "lcd.h"
#include "uart2.h"


void sendAt(char *cmd)
{
	unsigned char i;
	
		
	for(i=0;cmd[i]!='\0';i++)
	{
		uart_tx_char(cmd[i]);
	}
	uart_tx_str("\r\n");
	delay(100);

	uart_tx_char('A');

	while(flag!=2);
	
	flag=0;
	
}

int main()
{
	uart_init();
	lcd_init();
	
	while(1)
	{
		sendAt("AT");
		sendAt("ATE0");
	}

}
