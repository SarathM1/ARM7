#include<lpc213x.h>

void delay(unsigned int x)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
		for(j=0;j<7777;j++);
}
void pwm_init()	
{
	PINSEL0 = (1<<1); 	// Select PWM1 on P0.0 
	PWMPR	= 9999;
	/*
		Prescalar for 1ms
		PWMPR = (PWMPR+1)/(PCLK*10^6)
		Taking PCLk = 10 (10 MHz)
	*/
	PWMMR0	= 1000;	// 1 sec time period
	PWMMR1	= 500;	// T-ON = 50%
	PWMMCR 	= (1<<1);		// Reset PWMTC on PWMMR0 match
	PWMLER	= (1<<1)|(1<<0);	// update PWMMR0 and PWMMR1
	PWMPCR	= (1<<9);	// Enable PWM output
	PWMTCR 	= (1<<1);	// Reset PWM TC and PR
	PWMTCR  = (1<<0)|(1<<3);		// Enable counter's and PWM mode  

}


int main()
{
	int i=0;
	pwm_init();
	
	while(1)
	{
		delay(200);
		i++;
	}
	
}
