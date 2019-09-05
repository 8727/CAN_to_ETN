#include "main.h"
uint8_t sntpBuff[MAX_SNTP_BUF_SIZE];

struct __FILE { int handle; };
FILE __stdout;
int fputc(int ch, FILE *f){ if(DEMCR & TRCENA){ while(ITM_Port32(0x00) == 0x00);
  ITM_Port8(0x00) = ch; }
  return(ch);
}

int main(void){
  Setting();
  while(0x01){
    DelayMs(1000);
    dhcp_rutine();
    
    if(settings.lan > 0x00){
      printf("[SNTP] IP : %d.%d.%d.%d\n\r",settings.sntp[0],settings.sntp[1],settings.sntp[2],settings.sntp[3]);
  SNTP_init(W5500_SOCK_SNTP, settings.sntp, settings.timeZone, sntpBuff);
  uint32_t time;
  do{}
    while(SNTP_run(&time) != 1);
    printf("Date %d\r\n\r\n", time);
  
  #if defined DEBUG_ETHERNET
    printf("< OK >    Initialization SNTP\r\n");
  #endif
    DelayMs(100000);
  }
  }
}
