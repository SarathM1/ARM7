 #include<lpc213x.h>

 void uart_init()
 {
 	PINSEL0 = 0x05; // Enable Txd0 on P0.0, Rxd0 on P0.1
	U0LCR = 0x83;

 	U0DLM = 0x00;
	U0DLL = 0xc3;

	U0LCR = 0x03;
 }

 void uart_char(char ch)
 {
	U0THR = ch;
	while(!(U0LSR & 0x20));
 }

 void uart_str(char ch[50])
 {
	unsigned int i;
	for(i=0;ch[i]!='\0';i++)
	{
		uart_char(ch[i]);
	}
 
 }

 int main()
 {
	uart_init();
	uart_str("Hello World");
	while(1);
 }
