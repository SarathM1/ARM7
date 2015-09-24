buf_rx=read_string() is coded as follows
 
char *read_string(void){
static char buf_rx1[50]="";
int i;
unsigned char x=0;
while (!(U0LSR & 0x01));                        //wait for the response to come
  do                                    //read if character is available and pass time to ensure full data to be received
  {
    if((U0LSR & 0x01))                  //this count (i) actual depends on the serial baud rate and cpu clock  can 
    { 
        buf_rx1[x++]=U0RBR;             //count i roughly > CCLK/UART baud rage
        i=0;                            //60^6/9615 approx 6000
    }
    else
    {
      i++;
    }
  }while(i<6000);   
   return(buf_rx1);            
  }