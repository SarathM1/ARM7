 #include<lpc213x.h>
 #include "lcd.h"

 void adc_init()
 {
 	PINSEL1 = 1<<24; // P0.28 as  AD0.1
	//AD0CR =  0x00200602;
	AD0CR = (1<<1)|(1<<9)|(1<<10)|(1<<21);

 }

 int main()
 {
 	int val;
	adc_init();
	lcd_init();	delay(100);
	while(1) 
	{
		cmd(0x01); // Clear LCD
		//lcd_char('b');
		AD0CR |= (1<<24);

		while( !((AD0DR >>31)&1) );  // Waiting for adc
		
		//while ((AD0DR & 0x80000000)!=0x80000000) ;  // Waiting for adc
		
		val = (AD0DR & 0x0000FFC0) >> 6;
		
		lcd_int(val);
		delay(1000);		

	}
 	
 }
