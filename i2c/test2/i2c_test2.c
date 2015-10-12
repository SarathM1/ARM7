#include"LPC213x.h"
void Initialize(void);

/* I 2 C ISR */
//__irq void I2C _ISR(void);
void I2C_ISR(void)__irq;	  //Fn Declaration

/* Master Transmitter states */
void ISR_8(void);
void ISR_18(void);
void ISR_28(void);
/*************************** MAIN ************************/

int main()
{
	/* Initialize system */
	Initialize ();
	
	/* Send start bit */
	I2C0CONSET=0x60; 
	
	/* Do forever */
	while(1);
//	{
//		//IOCLR=0x40;
//		//IOSET=0x40;
//	}
}

/*************** System Initialization ***************/
void Initialize()
{
	/* Remap interrupt vectors to SRAM */
	MEMMAP=0x2;
	/* Initialize GPIO ports to be used as indicators */
	//IODIR=0xF0;
	//IOSET=0xF0; 
	/* Initialize Pin Connect Block */
	PINSEL0=0x50;
	/* Initialize I 2 C */
	I2C0CONCLR=0x6c; /* clearing all flags */
	I2C0CONSET=0x40; /* enabling I 2 C */
	I2C0SCLH=0xC; /* 100 KHz */
	I2C0SCLL=0xD;
	/* Initialize VIC for I 2 C use */
	VICIntSelect=0x0; /* selecting IRQ */
	VICIntEnable= 0x200;  /* enabling I 2 C */
	VICVectCntl0= 0x29; /* highest priority and enabled */
	VICVectAddr0=(unsigned) I2C_ISR;

	/* ISR address written to the respective address register*/
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
	VICVectAddr = 0xFF;
}

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
