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

void i2c0_write(char data,,unsigned char status)
{
	I2C0DAT 	= data;   // Data valid before other steps		#
	I2C0CONSET = (1<<2);
	I2C0CONCLR = (1<<3) | (1<<5);	// SIC = 1, STAC = 1		 #
	while(I2C0STAT != status);
}

char i2c0_read()
{
	I2C0CONSET = (1<<2);
	I2C0CONCLR = (1<<3) | (1<<5);	// SIC = 1, STAC = 1
	while(I2C0STAT != 0x50);
	return I2C0DAT;
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

void eeprom_write()
{
	i2c0_init();
	i2c0_start();
	i2c0_write(0xA0,0x18);	// Slave id = 0xA0, Wait while I2C0STAT != 0X18
	i2c0_write(0x00,0x28);		// Read from location 0x00, Wait while I2C0STAT != 0X28
	i2c0_write('A',0x28);	// Data = 'A' , wait  while I2C0STAT != 0X28
	i2c0_stop();
}

char eeprom_read()
{
	char data;

	/******DUMMY WRITE Start********/
	i2c0_init();
	i2c0_start();
	i2c0_write(0xA0,0x18);	// Slave id = 0xA0, R/W = 0, Wait while I2C0STAT != 0X18
	i2c0_write(0x00,0x28);		// Read from location 0x00, Wait while I2C0STAT != 0X28
	i2c0_stop();
	/******DUMMY WRITE End********/

	/******READ DATA Start********/
	i2c0_Start();	
	i2c0_Write(0xA1,0x40);	// Slave id = 0xA0, R/W = 1, Wait while I2C0STAT != 0X40
	data = i2c0_Read();
	i2c0_stop();
	/******READ DATA End********/

	return (data);

}

int main()
{
	lcd_init();

	eeprom_write();

	while(1);
}
