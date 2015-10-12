
void i2c0_init()
{
	//IO0SET=0X0000000D;	   //SETTING SCL0,SDA0 & TX PIN SET FOR O/P
	PINSEL0 = (1<<4)|(1<<6);
	//PINSEL0=0X00000051;
	I2C0CONCLR = 0X2C;   //Clear AA, I2C Interrupt Flag and Start Bit	 #
	//I2CONCLR=0XFF;
	I2C0CONSET = (1<<6)|(1<<2);	// I2EN =1, AA=1					#
	I2C0SCLH	= 0X96;
	I2C0SCLL	= 0X96;
}

void i2c0_start()
{
	I2C0CONSET = (1<<5)|(1<<3); // STA =1 				#
	I2C0CONCLR = (1<<3);
	
	debug_int(I2C0STAT);
	while(I2C0STAT!=0X08); // wait for acknowlege from slave
}

void i2c0_stop()
{
	I2C0CONSET = (1<<2) | (1<<4);	 // AA = 1, STO = 1
	I2C0CONCLR = (1<<3);			//Clear SIC
}

void i2c0_write(char data,unsigned char status)
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





void eeprom_write_char(unsigned char ch)
{
	i2c0_init();
	i2c0_start();
	i2c0_write(0xA0,0x18);	// Slave id = 0xA0, Wait while I2C0STAT != 0X18
	i2c0_write(0x00,0x28);		// Read from location 0x00, Wait while I2C0STAT != 0X28
	i2c0_write(ch,0x28);	// Data = ch , wait  while I2C0STAT != 0X28
	i2c0_stop();
}

char eeprom_read_char()
{
	char ch;
	/******DUMMY WRITE Start********/
	i2c0_init();
	i2c0_start();
	i2c0_write(0xA0,0x18);	// Slave id = 0xA0, R/W = 0, Wait while I2C0STAT != 0X18
	i2c0_write(0x00,0x28);		// Read from location 0x00, Wait while I2C0STAT != 0X28
	i2c0_stop();
	/******DUMMY WRITE End********/

	/******READ DATA Start********/
	i2c0_start();	
	i2c0_write(0xA1,0x40);	// Slave id = 0xA0, R/W = 1, Wait while I2C0STAT != 0X40
	
	ch = i2c0_read();
	i2c0_stop();
	
	/******READ DATA End********/
	return ch;
}

char* eeprom_read_str()
{
	unsigned int i=0;
	char* data;

	/******DUMMY WRITE Start********/
	//i2c0_init();
	debug("start");
	i2c0_start();
	debug("2");
	i2c0_write(0xA0,0x18);	// Slave id = 0xA0, R/W = 0, Wait while I2C0STAT != 0X18
	i2c0_write(0x00,0x28);		// Read from location 0x00, Wait while I2C0STAT != 0X28
	i2c0_stop();
	/******DUMMY WRITE End********/
	
	/******READ DATA Start********/
	i2c0_start();	
	i2c0_write(0xA1,0x40);	// Slave id = 0xA0, R/W = 1, Wait while I2C0STAT != 0X40
	

	do{
		data[i] = i2c0_read();
		i=i+1;
		
	}while(data[i-1]!='\0');

	data[i-1] = '\0';
	i2c0_stop();
	
	/******READ DATA End********/
	delay(500);
	return data;
}

void eeprom_write_str(char *str)
{
	unsigned int i;
	//i2c0_init();
	i2c0_start();
	i2c0_write(0xA0,0x18);	// Slave id = 0xA0, Wait while I2C0STAT != 0X18
	i2c0_write(0x00,0x28);		// Read from location 0x00, Wait while I2C0STAT != 0X28
	
	for(i=0;str[i]!='\0';i++)
		i2c0_write(str[i],0x28);	// Data = ch , wait  while I2C0STAT != 0X28
	
	i2c0_write('\0',0x28);	// Data = ch , wait  while I2C0STAT != 0X28

	i2c0_stop();
}

