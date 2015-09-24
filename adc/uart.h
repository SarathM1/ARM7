 #include<lpc213x.h>

 void uart_init()
 {
 	PINSEL0 = 0x05; // Enable Txd0 on P0.0, Rxd0 on P0.1
	U0LCR = 0x83;

 	U0DLM = 0x00;
	U0DLL = 0xc3;

	U0LCR = 0x03;
 }

 void uart_tx_char(char ch)
 {
	U0THR = ch;
	while(!(U0LSR & 0x20));
 }

 void uart_tx_str(char ch[50])
 {
	unsigned int i;
	for(i=0;ch[i]!='\0';i++)
	{
		uart_tx_char(ch[i]);
	}
 
 }

 void uart_tx_num(int val)
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
	

	for(i=3;i>=0;i--)
	{
		uart_tx_char(a[i]);
	}
 
 
 }
