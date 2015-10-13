#include<lpc213x.h>
#include<stdlib.h>
#include "lcd.h"
#include "uart0_inter.h"
#include "adc.h"


int flag = 0;

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

void writedata(char ch)
{
	I2C0DAT=ch;
	I2C0CONSET=0X04;
	I2C0CONCLR=0X08;
	while(I2C0STAT != 0X28);
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

void eeprom_write_str(char* str)
{
	int k;
	start();
	uart_tx_char('?');
	devadd1();
	location(0x00);
	for(k=0;str[k]!='\0';k++)	   //array[i]!='\0';
	{
		writedata(str[k]);
		cmd(0xCA);
		debug_int(k);
	}
	stop();
	delay(2);		  // delay 2 ms. I2c won't work if removed
}

void eeprom_read_str()
{
	int k;
	char *val;	
	/***********************DUMMY WRITE *************************************/
	start();
	devadd1();
	location(0x00);
	stop();
	
	/******************************************READ********************************************/
	start();
	devadd2();
	for(k=0;k<4;k++)
	{
		val[k] = readdata();
	}
	val[k]='\0';
	stop();
	delay(500);
	
	debug_str(val);
	//return val;
}

//void eeprom_write()
//{
//	int k;
//	start();
//	devadd1();
//	location(0x00);
//	for(k=0;k<=15;k++)	   
//	{
//		writedata();
//	}
//	stop();
//	delay(2);		  // delay 2 ms. I2c won't work if removed
//}

//void eeprom_read()
//{
//	int k;	
//	/***********************DUMMY WRITE *************************************/
//	start();
//	devadd1();
//	location(0x00);
//	stop();
//	
//	/******************************************READ********************************************/
//	start();
//	devadd2();
//	for(k=0;k<=64;k++)
//	{
//		uart_tx_char(readdata());
//	}
//	stop();
//}


void uart(void)__irq // ISR for UART0
{
	static int i = 0;
	char threshold[5];    

	char ch;
	ch = U0RBR; 		// Echo what is recieved
	
	uart_tx_char(ch);
	
	if(ch=='*')
	{
		i=0;
		flag=1;
	}
	else if(ch=='#')
	{
		flag=0;
		threshold[i]='\0';
		eeprom_write_str(threshold);
		
		//debug_str(threshold);
	}
	else
	{
		threshold[i++]=ch;		
	}		
	
	VICVectAddr = 0;	// Compulsary in all ISR's
}


int main()
{
	int adc_val,thresh_val;
	lcd_init();
	adc_init();
	uart_init();
	i2c_init();

	//thresh_val = atoi(eeprom_read_str());
	//thresh_val =  atoi("1234");
	eeprom_read_str();
//	debug_int(thresh_val);
	while(1);

	while(1)
	{
		if(flag == 0)
		{
			adc_val = adc_read();

			if(adc_val == thresh_val)
			{
				cmd(0x80);
				lcd_str("ALARM!!");
			}
			else
			{
				cmd(0x80);
				lcd_str(". . .");
			}
		}	
	}
}

