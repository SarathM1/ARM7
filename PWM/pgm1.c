#include<lpc213x.h>

void delay(int x)
{
	int i,j;
	for(i=0;i<x;i++)
		for(j=0;j<1000;j++);
}

void pwm_init()	
{
	PINSEL0 = (1<<1); 	// Select PWM1 on P0.0 
	PWMPR	= 9999;
	/*
		PWMPR = (PWMPR+1)/(PCLK*10^6)
		Taking PCLk = 60 (60 MHz)
	*/
	PWMMR0	= 1000;	// 1 sec time period
	PWMMCR 	= (1<<1);		// Reset PWMTC on PWMMR0 match
	PWMLER	= (1<<0);	// update PWMMR0 and PWMMR1
	PWMPCR	= (1<<9);	// Enable PWM output
	PWMTCR 	= (1<<1);	// Reset PWM TC and PR
	PWMTCR  = (1<<0)|(1<<3);		// Enable counter's and PWM mode  

	PWMMR1	= 500;	// T-ON = 50%
	PWMLER	= (1<<1);		// Load values to PWMMR1 
	
}


int main()
{
	unsigned int i;
	pwm_init();
	
	while(1);
	while(1)
	{
		for(i=0;i<=100;i++)
		{
			PWMMR1 	-= 10;
			PWMLER	= (1<<1);		// Load values to PWMMR1 
			delay(100);
		}

		for(i=0;i<=100;i++)
		{
			PWMMR1 	+= 10;
			PWMLER	= (1<<1);		// Load values to PWMMR1 
			delay(100);
		}
		
	}
	  
	
	
}
