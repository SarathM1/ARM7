#include<lpc213x.h>
#include "uart.h"
#include "lcd.h"

void sendAt(char *cmd)
{
	unsigned char i;
	for(i=0;cmd[i]!='\0';i++)
	{
		uart_tx_char(cmd[i]);
	}
	uart_tx_str("\r\n");
	lcd_str(uart_rx_str());
}

int main()
{
	uart_init();
	lcd_init();
	sendAt("at\r\n");
	
	sendAt("at+cipclose=1\r\n") ;
	
	sendAt("ate0\r\n");

	sendAt("at+cpin?\r\n");
	
	sendAt("at+csq\r\n");
	
	sendAt("at+creg?\r\n");
	
	sendAt("at+cgatt?\r\n");
	
	sendAt("at+cipshut\r\n");
	
	sendAt("at+cstt=\"internet\"\r\n");


	sendAt("at+ciicr\r\n");
	
	sendAt("at+cifsr\r\n");

	sendAt("at+cipstart=\"TCP\",\"52.76.46.163\",\"5000\"\r\n");

	sendAt("at+cipsend\r\n");
	

}
