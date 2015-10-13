#include<lpc213x.h>
#include<stdlib.h>
#include "lcd.h"
#include "uart0_inter.h"
#include "adc.h"
#include "i2c.h"

void uart(void)__irq // ISR for UART0
{
	char ch;
	ch = U0RBR;
	uart_tx_char(ch);
	VICVectAddr = 0;
}

int main()
{
	lcd_init();
	adc_init();
	uart_init();
	i2c_init();
	while(1);
}
