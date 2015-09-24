 #include<lpc213x.h>
 #include "lcd.h"

 void adc_init()
 {
 	PINSEL1 =0x05400000; // SElect ADC for AD0.0 , AD0.1, AD0.2
 	
 }

int read_adc()
{
	int val;
	while ((AD0DR & 0x80000000)!=0x80000000) ;  // Waiting for adc
		
	val = (AD0DR & 0x0000FFC0) >> 6;

	return val;
}

 int main()
 {
 	int x,y,z;
	adc_init();

	lcd_init();	delay(100);
	
	data('a');
 	
	while(1) 
	{	cmd(0x01); // Clear LCD
		
		AD0CR = 0x00200600; // ADC Stop, select Channel 0
		AD0CR |= 0x01000000;  // Start ADC, Channel 0
		x = read_adc();
		cmd(0x80);
		dispInt(x);
		
		AD0CR = 0x00200602;
		AD0CR |= 0x01000000;  // Start ADC, Channel 0
		y = read_adc();
		cmd(0x85); 
		dispInt(y);
		
	
		AD0CR = 0x00200604;
		AD0CR |= 0x01000000;  // Start ADC, Channel 0
		z = read_adc();
		cmd(0x8A); 
		dispInt(z);
				  
		if(x>600 )
		{
			cmd(0x01);
			str("ALERT x = ");
			cmd(0xc0);
			dispInt(x);
			delay(1000);
			cmd(0x01);
		}
		if(y<300 )
		{
			cmd(0x01);
			str("ALERT y = ");
			cmd(0xc0);
			dispInt(y);
			delay(1000);
			cmd(0x01);
		}
		delay(100);		

	}
 	
 }
