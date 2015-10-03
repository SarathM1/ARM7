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
	PWMPR	= 59;
	/*
		PWMPR = (PWMPR+1)/(PCLK*10^6)
		Taking PCLk = 60 (60 MHz)
	*/
	PWMMR0	= 10000;	// 10 ms time period
	PWMMR1	= 2500 ;	// T-ON = 25%
	PWMMCR 	= (1<<1);		// Reset PWMTC on PWMMR0 match
	PWMLER	= (1<<1)|(1<<0);	// update PWMMR0 and PWMMR1
	PWMPCR	= (1<<9);	// Enable PWM output
	PWMTCR 	= (1<<1);	// Reset PWM TC and PR
	PWMTCR  = (1<<0)|(1<<3);		// Enable counter's and PWM mode  

	
}


int main()
{
	pwm_init();
	//delay(500);
	/*
	while(1)
	{
		PWMMR1 	-= 1;
		
		if(PWMMR1 <= 0)
			PWMMR1 =6 ;
		
		PWMLER	= 0X03;		// Load values to PWMMR1 on next match
		delay(500);
		
	}
	  */
	while(1);

}
