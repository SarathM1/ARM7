#include"uart2.h"


void uart(void)__irq // ISR for UART0
{
	//char ch;
	U0THR = U0RBR; 		// Echo what is recieved

	while( !(U0LSR & 0x20) );
	VICVectAddr = 0;
}

int main()
{
	uart_init();
	
	while(1);

}
