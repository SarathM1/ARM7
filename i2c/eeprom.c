#include<lpc21xx.h>
#include "lcd.h"

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
	I2C0CONSET = (1<<5); // STA =1 				#
	while(I2C0STAT!=0X08); // wait for acknowlege from slave
}

void i2c0_stop()
{
	I2C0CONSET = (1<<2) | (1<<4);	 // AA = 1, STO = 1
	I2C0CONCLR = (1<<3);			//Clear SIC
}

void i2c0_addr(char slave_addr)
{
	I2C0CONSET = (1<<2); // AA =1
	I2C0CONCLR = (1<<3)|(1<<5);	// SIC = 1
	I2C0DAT = slave_addr;
	while(I2C0STAT != 0X18);	
}

void i2c0_addr2(char slave_addr)
{
	I2C0CONSET = (1<<2); // AA =1
	I2C0CONCLR = (1<<3);	// SIC = 1
	I2C0DAT = slave_addr;
	while(I2C0STAT != 0X40);
}

void i2c0_write(char data)
{
	I2C0DAT 	= data;   // Data valid before other steps		#
	I2C0CONSET = (1<<2);
	I2C0CONCLR = (1<<3) | (1<<5);	// SIC = 1, STAC = 1		 #
	while(I2C0STAT != 0X28);
}

char i2c0_read()
{
	I2C0CONSET = (1<<2);
	I2C0CONCLR = (1<<3) | (1<<5);	// SIC = 1, STAC = 1
	while(I2C0STAT != 0x50);
	return I2C0DAT;
}
void i2c0_location(char loc_addr)
{
	I2C0CONSET = (1<<2);
	I2C0CONCLR = (1<<3)|(1<<5);	// SIC = 1 , STAC = 1
	I2C0DAT 	= loc_addr;
	while(I2C0STAT != 0X28);

}



void i2c0_write_init(char slave_id, char locatn)
{
	i2c0_start();
	i2c0_addr(slave_id & 0XFE);	// Claering 8th bit only, select write operation
	i2c0_location(locatn);		// Location in EEPROM to use
}

void i2c0_dummy_write(char slave_id, char location)
{
	i2c0_start();

	i2c0_addr( slave_id & 0xFE);

	lcd_str("OK2");
	delay(500);
	cmd(0x01);

	
	i2c0_location(location);
	i2c0_stop();

} 

void i2c0_read_init(char slave_id, char location)
{
	i2c0_dummy_write(slave_id,location);
	
	i2c0_start();
	i2c0_addr2(slave_id | 0x01);	
}

int main()
{
	char ch;
	lcd_init();
	i2c0_init();
	i2c0_write_init(0xA0,0x00);

	
	for(ch='A';ch!='D';ch++)
	{
		i2c0_write(ch);
	}
	i2c0_stop();

	
	

	i2c0_read_init(0xA0,0x00);
	
	
	

	do
	{
		ch = i2c0_read();
		lcd_char(ch);

	}while(ch!='D');

	while(1);
}
