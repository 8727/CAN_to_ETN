#include "main.h"
uint8_t sntpBuff[MAX_SNTP_BUF_SIZE];

struct __FILE { int handle; };
FILE __stdout;
int fputc(int ch, FILE *f){ if(DEMCR & TRCENA){ while(ITM_Port32(0x00) == 0x00);
  ITM_Port8(0x00) = ch; }
  return(ch);
}

int main(void){
  printf("\tHardware : %s/tSoftware : %s\r\n",HW_BUILD ,SW_BUILD);
  printf("Unique ID 0x%08x 0x%08x 0x%08x\r\n",IDCODE_1, IDCODE_2, IDCODE_3);
  Setting();
  while(0x01){
    DelayMs(1000);
    dhcp_rutine();
    
    if(settings.lan > 0x00){
      printf("[SNTP] IP : %d.%d.%d.%d",settings.sntp[0],settings.sntp[1],settings.sntp[2],settings.sntp[3]);
      SNTP_init(W5500_SOCK_SNTP, settings.sntp, settings.timeZone, sntpBuff);
      while(SNTP_run() != 1);
      DelayMs(59000);
  }
  }
}
