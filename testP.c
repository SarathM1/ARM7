#include<lpc21xx.h>
void delay(unsigned int x)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
		for(j=0;j<10000;j++);

}
int main()
{
	//long int x = 0x00010000;
	unsigned int i = 0;
	IODIR1 = 0x00FF0000;
	  	IO1SET = 0x00010000;
	while(1)
	{
		for(i=0;i<7;i++)
		{	  delay(500);
			IO1SET = IO1SET<<1;
			//delay(500);
			IO1CLR = IO1SET>>1;
		 	
		}
		
		IO1SET = 0x00010000;
	}

}