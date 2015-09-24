 #include<lpc213x.h>
 #include "lcd.h"

 void adc_init()
 {
 	PINSEL1 = 0x01000000; // P0.28 as  AD0.1
	//IODIR0 = 0X00000000; // Default is i/p
	AD0CR =  0x00200602;

 }

 int main()
 {
 	int val;
	 adc_init();
	lcd_init();	delay(100);
	 data('a');
 	while(1) 
	{
		cmd(0x01); // Clear LCD
		data('b');
		AD0CR |= 0x01000000;

		//while( !(AD0DR & (1 <<31)) );  // Waiting for adc
		
		while ((AD0DR & 0x80000000)!=0x80000000) ;  // Waiting for adc
		
		val = (AD0DR & 0x0000FFC0) >> 6;
		  
		//val = 1234;
		dispInt(val);
		delay(1000);		

	}
 	
 }
