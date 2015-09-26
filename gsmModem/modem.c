#include<lpc213x.h>
#include "lcd.h"
#include "uart0_inter.h"

void uart(void)__irq // ISR for UART0
{
	char ch;
	ch = U0RBR;
	
	U0THR = ch; 		// Echo what is recieved
	while( !(U0LSR & 0x20) );

	if(ch == resp[flag])
		flag++;

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
	delay(100);

	while(flag!=2);
	
	flag=0;
	
}

int main()
{
	uart_init();
	
	while(1)
	{
		sendAt("AT");
		sendAt("ATE0");
	}

}
