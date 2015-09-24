#include<lpc213x.h>
void uart_init()
{
	PINSEL0 = 0x00000005; // Enable Txd and Rxd on P0.0 and P0.1
	U0LCR = 0X83;
	U0DLM = 0x00;
	U0DLL = 0xc3;
	U0LCR = 0X03;
}
void uart_tx_char(unsigned char ch)
{
	U0THR = ch;
	while( !(U0LSR &0x20) );
}

void uart_tx_str(unsigned char *ch)
{
	unsigned int i;
	for(i=0;ch[i]!='\0';i++)
	{
		uart_tx_char(ch[i]);
	}
}
unsigned char uart_rx_char()
{
	while( !(U0LSR &0X01) );
	return U0RBR;
}

char* uart_rx_str()
{
	char *bfr;
	unsigned char x=0;
	int i=0;
	bfr[x++] = uart_rx_char();
	while(i<6000)
	{
		if(U0LSR&0X01)
		{
			bfr[x++] = U0RBR;
			i=0;
		}
		else
		{
			i++;
		}
	}
	return (bfr);
}
