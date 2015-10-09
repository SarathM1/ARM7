#include<lpc21xx.h>
void delay(int b);
char x=1;
char array1[64]={"INDIA IS MY COUNTRY AND ALL INDIANS ARE MY BROTHERS AND SISTERS"};    //SHIJU VARGHESE CHIRAKKALAKATHU  THADIKKADAVU	
char i,j,DATA=0;

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

void readdata(void)
{
	I2CONSET=0X04;
	I2CONCLR=0X28;
	while(I2STAT!=0X50);
	
	DATA=I2DAT; 
	U0THR=DATA;
	while((U0LSR & 0X20)!=0X20);
}

void delay(int b)
{
	while(b!=0)
	{
		b--;
	}
}

int main()
{
	IO0SET=0X0000000D;	   //SETTING SCL0,SDA0 & TX PIN SET FOR O/P
	PINSEL0=0X00000051;
	U0LCR   = 0X83;	       //	 transmiting 8 bit data (bit 1:0 =11 for 8bit data).7th bit should be
	U0DLL   = 0XC3;			//  baud rate register 9600 (lsb )
	U0DLM   = 0X00;	        //  baud rate (msb)
	U0LCR   = 0X03;
	I2CONCLR=0XFF;
	I2CONSET=0X44;		  // ENABLE THE I2C BY SETTING I2EN BIT & ACKNOWEDGE FLAG SET  
	I2SCLL=0X96;
	I2SCLH=0X96;

	while(1)
	{
		
		
		/************************WRITE*************************************************************/
		start();
		devadd1();
		location(0x00);
		for(i=0;i<=15;i++)	   //array[i]!='\0';
		{
			writedata1();
		}
		stop();
		delay(20000);
		/******************************************DEMMY WRITE**************************************/
		start();
		devadd1();
		location(0x00);
		stop();
		
		/******************************************READ********************************************/
		start();
		devadd2();
		
		for(j=0;j<=15;j++)
		{
			readdata();
		}
		
		stop();
		delay(150000);
		delay(150000);
		delay(150000);
		delay(150000);
	}
	
}

