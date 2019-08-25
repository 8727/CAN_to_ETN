#include "ethernet.h"

wiz_NetInfo netInfo;
wiz_NetInfo defNet;
uint8_t DHCPDATABUF[W5500_DATA_BUF_SIZE];

void EthernetCsLow(void){ ETHERNET_CS_LOW; }

void EthernetCsHight(void){ ETHERNET_CS_HIGHT; }

void EthernetReadBuff(uint8_t* buff, uint16_t len){
//  HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

void EthernetWriteBuff(uint8_t* buff, uint16_t len){
//  HAL_SPI_Transmit(&hspi1, buff, len, HAL_MAX_DELAY);
}

uint8_t EthernetReadByte(void){
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = 0xFF;
  while(!(SPI1->SR & SPI_SR_RXNE));
  return SPI1->DR;
}

void EthernetWriteByte(uint8_t byte){
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = 0xFF;
  while(!(SPI1->SR & SPI_SR_RXNE));
}

//uint8_t EthernetWriteRead(uint8_t byte){
//  while(!(SPI1->SR & SPI_SR_TXE));
//  SPI1->DR = byte;
//  while(!(SPI1->SR & SPI_SR_RXNE));
//  return SPI1->DR;
//}

void EthernetNetwork(void){
  uint8_t tmpstr[6] = {0,};
  wiz_NetInfo info;
  ctlnetwork(CN_SET_NETINFO, (void*)&defNet);// Set Network information from netinfo structure
  #ifdef ETHERNET
    ctlnetwork(CN_GET_NETINFO, (void*)&info); // Get Network information
    ctlwizchip(CW_GET_ID,(void*)tmpstr); // Display Network Information
    if(info.dhcp == NETINFO_DHCP) printf("\r\n=== %s NET CONF : DHCP ===\r\n",(char*)tmpstr);
    else printf("\r\n=== %s NET CONF : Static ===\r\n",(char*)tmpstr);
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",info.mac[0x00],info.mac[0x01],info.mac[0x02],
    info.mac[0x03],info.mac[0x04],info.mac[0x05]);
    printf("SIP: %d.%d.%d.%d\r\n", info.ip[0x00],info.ip[0x01],info.ip[0x02],info.ip[0x03]);
    printf("GAR: %d.%d.%d.%d\r\n", info.gw[0x00],info.gw[0x01],info.gw[0x02],info.gw[0x03]);
    printf("SUB: %d.%d.%d.%d\r\n", info.sn[0x00],info.sn[0x01],info.sn[0x02],info.sn[0x03]);
    printf("DNS: %d.%d.%d.%d\r\n", info.dns[0x00],info.dns[0x01],info.dns[0x02],info.dns[0x03]);
    printf("===========================\r\n");
  #endif
}

void EthernetIpAssign(void){      //будет вызвана при первом назначении IP от DHCP сервера
  getIPfromDHCP(netInfo.ip);      //получаем ip от DHCP
  getGWfromDHCP(netInfo.gw);      //адрес шлюза
  getSNfromDHCP(netInfo.sn);      //маска под сети 
  getDNSfromDHCP(netInfo.dns);    //адрес DNS 
  netInfo.dhcp = NETINFO_DHCP;
  EthernetNetwork();              // apply from dhcp
  settings.dhcpSt = true;
  #ifdef ETHERNET
     printf("DHCP LEASED TIME : %ld Sec.\r\n", (long)getDHCPLeasetime());//получить время аренды на сервере DHCP
  #endif
}

void EthernetIpConflict(void){
  #ifdef ETHERNET
    printf("CONFLICT IP from DHCP\r\n");
  #endif
   while(1); // this example is halt.
}

void EthernetDhcpRutine(void){
  uint8_t my_dhcp_retry = 0x00;
  switch(DHCP_run()){
    case DHCP_IP_ASSIGN://можем сделать что-то сдесь когда мы получили ip
    case DHCP_IP_CHANGED://можем сделать что-то сдесь при изменениее ip
    break;
    case DHCP_IP_LEASED:
    break;
    case DHCP_FAILED://ошибка получения ip адреса от DHCP
      my_dhcp_retry++;//кол-во попыток
      if(my_dhcp_retry > settings.dhcpOn){
        #ifdef ETHERNET
          printf(">> DHCP %d Failed\r\n", my_dhcp_retry);
        #endif
        my_dhcp_retry = 0x00;
        DHCP_stop();         // if restart, recall DHCP_init()
        EthernetNetwork();   // назначаем статический ip
      }
    break;
    default:
    break;
  }
}

void EthernetInit(void){
  uint8_t temp;
  uint8_t bufSize[] = {0x02, 0x02, 0x02, 0x02};
  
  GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
  GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;
  GPIOA->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7;   // GPIO SPI Ethernet
  
  GPIOB->CRL &= (GPIO_CRL_CNF1);
  GPIOB->CRL |= GPIO_CRL_MODE1; // reset
  
  ETHERNET_RESET_HIGHT;
  
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  SPI1->CR1 &= ~SPI_CR1_BR;
  SPI1->CR1 &= ~SPI_CR1_CPOL;
  SPI1->CR1 &= ~SPI_CR1_CPHA;
  SPI1->CR1 &= ~SPI_CR1_DFF;
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI1->CR1 |= SPI_CR1_MSTR;
  SPI1->CR1 |= SPI_CR1_SPE;     // SPI Ethernet
  
  DelayMs(0x01);
  ETHERNET_RESET_LOW;
  DelayMs(0x02);
  ETHERNET_RESET_HIGHT;
  
  reg_wizchip_cs_cbfunc(EthernetCsLow, EthernetCsHight);
  reg_wizchip_spi_cbfunc(EthernetReadByte, EthernetWriteByte);
  reg_wizchip_spiburst_cbfunc(EthernetReadBuff, EthernetWriteBuff);
  
  wizchip_init(bufSize, bufSize);
  do{
    if(ctlwizchip(CW_GET_PHYLINK, (void*)&temp) == -1){
      #ifdef ETHERNET
        printf("Unknown PHY link status.\r\n");
      #endif
    }
  }while(temp == PHY_LINK_OFF);
  wiz_NetInfo netInfo;
  for(uint8_t i = 0x00; i < 0x06; i++){
    netInfo.mac[i] = defNet.mac[i]; // MAC адрес
  }
  netInfo.dhcp = NETINFO_DHCP; 
  setSHAR(netInfo.mac); //настройка
  if(0x00 < settings.dhcpOn){
    DHCP_init(W5500_SOCK_DHCP, DHCPDATABUF); //передаем номер сокета
    reg_dhcp_cbfunc(EthernetIpAssign, EthernetIpAssign, EthernetIpConflict); //передаем функции
  }else{
    EthernetNetwork();
  }
  #if defined ETHERNET
    printf("< OK >    Initialization ethernet\r\n");
  #endif
}
