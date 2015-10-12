#include<lpc213x.h>
#include "lcd.h"

int i;
char array[64]={"INDIA IS MY COUNTRY AND ALL INDIANS ARE MY BROTHERS AND SISTERS"};    

void i2c0_init()
{
	PINSEL0 = (1<<4)|(1<<6);
	I2C0CONCLR = 0X2C;   //Clear AA, I2C Interrupt Flag and Start Bit	 #
	I2C0CONSET = (1<<6)|(1<<2);	// I2EN =1, AA=1					#
	I2C0SCLH	= 0X96;
	I2C0SCLL	= 0X96;
}

void i2c0_start()
{
	I2C0CONSET = (1<<5)|(1<<2); // STA =1 				#
	I2C0CONCLR=0x08;
	while(I2C0STAT!=0X08); // wait for acknowlege from slave
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


void i2c0_stop()
{
	I2C0CONSET = (1<<2) | (1<<4);	 // AA = 1, STO = 1
	I2C0CONCLR = (1<<3);			//Clear SIC
}

void writeData(char data)
{
	I2C0DAT 	= data;   // Data valid before other steps		#
	I2C0CONSET = (1<<2);
	I2C0CONCLR = (1<<3);	// SIC = 1		 #
	while(I2C0STAT != 0x28);
}

char readData()
{
	I2C0CONSET = (1<<2);
	I2C0CONCLR = (1<<3) | (1<<5);	// SIC = 1, STAC = 1
	while(I2C0STAT != 0x50);
	return I2C0DAT;
}


void eeprom_write()
{
	i2c0_start();
	devadd1();
	location(0x00);
	for(i=0;i<=15;i++)
		writeData(array[i]);	// Data = 'A' , wait  while I2C0STAT != 0X28
	i2c0_stop();
}

void dummy_write()
{
	i2c0_start();
	devadd1();
	location(0x00);
	i2c0_stop();
}
char eeprom_read()
{
	char data;

	/******DUMMY WRITE Start********/
	
	/******DUMMY WRITE End********/

	/******READ DATA Start********/
	i2c0_start();
	devadd2();
	for(i=0;i<=15;i++)
	{	data = readData();
	 	lcd_char(data);
	 }
	i2c0_stop();
	I2C0CONSET = 0X	
	/******READ DATA End********/

	return (data);

}

void delay2(int b)
{
	while(b!=0)
	{
		b--;
	}
}


int main()
{
	char data;
	lcd_init();
	i2c0_init();
	eeprom_write();
	delay2(20000);
	debug("Data Written!");
	dummy_write();
	while(1)
	{
		//debug("Reading data. .");
		//cmd(0xc0);
		data = eeprom_read();
		delay2(150000);
		delay2(150000);
		delay2(150000);
		delay2(150000);
		lcd_char(data);
	}
	//while(1);
}
