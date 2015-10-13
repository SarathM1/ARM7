#include<lpc213x.h>
#include<stdlib.h>
#include "lcd.h"
#include "uart0_inter.h"

char  val[5];	
//int flag = 1;


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
	char ch;
	/***********************WRITING DATA*************************************/
	start();
	uart_tx_str("\r\nhere\r\n");
	devadd1();
	location(0x00);
	stop();
	
	/******************************************READ********************************************/
	start();
	devadd2();
	for(j=0;j<4;j++)
	{
		ch = readdata();
		val[j] = ch;
		if(ch == '\0')
			break;
	}
	stop();
	return val;
}

void uart(void)__irq // ISR for UART0
{
	char ch;
	char threshold[5];
	static int i;

	ch = U0RBR;	

	if(ch == '*')
	{
		i = 0;
		//flag = 0;
	}
	else if(ch == '#')
	{
		threshold[i] = '\0';
		eeprom_write_str(threshold);
		debug_str("Written to eeprom!!");
		delay(500);
		cmd(0x01);
		//flag = 1;	
	}
	else
	{
		threshold[i] = ch;
		i++;
	}
	
	VICVectAddr = 0;
}

int main()
{
	int thresh_val;
	i2c_init();
	uart_init();
	lcd_init();
	
	while(1)
	{
		   	uart_tx_str("\r\nReading from eeprom\r\n");
			thresh_val = atoi(eeprom_read_str());
			uart_tx_int(thresh_val);
			uart_tx_str("\r\nDONE!!\r\n");
			delay(500);
	}
}

