#include<lpc213x.h>
#include "lcd.h"

void rtc(void)__irq;	  //Fn Declaration 

void rtc_init()
{
	
	ILR = 0X01;
	CCR = 0X13;
	CCR = 0X11;
	CIIR = 0X01;

	VICIntEnable = 0x00002000; // Enable RTC Interrupt
	VICIntSelect = 0x00000000; // Set RTC interrupt as IRQ
	VICVectCntl0 = 0x2D;	// Asign 13 for RTC and set IRQslot_en
	VICVectAddr0 = (unsigned)rtc;	 // Address for ISR

}

void reset_time()
{
	HOUR 	= 0;
	MIN		= 0;
	SEC		= 0;
	
}



void rtc(void)__irq // ISR for RTC
{
	static int i=0;
	i++;
	//IOSET1 = IOPIN1 ^ 0x00010000;


	if(i%2==0)
		IOSET1 = 0X00010000;
	else
		IOCLR1 = 0X00010000;
	
	cmd(0x01);

	cmd(0xc0);
	lcd_int(HOUR) ;
	//lcd_char(':');
	
	cmd(0xc3);
	lcd_int(MIN);
	//lcd_char(':');
	
	cmd(0xc6);
	lcd_int(SEC);

	ILR = 0X01;
	VICVectAddr = 0;	// Compulsary in all ISR's
	
}

int main()
{  	
	//unsigned int i = 0;
	

	lcd_init();
	IODIR1 |= 0x00010000;
	

	/*
	while(1){
	IO1SET = 0x00010000;
	delay(500);
	IO1CLR = 0x00010000;
	delay(500);}
	*/

	rtc_init();
	reset_time();
	while(1);
}			   

