#include<lpc213x.h>
#include "lcd.h"
#include "eeprom.h"

int main()
{
	char* str;
	lcd_init();

   	eeprom_write_str("1234");
	
	debug("Data Written!");

	debug("Reading data. .");
	
	str = eeprom_read_str();
	//ch = eeprom_read_char();
	lcd_str(str);
	//lcd_char(ch);
	
	while(1);
}