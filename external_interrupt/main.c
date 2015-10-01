#include<lpc213x.h>
#include "lcd.h"

void eint1_isr(void)__irq;	  //Fn Declaration
void inter_init()
{
	/* We can use either P0.3 or P0.14 as EINT1*/
	PINSEL0 		= 0x20000000; // Enable EINT1 on P0.14, -> 29:28 = 10
		
	/***EXTMODE, EXTPOLAR AND EXTINT must be used 
	before enabling interrupt using VICIntEnable **/

	EXTMODE			= 0X02;			// EINT1 is edge sensitive
	EXTPOLAR		= 0X02;			// Rising edge sensitive
	EXTINT 			= 0x02;			// Clear EINT1 flag
	
	VICIntEnable	= 0x00008000; // Enable EINT1 (bit 15 high)
	VICIntSelect 	= 0x00000000; // IRQ
	VICVectCntl0 	= 0x2F;		// Bits 4:0 = 15(mask), bit 5 =1 (IRQslot_en);
	
	INTWAKE			= 0X02;			// Assertion EINT1 will wake up processor from pow down mode
	VICVectAddr0 	= (unsigned)eint1_isr;	 // Address for ISR  
}

void eint1_isr(void)__irq // ISR for EINT1
{
	static int x = 0;
	x++;
	cmd(0x80);
	dispInt(x);
	IOPIN1 = ~IO1SET;
	EXTINT = 0X02;		// Must be set in ISR to clear EINT1 flag
	VICVectAddr = 0;   // Must be cleared in ISR
}


int main()
{
	IODIR1 = 0x00FF0000;
	lcd_init();
	inter_init();
	while(1);


}
