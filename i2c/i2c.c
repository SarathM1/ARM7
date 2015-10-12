#include<lpc21xx.h>
#include "lcd.h"
#include "uart0_inter.h"

char x=1;
char array1[64]={"INDIA IS MY COUNTRY AND ALL INDIANS ARE MY BROTHERS AND SISTERS"};    //SHIJU VARGHESE CHIRAKKALAKATHU  THADIKKADAVU	
char i,j;

void uart(void)__irq // ISR for UART0
{
	VICVectAddr = 0;
}
void start(void)
{
	 I2CONSET=0x24;	  //SEND START BIT(STA) & ACKNOWEDGE FLAG SET
	 I2CONCLR=0x08;
	 while(I2STAT!=0X08);
}

void devadd1(void)
{
	I2DAT=0XA0;
	I2CONSET=0X04;
	I2CONCLR=0X08;
	while(I2STAT!=0X18);
}

void devadd2(void)
{
	I2DAT=0XA1;
	I2CONSET=0X04;
	I2CONCLR=0X08;
	while(I2STAT!=0X40);
}

void location(int add)
{
	I2DAT=add;
	I2CONSET=0X04;
	I2CONCLR=0X28;
	while(I2STAT != 0X28);
}

void stop(void)
{ 
	I2CONSET=0X14;		  	//SEND STOP BIT(STO) & ACKNOWEDGE FLAG SET
	I2CONCLR=0X08;
}

void writedata1(void)
{
	I2DAT=array1[i];
	I2CONSET=0X04;
	I2CONCLR=0X08;
	while(I2STAT != 0X28);
}

char readdata(void)
{
	I2CONSET=0X04;
	I2CONCLR=0X28;
	while(I2STAT!=0X50);
	return I2DAT; 
}

void i2c_init()
{
	//IO0SET=0X0000000D;	   //SETTING SCL0,SDA0 & TX PIN SET FOR O/P
	PINSEL0 |= 0X00000050;
	I2CONCLR=0XFF;
	I2CONSET=0X44;		   // ENABLE THE I2C BY SETTING I2EN BIT & ACKNOWEDGE FLAG SET  
	I2SCLL=0X96;
	I2SCLH=0X96;
	
}


void eeprom_write()
{
	start();
	devadd1();
	location(0x00);
	for(i=0;i<=15;i++)	   //array[i]!='\0';
	{
		writedata1();
	}
	stop();
	delay(2);		  // delay 2 ms. I2c won't work if removed
}

void eeprom_read()
{
	/***********************WRITING DATA*************************************/
	start();
	devadd1();
	location(0x00);
	stop();
	
	/******************************************READ********************************************/
	start();
	devadd2();
	for(j=0;j<=64;j++)
	{
		uart_tx_char(readdata());
	}
	stop();
}

int main()
{
	i2c_init();
	uart_init();
	
	while(1)
	{
		eeprom_read();	
	}
}

