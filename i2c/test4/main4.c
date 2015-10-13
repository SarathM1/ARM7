#include<lpc213x.h>
#include "lcd.h"
#include "uart0_inter.h"

unsigned char i,j;

void uart(void)__irq // ISR for UART0
{
	uart_tx_char(U0RBR);
	VICVectAddr = 0;
}
void start(void)
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
	while(I2C0STAT!=0X18);
}

void devadd2(void)
{
	I2C0DAT=0XA1;
	I2C0CONSET=0X04;
	I2C0CONCLR=0X08;
	while(I2C0STAT!=0X40);
}

void location(int add)
{
	I2C0DAT=add;
	I2C0CONSET=0X04;
	I2C0CONCLR=0X28;
	while(I2C0STAT != 0X28);
}

void stop(void)
{ 
	I2C0CONSET=0X14;		  	//SEND STOP BIT(STO) & ACKNOWEDGE FLAG SET
	I2C0CONCLR=0X08;
}

char readdata(void)
{
	I2C0CONSET=0X04;
	I2C0CONCLR=0X28;
	while(I2C0STAT!=0X50);
	return I2C0DAT; 
}

void i2c_init()
{
	//IO0SET=0X0000000D;	   //SETTING SCL0,SDA0 & TX PIN SET FOR O/P
	PINSEL0 |= 0X00000050;
	I2C0CONCLR=0XFF;
	I2C0CONSET=0X44;		   // ENABLE THE I2C BY SETTING I2EN BIT & ACKNOWEDGE FLAG SET  
	I2C0SCLL=0X96;
	I2C0SCLH=0X96;
	
}



void eeprom_read()
{
	/***********************WRITING DATA*************************************/
	start();
	uart_tx_str("\r\nhere\r\n");
	devadd1();
	location(0x00);
	stop();
	
	/******************************************READ********************************************/
	start();
	devadd2();
	for(j=0;j<=15;j++)
	{
		uart_tx_char(readdata());
	}
//	for(j=17;j<=64;j++)
//	{
//		readdata();
//	}
	stop();
}


int main()
{
	i2c_init();
	uart_init();
	while(1)
	{
		   	I2C0CONCLR=0XFF;
			I2C0CONSET=0X44;		   // ENABLE THE I2C BY SETTING I2EN BIT & ACKNOWEDGE FLAG SET  
			
			delay(500);
			uart_tx_str("\r\nReading from eeprom\r\n");
			eeprom_read();
			uart_tx_str("\r\nDONE!!\r\n");
			delay(500);	
	}
}

