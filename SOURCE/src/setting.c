#include "setting.h"

extern wiz_NetInfo gWIZNETINFO;
struct settingsInitTypeDef settings;
struct sntpInitTypeDef sntp;
static __IO uint32_t msTicks;

void SysTick_Handler(void){ msTicks++; }

uint32_t GetTick(void){ return msTicks; }

void DelayMs(uint32_t ms){ uint32_t tickstart = GetTick();
  while((GetTick() - tickstart) < ms){}
}

void DelayMc(uint32_t mc){ mc *= (SystemCoreClock / 1000000) / 9;
  while (mc--);
}

void WriteData32ToBuffer(uint8_t addr, uint32_t data, uint8_t* buff){
  buff[addr + 0x03] = (data >> 0x18);
  buff[addr + 0x02] = (data >> 0x10);
  buff[addr + 0x01] = (data >> 0x08);
  buff[addr] = data; 
}

void WriteData16ToBuffer(uint8_t addr, uint16_t data, uint8_t* buff){
  buff[addr + 0x01] = (data >> 0x08);
  buff[addr] = data;
}

uint32_t ReadData32Buffer(uint8_t addr, uint8_t* buff){
  uint32_t data = buff[addr + 0x03] << 0x18;
  data |= buff[addr + 0x02] << 0x10;
  data |= buff[addr + 0x01] << 0x08;
  data |= buff[addr]; 
  return data;
}

uint16_t ReadData16Buffer(uint8_t addr, uint8_t* buff){
  uint16_t data = buff[addr + 0x01] << 0x08;
  data |= buff[addr];
  return data;
}

void CopyBeffer(uint8_t* receiver ,uint8_t* source, uint8_t addr, uint8_t len){
  for(uint8_t i = 0x00; i < len; i++){ receiver[i] = source[addr + i];}
}

void ReadConfig(void){
  #if defined DEBUG_SETTING
    printf("< OK >    Read configuration\r\n");
  #endif
  uint8_t buffEeprom[EEPROM_BUFF];
  uint8_t i;
  Ee24cxxRead(buffEeprom);
  if(0xFF != buffEeprom[ADDR_STATUS]){
    RtcTypeDef dateBuild;
    dateBuild.year  = BUILD_YEAR;
    dateBuild.month = BUILD_MONTH;
    dateBuild.day   = BUILD_DAY;
    dateBuild.hour  = BUILD_TIME_H;
    dateBuild.min   = BUILD_TIME_M;
    dateBuild.sec   = BUILD_TIME_S;
    WriteData32ToBuffer(ADDR_DATE_BUILD, (RtcTimeToSeconds(&dateBuild)) - RtcTimeZoneToSeconds(TIME_ZONE), buffEeprom);
    buffEeprom[ADDR_STATUS] = STATUS;
    buffEeprom[ADDR_DEVICE_TYPE] = DEVICE_TYPE;
    buffEeprom[ADDR_DEVICE_NUMBER] = DEVICE_NUMBER;
    buffEeprom[ADDR_RTC_CALIBRATION] = RTC_CALIBRATION;
    WriteData32ToBuffer(ADDR_CAN_SPEED, CAN_SPEED, buffEeprom);
    WriteData16ToBuffer(ADDR_RS485_SPEED, RS485_SPEED, buffEeprom);
    
    buffEeprom[ADDR_LCD_ROTATION] = LCD_ROTATION;
    buffEeprom[ADDR_TIME_ZONE] = TIME_ZONE;
    //RF24L01
    WriteData16ToBuffer(ADDR_RF24_ADDR, RF24_ADDR, buffEeprom);
    buffEeprom[ADDR_RF24_PRIM] = RF24_PRIM;
    buffEeprom[ADDR_RF24_SECON] = RF24_SECON;
    buffEeprom[ADDR_RF24_SPEED] = RF24_SPEED;
    buffEeprom[ADDR_RF24_POWER] = RF24_POWER;
    buffEeprom[ADDR_RF24_CH] = RF24_CH;
    buffEeprom[ADDR_RF24_TYPE_ON] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_SEND_1] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ADDR_1] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_SEND_2] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ADDR_2] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_SEND_3] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ADDR_3] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_SEND_4] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ADDR_4] = 0x00;
    
/*----------------------------------------------------------------------------*/
    WriteData32ToBuffer(ADDR_W5500_MAC +0x02, (IDCODE_2 & 0xFCFFFF00), buffEeprom);
    WriteData16ToBuffer(ADDR_W5500_MAC, RF24_ADDR, buffEeprom);
    buffEeprom[ADDR_W5500_ETHERNET] = W5500_DHCP;
    buffEeprom[ADDR_UPDATE_SNTP] = UPDATE_SNTP;
    WriteData32ToBuffer(ADDR_W5500_IP, ReadData32Buffer(0x00, (uint8_t*)IP_ADDR), buffEeprom);
    WriteData32ToBuffer(ADDR_W5500_SN, ReadData32Buffer(0x00, (uint8_t*)IP_MASK), buffEeprom);
    WriteData32ToBuffer(ADDR_W5500_GW, ReadData32Buffer(0x00, (uint8_t*)IP_GATE), buffEeprom);
    WriteData32ToBuffer(ADDR_W5500_SEND, ReadData32Buffer(0x00, (uint8_t*)IP_SEND), buffEeprom);
    WriteData32ToBuffer(ADDR_W5500_NTP_P, ReadData32Buffer(0x00, (uint8_t*)IP_NTP_P), buffEeprom);
    WriteData32ToBuffer(ADDR_W5500_NTP_S, ReadData32Buffer(0x00, (uint8_t*)IP_NTP_S), buffEeprom);


/*----------------------------------------------------------------------------*/
    Ee24cxxWrite(buffEeprom);
    
    #if defined DEBUG_SETTING
      printf("          Default configuration\r\n");
    #endif
  }
  settings.dateBuild = ReadData32Buffer(ADDR_DATE_BUILD, buffEeprom);
  settings.type = buffEeprom[ADDR_DEVICE_TYPE];
  settings.number = buffEeprom[ADDR_DEVICE_NUMBER];
  settings.rtcCalib = buffEeprom[ADDR_RTC_CALIBRATION];
  
  settings.canSpeed = ReadData32Buffer(ADDR_CAN_SPEED, buffEeprom);
  settings.rs485Speed = ReadData16Buffer(ADDR_RS485_SPEED, buffEeprom);
  settings.canDevice = (settings.type << 0x08) | (settings.number << 0x04);
  settings.timeZone = TIME_ZONE;
  //RF24L01
  settings.rf24Addr = ReadData16Buffer(ADDR_RF24_ADDR, buffEeprom);
  settings.rf24Prim = buffEeprom[ADDR_RF24_PRIM];
  settings.rf24Secon = buffEeprom[ADDR_RF24_SECON];
  settings.rf24Speed = buffEeprom[ADDR_RF24_SPEED];
  settings.rf24Power = buffEeprom[ADDR_RF24_POWER];
  settings.rf24Ch = buffEeprom[ADDR_RF24_CH];
  settings.rf24TypeOn = buffEeprom[ADDR_RF24_TYPE_ON];
  settings.rf24TypeSend1 = buffEeprom[ADDR_RF24_TYPE_SEND_1];
  settings.rf24TypeAddr1 = buffEeprom[ADDR_RF24_TYPE_ADDR_1];
  settings.rf24TypeSend2 = buffEeprom[ADDR_RF24_TYPE_SEND_2];
  settings.rf24TypeAddr2 = buffEeprom[ADDR_RF24_TYPE_ADDR_2];
  settings.rf24TypeSend3 = buffEeprom[ADDR_RF24_TYPE_SEND_3];
  settings.rf24TypeAddr3 = buffEeprom[ADDR_RF24_TYPE_ADDR_3];
  settings.rf24TypeSend4 = buffEeprom[ADDR_RF24_TYPE_SEND_4];
  settings.rf24TypeAddr4 = buffEeprom[ADDR_RF24_TYPE_ADDR_4];
  
/*----------------------------------------------------------------------------*/
  for(i = 0x00; i < 0x06; i++){ gWIZNETINFO.mac[0x05 - i] = buffEeprom[ADDR_W5500_MAC + i];} // MAC адрес
  settings.dhcp = buffEeprom[ADDR_W5500_ETHERNET];
  sntp.update = ((buffEeprom[ADDR_UPDATE_SNTP] + 0x01) * 0x0E10); //
  sntp.timer = sntp.update; 
  CopyBeffer(gWIZNETINFO.ip, buffEeprom, ADDR_W5500_IP, 0x04); // IP адрес
  CopyBeffer(gWIZNETINFO.sn, buffEeprom, ADDR_W5500_SN, 0x04); // SN адрес
  CopyBeffer(gWIZNETINFO.gw, buffEeprom, ADDR_W5500_GW, 0x04); // GW адрес
  CopyBeffer(settings.ipSend, buffEeprom, ADDR_W5500_SEND, 0x04); // SEND адрес
  CopyBeffer(settings.ipSntpP, buffEeprom, ADDR_W5500_NTP_P, 0x04); // SNTP адрес
  CopyBeffer(settings.ipSntpS, buffEeprom, ADDR_W5500_NTP_S, 0x04); // SNTP адрес
  if(0x10 & settings.dhcp){ gWIZNETINFO.dhcp = NETINFO_DHCP; }else{ gWIZNETINFO.dhcp = NETINFO_STATIC; }
  
  
/*----------------------------------------------------------------------------*/
}


//void EXTI9_5_IRQHandler(void){
//  if((EXTI->PR & EXTI_PR_PR5) == EXTI_PR_PR5){ // Прерывание от EXTI5 //PB5
////    Nrf24CheckRadio();
//    EXTI->PR |= EXTI_PR_PR5; // Сбросить флаг EXTI5
//  }
  
  
  
void TIM2_IRQHandler(void){
  TIM2->SR &= ~TIM_SR_UIF;
  for(uint8_t i = 0; i < W5500_MAX_HTTPSOCK; i++) httpServer_run(i);  // HTTP server handler
}

void TIM3_IRQHandler(void){
  TIM3->SR &= ~TIM_SR_UIF;
  EthernetDhcpRutine();
  if(0x00 == sntp.timer){
    sntp.timer = sntp.update;
    while(SNTP_run() != 1);
  }
  sntp.timer--;
  httpServer_time_handler();
}

void TIM4_IRQHandler(void){
  TIM4->SR &= ~TIM_SR_UIF;
  uint8_t x;
  ctlwizchip(CW_GET_PHYLINK, (void*)&x);
  if(x == PHY_LINK_OFF){
    #if defined DEBUG_ETHERNET
      printf("Unknown PHY link status.\r\n");
    #endif
  }else{
    EthernetInitIP();
  }
}
 
void TimerHTTP(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 0x9C3F; // 39999 80000000:40000=2000Hz
  TIM2->ARR = 0x27; // 50Hz
  TIM2->SR = 0x00;
  TIM2->DIER |= TIM_DIER_UIE;
  TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  #if defined DEBUG_SETTING
    printf("< OK >    Start Timer HTTP\r\n");
  #endif
  
  NVIC_SetPriority(TIM2_IRQn, PRIORITY_HTTP);
  NVIC_EnableIRQ(TIM2_IRQn);
}

void TimerDHCPSNTP(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->PSC = 0x9C3F; // 39999 80000000:40000=2000Hz
  TIM3->ARR = 0x7CF; // 1Hz
  TIM3->SR = 0x00;
  TIM3->DIER |= TIM_DIER_UIE;
  TIM3->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  #if defined DEBUG_SETTING
    printf("< OK >    Start Timer DHCP\r\n");
  #endif
  
  NVIC_SetPriority(TIM3_IRQn, PRIORITY_DHCP);
  NVIC_EnableIRQ(TIM3_IRQn);
}

void TimerEthernetPHYLINK(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  TIM4->PSC = 0x9C3F; // 39999 80000000:40000=2000Hz
  TIM4->ARR = 0xC7; // 10Hz
  TIM4->SR = 0x00;
  TIM4->DIER |= TIM_DIER_UIE;
  TIM4->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  #if defined DEBUG_SETTING
    printf("< OK >    Start Timer PHYLINK\r\n");
  #endif
  
  NVIC_SetPriority(TIM4_IRQn, PRIORITY_PHYLINK);
  NVIC_EnableIRQ(TIM4_IRQn);
}

void Setting(void){
  #if defined DEBUG_SETTING
    printf("\n\r\t\tStart setting\n\r\n");
  #endif
  SysTick_Config(SystemCoreClock / 1000);   //1ms
  
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
  
  
  Ee24cxxInit();
  ReadConfig();
  RtcInit();
  CanInit();
  Rs485Init();
  Nrf24Init();
  EthernetInit();
  EthernetSettings();
}
