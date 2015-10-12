 
 void adc_init()
 {
 	PINSEL1 = 1<<24; // P0.28 as  AD0.1
	//AD0CR =  0x00200602;
	AD0CR = (1<<1)|(1<<9)|(1<<10)|(1<<21);

 }

 int adc_read()
 {
 	int val;
	AD0CR |= (1<<24); //Start conversion

	while( !((AD0DR >>31)&1) );  // Waiting for adc
	
	val = (AD0DR & 0x0000FFC0) >> 6;
	return val;
 }
