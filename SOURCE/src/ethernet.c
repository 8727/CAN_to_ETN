#include "ethernet.h"

wiz_NetInfo gWIZNETINFO;
uint8_t dhcpBuff[W5500_DATA_BUF_SIZE];
uint8_t my_dhcp_retry = 0x00;

void EthernetCsLOW(void){ ETHERNET_CS_LOW; }
void EthernetCsHIGHT(void){ ETHERNET_CS_HIGHT; }

uint8_t EthernetReadByte(void){
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = 0xFF;
  while(!(SPI1->SR & SPI_SR_RXNE));
  return SPI1->DR;
}

void EthernetWriteByte(uint8_t byte){
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = byte;
  while(!(SPI1->SR & SPI_SR_RXNE));
  (void)SPI1->DR;
}

void EthernetInfo(void){
  settings.ethernet |= 0x40;
  #ifdef DEBUG_ETHERNET
    uint8_t tmpstr[0x06] = {0,};
    wiz_NetInfo info;
    ctlwizchip(CW_GET_ID,(void*)tmpstr); // Get WIZCHIP name
    wizchip_getnetinfo(&info);
    printf("\r\n\t=======================================\r\n");
    printf("\t    WIZnet chip:  %s     ", tmpstr);
    if(info.dhcp == NETINFO_STATIC){ printf("STATIC IP\r\n"); }else{ printf("DHCP IP\r\n"); }
    printf("\t=======================================\r\n");
    printf("\tMac address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",info.mac[0],info.mac[1],info.mac[2],info.mac[3],info.mac[4],info.mac[5]);
    printf("\tIP address : %d.%d.%d.%d\r\n",info.ip[0],info.ip[1],info.ip[2],info.ip[3]);
    printf("\tSM Mask	   : %d.%d.%d.%d\r\n",info.sn[0],info.sn[1],info.sn[2],info.sn[3]);
    printf("\tGate way   : %d.%d.%d.%d\r\n",info.gw[0],info.gw[1],info.gw[2],info.gw[3]);
    printf("\tDNS Server : %d.%d.%d.%d\r\n",info.dns[0],info.dns[1],info.dns[2],info.dns[3]);
    printf("\t=======================================\r\n");
  #endif
}

void EthernetDhcpRutine(void){
  switch(DHCP_run()){
    case DHCP_IP_ASSIGN://можем сделать что-то сдесь когда мы получили ip
    case DHCP_IP_CHANGED://можем сделать что-то сдесь при изменениее ip
    break;
    case DHCP_IP_LEASED:
    break;
    case DHCP_RUNNING:
    case DHCP_FAILED://ошибка получения ip адреса от DHCP
      my_dhcp_retry++;//кол-во попыток
      if(my_dhcp_retry > (0x0F & settings.ethernet)){
        #ifdef DEBUG_ETHERNET
          printf(">> DHCP %d Failed\r\n", my_dhcp_retry);
        #endif
        my_dhcp_retry = false;
        DHCP_stop();      // if restart, recall DHCP_init()
        gWIZNETINFO.dhcp = NETINFO_STATIC;
        ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);   // назначаем статический ip
        EthernetInfo();
      }
     break;
     default:
     break;
  }
}

void EthernetIPAssign(void){//будет вызвана при первом назначении IP от DHCP сервера
  getIPfromDHCP(gWIZNETINFO.ip);//получаем ip от DHCP
  getGWfromDHCP(gWIZNETINFO.gw);//адрес шлюза
  getSNfromDHCP(gWIZNETINFO.sn);//маска под сети 
  getDNSfromDHCP(gWIZNETINFO.dns);//адрес DNS 
  gWIZNETINFO.dhcp = NETINFO_DHCP;
  /* Network initialization */
  ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);  // apply from dhcp
  EthernetInfo();
  #ifdef DEBUG_ETHERNET
    printf("\tDHCP LEASED TIME : %ld Sec.\n\r\n", getDHCPLeasetime());//получить время аренды на сервере DHCP
  #endif
}

void EthernetIpConflict(void){
  #ifdef DEBUG_ETHERNET
    printf("CONFLICT IP from DHCP\r\n");
  #endif
  //halt or reset or any...
  while(1); // this example is halt.
}

void EthernetPHYLINK(void){
  uint8_t temp;
  do{
    ctlwizchip(CW_GET_PHYLINK, (void*)&temp);
    if(temp == PHY_LINK_OFF){
      #if defined DEBUG_ETHERNET
      DelayMs(0x1F4);
        printf("Unknown PHY link status.\r\n");
      #endif
    }
  }while(temp == PHY_LINK_OFF);
  settings.ethernet |= 0x80;
}

void EthernetSettings(void){
  ETHERNET_RESET_LOW;
  DelayMs(0x01);
  ETHERNET_RESET_HIGHT;
  DelayMs(0x01);
  
  uint8_t W5500FifoSize[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2, }, {2, 2, 2, 2, 2, 2, 2, 2}};

  EthernetCsHIGHT();
 
  reg_wizchip_spi_cbfunc(EthernetReadByte, EthernetWriteByte); /*передаем функции чтения записи драйверу */
  reg_wizchip_cs_cbfunc(EthernetCsLOW, EthernetCsHIGHT); /* CS function register */

  if(ctlwizchip(CW_INIT_WIZCHIP, (void*)W5500FifoSize) == -1){
    #if defined DEBUG_ETHERNET
      printf("W5500 initialized fail.\r\n");
    #endif
    while(1);
  }
  EthernetPHYLINK();
  setSHAR(gWIZNETINFO.mac); //настройка MAC
  if(gWIZNETINFO.dhcp == NETINFO_DHCP){
    DHCP_init(W5500_SOCK_DHCP, dhcpBuff);//передаем номер сокета  
    reg_dhcp_cbfunc(EthernetIPAssign, EthernetIPAssign, EthernetIpConflict);//передаем функции
    settings.ethernet |= 0x20;
  }else{
    gWIZNETINFO.dhcp = NETINFO_STATIC;
    ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);   // назначаем статический ip
    EthernetInfo();
  }
  
  #if defined DEBUG_ETHERNET
    printf("< OK >    Initialization ethernet\r\n");
  #endif
}

void EthernetInit(void){
  GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
  GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;
  GPIOA->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7;   // GPIO SPI Ethernet
  
  GPIOB->CRL &= ~(GPIO_CRL_CNF1);
  GPIOB->CRL |= GPIO_CRL_MODE1; // reset
  
  ETHERNET_RESET_HIGHT;
  ETHERNET_CS_HIGHT;
  
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  SPI1->CR1 &= ~SPI_CR1_BR;
  SPI1->CR1 &= ~SPI_CR1_CPOL;
  SPI1->CR1 &= ~SPI_CR1_CPHA;
  SPI1->CR1 &= ~SPI_CR1_DFF;
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI1->CR1 |= SPI_CR1_MSTR;
  SPI1->CR1 |= SPI_CR1_SPE;     // SPI Ethernet
  
  #if defined DEBUG_ETHERNET
    printf("< OK >    Initialization SPI Ethernet\r\n");
  #endif
}
