 #include<lpc213x.h>
 #include "lcd.h"
 #include "uart.h"

 void adc_init()
 {
 	PINSEL1 =0x05400000; // SElect ADC for AD0.0 , AD0.1, AD0.2
 	
 }

int read_adc()
{
	int val;
	while (( AD0GDR & 0x80000000 )==0) ;  // Waiting for adc
		
	val = (AD0GDR & 0x0000FFC0) >> 6;

	return val;
}

 int main()
 {
 	int x,y,z;
	adc_init();
	uart_init();

	lcd_init();	delay(100);
	
	lcd_char('a');
 	
	while(1) 
	{	cmd(0x01); // Clear LCD
		
		AD0CR = 0x01200600; // ADC Stop, select Channel 0
		//AD0CR |= 0x01000000;  // Start ADC, Channel 0
		x = read_adc();
		cmd(0x80);
		lcd_int(x);
		uart_tx_num(x);
		uart_tx_char(' ');
		
		AD0CR = 0x01200602;
		//AD0CR |= 0x01000000;  // Start ADC, Channel 0
		y = read_adc();
		cmd(0x85); 
		lcd_int(y);
		uart_tx_num(y);
		uart_tx_char(' ');

		
	
		AD0CR = 0x01200604;
		//AD0CR |= 0x01000000;  // Start ADC, Channel 0
		z = read_adc();
		cmd(0x8A); 
		lcd_int(z);
		uart_tx_num(z);
		uart_tx_str("\r");

				  
		if(x>600 )
		{
			cmd(0x01);
			lcd_str("ALERT x = ");
			uart_tx_str("\nAlert x");
			cmd(0xc0);
			lcd_int(x);
			delay(1000);
			cmd(0x01);
		}
		if(y<300 )
		{
			cmd(0x01);
			lcd_str("ALERT y = ");
			uart_tx_str("\nAlert y");
			cmd(0xc0);
			lcd_int(y);
			delay(1000);
			cmd(0x01);
		}
		delay(100);	
		
			

	}
 	
 }
