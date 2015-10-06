
/*
Author:-	Embedded Laboratory
UART Functions Header File
*/

#include <LPC214X.H>
/**************Function Prototypes************/
void UART0_Init(void);
void UART0_Write(unsigned char value);
void UART0_Write_Text(unsigned char *msg);
unsigned char UART0_Read(void);

void UART1_Init(void);
void UART1_Write(unsigned char value);
void UART1_Write_Text(unsigned char *msg);
unsigned char UART1_Read(void);
/*********************************************/
