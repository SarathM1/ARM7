#include"LPC213x.h"
#include "uart0_inter.h"

#define SLAV_ADDR 0XA0
#define SIC 3
#define STAC 5
#define I2ENC 6

#define	 STO 4
#define	 STA 5
#define	 I2EN 6
//#define

void uart(void)__irq // ISR for UART0
{
	VICVectAddr = 0;	// Compulsary in all ISR's
}

//__irq void I2C_ISR()
void I2C_ISR(void)__irq
{
	static int flag = 0;
	int temp=0;
	temp=I2C0STAT;
	switch(temp)
	{
		case 0x08:					/* Start condition transmitted */
			if(flag == 0)
				I2C0DAT = SLAV_ADDR; /* Slave address + write */
			else if(flag == 1)
				I2C0DAT = SLAV_ADDR | 0x01; /* Slave address + write */
			I2C0CONCLR = (1<<SIC) | (1<<STAC); 	/* Clear SI and Start flag */
			break;
		
		case 0x18: 				/* Acknowledgement received from slave for slave address */
			I2C0DAT=0X00;  		/* Data to be transmitted */
			I2C0CONCLR = (1<<SIC);		/* clear SI */
			break;
		
		case 0x28:				/* Acknowledgement received from slave for byte transmitted from master. Stop
								condition is transmitted in this state signaling the end of transmission */
			switch(flag)
			{
				case 0:
					I2C0CONSET = (1<<STO); 	/* Transmit stop condition */
					flag++;
					I2C0CONSET = (1 << STA); 			/* Send start bit */
					break;
				default:
					break;
			}
			I2C0CONCLR = (1<<SIC);  	/* clear SI */
			break;
		
		case 0x50:
			uart_tx_char(I2C0DAT);
			I2C0CONSET = (1<<STO);
			I2C0CONCLR = (1<<SIC);		/* clear SI */
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
	I2C0CONSET = (1<<I2EN); 	/* Enable i2c */ 
	I2C0SCLH = 0x96; 			/* 100 KHz */
	I2C0SCLL = 0x96;
	
	/* Initialize VIC for I 2 C use */
	VICIntSelect = 0x0; 				/* selecting IRQ */
	VICIntEnable = 0x200;  				/* enabling I 2 C */
	VICVectCntl0 = (1<<5) | (0x09); 	/* highest priority and enabled, int_request(4:0) = 0x09, IRQslot_en1(5) = 1 */
	VICVectAddr0 = (unsigned) I2C_ISR;	/* ISR address written to the respective address register*/
}

int main()
{
	i2c_init();
	uart_init();
	I2C0CONSET = (1 << STA); 			/* Send start bit */
	while(1);
}


