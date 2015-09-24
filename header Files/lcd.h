void delay(unsigned int x)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
		for(j=0;j<10000;j++);
}

void lcd_char(char ch)
{
	IO1CLR = 0xffffffff;

	IO1SET = 0x000A0000;
	IO1CLR = 0x00040000;
	IO1SET = IO1SET | ( (ch & 0xf0) << 16 );
	delay(1);
	IO1CLR = 0x00080000;

	IO1CLR = 0xffffffff;

	IO1SET = 0x000A0000;
	IO1CLR = 0x00040000;
	IO1SET = IO1SET | ( (ch & 0x0F) << 20 );
	delay(1);
	IO1CLR = 0x00080000;

	IO1CLR = 0xffffffff;
	
}

void dispInt(int val)
{
	char ch;
	unsigned char a[5];
	int i = 0;
	for(i=0;i<=3;i++)
	{
		ch =  (val%10) ;
		a[i] = ch+48;
		
		val = val / 10;
	}
	//a[i]= '\0';

	for(i=3;i>=0;i--)
	{
		data(a[i]);
	}
}

void lcd_str(char ch[20])
{
	unsigned int i;
	for(i=0;ch[i]!='\0';i++)
		data(ch[i]);

}
void write_command(char ch)
{
	IO1CLR = 0xffffffff;	

	IO1CLR = 0x00060000;
	IO1SET = 0X00080000;
	IO1SET = IO1SET | ((ch & 0xf0)<<16);
	delay(1);
	IO1CLR = 0x00080000;

	
}
void cmd(char ch)
{
	IO1CLR = 0xffffffff;	

	IO1CLR = 0x00060000;
	IO1SET = 0X00080000;
	IO1SET = IO1SET | ((ch & 0xf0)<<16);
	delay(1);
	IO1CLR = 0x00080000;

	IO1CLR = 0xffffffff;

	IO1CLR = 0x00060000;
	IO1SET = 0X00080000;
	IO1SET = IO1SET | ((ch & 0x0f)<<20);
	delay(1);
	IO1CLR = 0x00080000;

	IO1CLR = 0xffffffff;

}
void lcd_init()
{
	  IODIR1 = 0x00FE0000;
	  write_command(0x30); 
	  delay(1);
	  write_command(0x30);
	  delay(1);
	  write_command(0x30);
	  delay(1);
	  write_command(0x20);


	cmd(0x28);  // 4 bit mode
	cmd(0x06);	// Auto incre
	cmd(0x0c);  // Cursor off
	cmd(0x01);	// LCD Clear
	
}