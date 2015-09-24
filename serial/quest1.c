#include<lpc213x.h>
#include "lcd.h"

void uart(void)__irq;	  //Fn Declaration

char ar[15];
char ch;
unsigned int i=0,j;
unsigned int flag = 0;

void uart_init()
{
	PINSEL0 = 0x05; // Enable Txd0 on P0.0, Rxd0 on P0.1
	
	U0IER = 0x01; // Enable RX interrupt
	U0FCR = 0x01; //FIFO Enable
	
	U0LCR = 0x83;
	
	U0DLM = 0x00;
	U0DLL = 0xc3;
	
	U0LCR = 0x03;

}

void interrupt_init()
{
	VICIntEnable = 0x00000040; // Enable UART0 Interrupt
	VICIntSelect = 0x00000000; // Set UART0 interrupt as IRQ
	VICVectCntl0 = 0x26;	// Asign 6 for UART0 and set IRQslot_en
	VICVectAddr0 = (unsigned)uart;	 // Address for ISR
}

void uart(void)__irq // ISR for UART0
{
	ch = U0RBR;
	
	if(ch == '*')
	{
		flag=1;
		i=0;
	}
	else if(ch == '#')
	{
		flag = 0;
	}

	if(flag == 1)
	{
		ar[i] = ch; 	
		i++;
	}
	else
	{
		cmd(0x01); // Clear LCD
		for(j=1;j<=i-1;j++)
			data(ar[j]);
		
	}
	VICVectAddr = 0;
}

int main()
{
	uart_init();
	interrupt_init();
	lcd_init();
	while(1);		// Main will wait infinitely except for serving UART0 Interrupt
}