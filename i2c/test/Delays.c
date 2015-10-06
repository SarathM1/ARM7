
/*
Author:-	Embedded Laboratory
Delay Functions C File
*/

#include "Delays.h"

/****************Delay Function***************/
void Delay_ms(unsigned long times)
{
	unsigned long i,j;
	for(j=0;j<times;j++)
		for(i=0;i<7500;i++);
}
/*********************************************/
