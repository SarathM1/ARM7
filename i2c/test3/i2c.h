//#include<lpc213x.h>
//#include "lcd.h"
//#include "uart0_inter.h"

unsigned char i;

void start(void)
{
	 I2C0CONSET=0x64;	  //SEND START BIT(STA) & ACKNOWEDGE FLAG SET
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
	I2C0CONCLR=0X48;
}

char readdata(void)
{
	I2C0CONSET=0X04;
	I2C0CONCLR=0X28;
	while(I2C0STAT!=0X50);
	return I2C0DAT; 
}

void writedata(char ch)
{
	I2C0DAT=ch;
	I2C0CONSET=0X04;
	I2C0CONCLR=0X08;
	while(I2C0STAT != 0X28);
}

void i2c_init()
{
	PINSEL0 |= 0X00000050;
	I2C0CONCLR=0XFF;
	I2C0CONSET=0X44;		   // ENABLE THE I2C BY SETTING I2EN BIT & ACKNOWEDGE FLAG SET  
	I2C0SCLL=0X96;
	I2C0SCLH=0X96;
	
}



char* eeprom_read_str()
{
	int j;
	char* val;
	/***********************WRITING DATA*************************************/
	start();
	//uart_tx_str("\r\nhere\r\n");
	devadd1();
	location(0x00);
	stop();
	
	/******************************************READ********************************************/
	start();
	devadd2();
	for(j=0;j<=5;j++)
	{
		val[j] = readdata();
	}

	stop();
	return val;
}

void eeprom_write_str(char* str)
{
	int k;
	start();
	devadd1();
	location(0x00);
	for(k=0;str[k]!='\0';k++)	   //array[i]!='\0';
	{
		writedata(str[k]);
	}
	stop();
	delay(2);		  // delay 2 ms. I2c won't work if removed
}


