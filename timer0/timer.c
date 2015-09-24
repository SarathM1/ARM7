#include<lpc213x.h>

void timer(void)__irq;	  //Fn Declaration

void timer_init()
{
	T0TCR = 0X02; // Enable Timer Counter and Prescalar counter, Reset Tmr Cntr and Presc Cntr
	T0CTCR = 0X00; //  Select Timer Mode	
	T0MCR = 0x03;	// Interrupt on MR0, Reset on MR0
	T0MR0  = 15000000; // T0MR0 = (Req.Time(in sec) * PCLK)
						// Here PCLK = 30* 10^6
}

void interrupt_init()
{
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
	T0IR = 0X01;
	VICVectAddr = 0;
}	// Reset all Interrupt Flags		
}

int main()
{
	timer_init();
	interrupt_init();
	T0TCR = 0X01;
	while(1);
}