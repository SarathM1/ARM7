#include"LPC213x.h"


/* I 2 C states */
/* Start condition transmitted */

void ISR_8()
{
	/* Port Indicator */
	 //IOCLR=0x10; 
	/* Slave address + write */
	 I2C0DAT=0x74; 
	/* Clear SI and Start flag */
	 I2C0CONCLR=0x28; 
	/* Port Indicator */
	 //IOSET=0x10;
}

/* Acknowledgement received from slave for slave address */
void ISR_18()
{
	/* Port Indicator */
	//IOCLR=0x20; 
	/* Data to be transmitted */
	I2C0DAT=0x55;  
	/* clear SI */
	I2C0CONCLR=0x8;
	/* Port Indicator */
	//IOSET=0x20;
}

/* Acknowledgement received from slave for byte transmitted from master. Stop
condition is transmitted in this state signaling the end of transmission */
void ISR_28()
{
	/* Port Indicator */
	 //IOCLR=0x80;
	/* Transmit stop condition */
	 I2C0CONSET=0x10; 
	/* clear SI */
	 I2C0CONCLR=0x8;  
	/* Port Indicator */
	 //IOSET=0x80;
}

/********************** I 2 C ISR **************************/
//__irq void I2C_ISR()
void I2C_ISR(void)__irq
{
	int temp=0;
	temp=I2C0STAT;
	switch(temp)
	{
		case 8:
			ISR_8();
			break;
		
		case 24:
			ISR_18();
			break;
		
		case 40:
			ISR_28();
			break;
		
		default :
			break;
	} 
	
	//VICVectAddr = 0xFF;  		// Change here
	VICVectAddr	= 0;
}

void i2c_init()
{
	
	//MEMMAP = 0x2;				/* Remap interrupt vectors to SRAM */
	PINSEL0 = 0x50;				/* Initialize Pin Connect Block */
	I2C0CONCLR = 0XFF; 			/* clearing all flags */
	//I2C0CONSET = 0x40; 		// Change here 
	I2C0CONSET = 0X44;		   // ENABLE THE I2C BY SETTING I2EN BIT & ACKNOWEDGE FLAG SET  
	I2C0SCLH = 0x96; 			/* 100 KHz */
	I2C0SCLL = 0x96;
	
	/* Initialize VIC for I 2 C use */
	VICIntSelect = 0x0; 				/* selecting IRQ */
	VICIntEnable = 0x200;  				/* enabling I 2 C */
	VICVectCntl0 = 0x29; 				/* highest priority and enabled */
	VICVectAddr0 = (unsigned) I2C_ISR;	/* ISR address written to the respective address register*/
}

int main()
{
	/* Initialize system */
	i2c_init();
	
	/* Send start bit */
	I2C0CONSET=0x60; 
	
	/* Do forever */
	while(1);
}


