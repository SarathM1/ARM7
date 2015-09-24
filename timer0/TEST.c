#include<lpc213x.h>

void timer(void)__irq;	  //Fn Declaration

void gpio_init()
{
	IO1DIR= 0x00FF0000;

}

void blink_led1()
{
	unsigned int i,j,x=100;
	IO1SET = 0x00010000;
	for(i=0;i<x;i++)
		for(j=0;j<10000;j++);
	IO1CLR = 0x00010000;
}

void blink_led2()
{
   unsigned int i,j,x=100;
	IO1SET = 0x00020000;
	for(i=0;i<x;i++)
		for(j=0;j<10000;j++);
	IO1CLR = 0x00020000;

}


void timer_init()
{
	T0TCR = 0X02; // Enable Timer Counter and Prescalar counter, Reset Tmr Cntr and Presc Cntr
	//T0CTCR = 0X00; //  Select Timer Mode	
	
	//T0MCR = 0x03;	// Interrupt on MR0, Reset on MR0
	T0MCR = 0x1B;	// Interrupt on MR0 & MR1 , Reset on MR0 & MR1

	T0MR0 = 15000000; // T0MR0 = (Req.Time(in sec) * PCLK)
						// Here PCLK = 30* 10^6
	T0MR1 = 30000000;	// T0MR1 = (Req.Time(in sec) * PCLK)
						// Here PCLK = 30* 10^6
	VICIntEnable = 0x00000010; // Enable TIMER0 Interrupt
	VICIntSelect = 0x00000000; // Set TIMER0 interrupt as IRQ
	VICVectCntl0 = 0x24;	// Asign 4 (bits 4:0) to make TIMER0 Highest Priority
							// and set IRQslot_en
	VICVectAddr0 = (unsigned)timer;	 // Address for ISR
	
}

void timer(void)__irq // ISR for timer0
{
	if(T0IR==0X01)
	{
		//blink_led1();
		IO1SET = 0x00010000;
		T0IR = 0X01;
		VICVectAddr = 0;	
	}	// Reset all Interrupt Flags
	if(T0IR == 0x02)
	{
		//blink_led2();
		IO1CLR = 0x00010000;
		T0IR = 0x02;
		VICVectAddr = 0;	
	}

		
}

int main()
{
	timer_init();
	gpio_init();
	T0TCR = 0X01;
	while(1);
}
