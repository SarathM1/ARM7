#include<lpc213x.h>

void delay(int x)
{
	int i,j;
	for(i=0;i<x;i++)
		for(j=0;j<1000;j++);
}

void pwm_init()	
{
	PINSEL0 = 0x02; 	// Select PWM1 on P0.0 
	PWMTCR 	= 0x02;		// Enable PWM, Enable counter, 

	PWMMR0 	= 15000;	// Value to generate PWM of 500 ms Time period
	
	/*
		reqd time (ms) = TMMR0 / PCLK(MHz)
		
		here PCLK = 30 MHz	
	*/ 	

	PWMMR1 	= 15000;  // Make duty cycle=100%, Duty cycle = PRMMR1 / PWMMR0 
	PWMMCR 	= 0X02;		// Reset PWM1 on match
	PWMPCR	= 0x0200;	// Enable PWM
	PWMLER	= 0X03;		// Load values to PWMMR0 and PWMMR1 on next match
	PWMTCR  = 0x09;		// Reset PWM Timer Counter and PWM Prescalat counter on match  
}


int main()
{
	pwm_init();
	//delay(500);
	while(1)
	{
		PWMMR1 	-= 1000;
		
		if(PWMMR1 <= 0)
			PWMMR1 = 15000;
		
		PWMLER	= 0X02;		// Load values to PWMMR1 on next match
		delay(500);
		
	}
	
	//while(1);

}
