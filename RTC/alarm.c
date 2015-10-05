#include<lpc213x.h>
#include "lcd.h"

void rtc(void)__irq;	  //Fn Declaration 

void rtc_init()
{

	AMR = 0XF8; // Mask all alarms except hour,min,sec	
	ILR = 0X03;
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
	HOUR 	= 23;
	MIN		= 59;
	SEC		= 58;

	ALSEC = 0;	 // Alarm for sec
	ALMIN = 0;	 // Alarm for min
	ALHOUR = 0;	 // Alarm for hour
	
}



void rtc(void)__irq // ISR for RTC
{
	
	cmd(0x01);

	cmd(0x8A);
	lcd_int(ILR);

	if(ILR & 0x01)
	{
		cmd(0xc0);
		lcd_int(HOUR) ;
		lcd_char(':');
		
		cmd(0xc3);
		lcd_int(MIN);
		lcd_char(':');
		
		cmd(0xc6);
		lcd_int(SEC);
	
		
	}

	cmd(0x8D);
	lcd_int(ILR);

	if(ILR & 0x02)
	{
		cmd(0x80);
		lcd_str("ALARM");
			
	}

	ILR |= 0X03;
	VICVectAddr = 0;	// Compulsary in all ISR's
	
}

int main()
{  	
	//unsigned int i = 0;
	

	lcd_init();
	

	rtc_init();
	reset_time();
	while(1);
}			   

