#include<lpc213x.h>
void uart_init()
{
	PINSEL0 |= 0x00050005; // Enable Txd and Rxd on P0.0 and P0.1
	U0LCR = 0X83;
	U0DLM = 0x00;
	U0DLL = 0xc3;
	U0LCR = 0X03;

	U1LCR = 0X83;
	U1DLM = 0x00;
	U1DLL = 0xc3;
	U1LCR = 0X03;
}
void uart0_tx_char(unsigned char ch)
{
	U0THR = ch;
	while( !(U0LSR &0x20) );
}

void uart0_tx_str(char *ch)
{
	unsigned int i;
	for(i=0;ch[i]!='\0';i++)
	{
		uart0_tx_char(ch[i]);
	}
}

char uart0_rx_char()
{
	while( !(U0LSR &0X01) );
	return U0RBR;
}

char* uart0_rx_str()
{
	char *bfr;
	unsigned char x=0;
	int i=0;
	bfr[x++] = uart0_rx_char();
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

/////////////// UART 1 /////////////////


void uart1_tx_char(unsigned char ch)
{
	U1THR = ch;
	while( !(U1LSR &0x20) );
}

void uart1_tx_str(char *ch)
{
	unsigned int i;
	for(i=0;ch[i]!='\0';i++)
	{
		uart1_tx_char(ch[i]);
	}
}
char uart1_rx_char()
{
	while( !(U1LSR &0X01) );
	return U1RBR;
}

char* uart1_rx_str()
{
	char *bfr;
	unsigned char x=0;
	int i=0;
	bfr[x++] = uart1_rx_char();
	while(i<6000)
	{
		if(U1LSR&0X01)
		{
			bfr[x++] = U1RBR;
			i=0;
		}
		else
		{
			i++;
		}
	}
	return (bfr);
}
