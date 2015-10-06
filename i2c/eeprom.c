#include<lpc21xx.h>
#include "lcd.h"

void i2c_init()
{
	PINSEL0 = (1<<4)|(1<<6);
	I2CONCLR = 0XFF;   // Clear all bits of I2CONSET
	I2CONSET = 0X40;	// Enable I2C, I2EN =1
	I2SCLH	= 0X96;
	I2SCLL	= 0X96;
}

void i2c_start()
{
	I2CONSET = (1<<2)|(1<<5); // AA=1 , STA =1
	I2CONCLR = (1<<3);	// SIC = 1
	while(I2STAT!=0X08); // wait for acknowlege from slave


}

void i2c_addr(char slave_addr)
{
	I2CONSET = (1<<2); // AA =1
	I2CONCLR = (1<<3)|(1<<5);	// SIC = 1
	I2DAT = slave_addr;
	while(I2STAT != 0X18);	
}

void i2c_addr2(char slave_addr)
{
	I2CONSET = (1<<2); // AA =1
	I2CONCLR = (1<<3);	// SIC = 1
	I2DAT = slave_addr;
	while(I2STAT != 0X40);
}

void i2c_write(char data)
{
	I2CONSET = (1<<2);
	I2CONCLR = (1<<3);	// SIC = 1
	I2DAT 	= data;
	while(I2STAT != 0X28);
}

char i2c_read()
{
	I2CONSET = (1<<2);
	I2CONCLR = (1<<3);	// SIC = 1
	while(I2STAT != 0x50);
	return I2DAT;
}
void i2c_location(char loc_addr)
{
	I2CONSET = (1<<2);
	I2CONCLR = (1<<3)|(1<<5);	// SIC = 1 , Clear ST0 ??
	I2DAT 	= loc_addr;
	while(I2STAT != 0X28);

}

void i2c_stop()
{
	I2CONSET = (1<<2) | (1<<4);	 // AA = 1, STO = 1
	I2CONCLR = (1<<3);			//Clear SIC

}

void i2c_write_init(char slave_id, char locatn)
{
	i2c_start();
	i2c_addr(slave_id & 0XFE);	// Claering 8th bit only, select write operation
	i2c_location(locatn);		// Location in EEPROM to use
}

void i2c_dummy_write(char slave_id, char location)
{
	i2c_start();

	i2c_addr( slave_id & 0xFE);

	lcd_str("OK2");
	delay(500);
	cmd(0x01);

	
	i2c_location(location);
	i2c_stop();

} 

void i2c_read_init(char slave_id, char location)
{
	i2c_dummy_write(slave_id,location);
	
	i2c_start();
	i2c_addr2(slave_id | 0x01);	
}

int main()
{
	char ch;
	lcd_init();
	i2c_init();
	i2c_write_init(0xA0,0x00);

	
	for(ch='A';ch!='D';ch++)
	{
		i2c_write(ch);
	}
	i2c_stop();

	
	

	i2c_read_init(0xA0,0x00);
	
	
	

	do
	{
		ch = i2c_read();
		lcd_char(ch);

	}while(ch!='D');

	while(1);
}