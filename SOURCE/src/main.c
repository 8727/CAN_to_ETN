#include "main.h"

struct __FILE { int handle; };
FILE __stdout;
int fputc(int ch, FILE *f){ if(DEMCR & TRCENA){ while(ITM_Port32(0x00) == 0x00);
  ITM_Port8(0x00) = ch; }
  return(ch);
}

int main(void){
  printf("\tHardware : %s\tSoftware : %s\r\n",HW_BUILD ,SW_BUILD);
  printf("Unique ID 0x%08x 0x%08x 0x%08x\r\n",IDCODE_1, IDCODE_2, IDCODE_3);
  Setting();
  while(0x01){
    DelayMs(100);
  }
}
