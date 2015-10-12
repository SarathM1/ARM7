#include<lpc213x.h>
#include "lcd.h"
#include "uart0_inter.h"
int i;
char array[] = "Hello";

void uart(void)__irq // ISR for UART0
{
	uart_tx_char(U0RBR); 		// Echo what is recieved


	VICVectAddr = 0;	// Compulsary in all ISR's
}

void i2c_init()
{
	I2C0ADR = 0XA1;	
	PINSEL0=0X00000051;
	I2C0CONCLR=0XFF;
	I2C0SCLL=0X96;
	I2C0SCLH=0X96;
}

void master_init()
{
	I2C0CONSET=0X44;		   // Use 0x40 for Master only mode otherwise  
}

void start()
{
	I2C0CONSET=0x24;	  //SEND START BIT(STA) & ACKNOWEDGE FLAG SET
	I2C0CONCLR=0x08;
	while(I2C0STAT!=0X08);
}

void devadd1(void)
{
	I2C0DAT=0XA0;
	I2C0CONSET=0X04;
	I2C0CONCLR=0X08;
	while(I2C0STAT!=0X18)
	{
		if(I2C0STAT == 0x20)
		{
			I2C0CONSET = 0X14;
			I2C0CONCLR	= 0X08;	
			break;
		}
	
	}
}

void devadd2(void)
{
	I2C0DAT=0XA1;
	I2C0CONSET=0X04;
	I2C0CONCLR=0X08;
	while(I2C0STAT!=0X40);
}

void location(void)
{
	I2C0DAT=0x00;
	I2C0CONSET=0X04;
	I2C0CONCLR=0X28;
	while(I2C0STAT != 0X28)
	{
		if(I2C0STAT == 0x20)
		{
			I2C0CONSET = 0X14;
			I2C0CONCLR	= 0X08;	
			break;
		}
		//lcd_str(int_to_str(I2C0STAT));
	}
}

void writeData(void)
{
	I2C0DAT=array[i];
	I2C0CONSET=0X04;
	I2C0CONCLR=0X08;
	while(I2C0STAT != 0X28);
}

char readdata(void)
{
	I2C0CONSET=0X04;
	I2C0CONCLR=0X28;
	while(I2C0STAT!=0X50 & I2C0STAT!= 0X58);
	
	return(I2C0DAT);
	
}

void stop()
{
	I2C0CONSET = 0X14;
	I2C0CONCLR	= 0X08;

}

void eeprom_write()
{
	start();
	devadd1();
	location();
	for(i=0;i<5;i++)
		writeData();
	stop();	
}



void eeprom_read()
{
	char ch;
	/****************DUMMY*************/
	//I2C0CONSET = 0X20; 
	start();
	devadd1();
	location();
	stop();
	/********************************/

	/***************READ************/
	start();
	devadd2();
	for(i=0;i<=5;i++)
	{
		ch = readdata();
		uart_tx_char(ch);
	}
	I2C0CONCLR = 0X0C;
	stop();

}


int main()
{
	uart_init();
	i2c_init();
	master_init();
	eeprom_write();
	eeprom_read();
	while(1);
}
