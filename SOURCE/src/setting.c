#include "setting.h"

extern wiz_NetInfo defNet;
struct settingsInitTypeDef settings;
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

void ReadConfig(void){
  #if defined DEBUG_SETTING
    printf("< OK >    Read configuration\r\n");
  #endif
  uint8_t buffEeprom[EEPROM_BUFF];
  Ee24cxxRead(buffEeprom);
  if(0xFF != buffEeprom[ADDR_STATUS]){
    RtcTypeDef dateBuild;
    dateBuild.year  = BUILD_YEAR;
    dateBuild.month = BUILD_MONTH;
    dateBuild.day   = BUILD_DAY;
    dateBuild.hour  = BUILD_TIME_H;
    dateBuild.min   = BUILD_TIME_M;
    dateBuild.sec   = BUILD_TIME_S;
    WriteData32ToBuffer(ADDR_DATE_BUILD, RtcTimeToSeconds(&dateBuild), buffEeprom);
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
    buffEeprom[ADDR_RF24_TYPE_ON] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ON] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ON] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ON] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ON] = 0x00;
    buffEeprom[ADDR_RF24_TYPE_ON] = 0x00;
    buffEeprom[ADDR_W5500_IP] = defNet.ip[0x00];
    buffEeprom[ADDR_W5500_IP] = defNet.ip[0x01];
    buffEeprom[ADDR_W5500_IP] = defNet.ip[0x02];
    buffEeprom[ADDR_W5500_IP] = defNet.ip[0x03];
    buffEeprom[ADDR_W5500_NS] = defNet.sn[0x00];
    buffEeprom[ADDR_W5500_NS] = defNet.sn[0x01];
    buffEeprom[ADDR_W5500_NS] = defNet.sn[0x02];
    buffEeprom[ADDR_W5500_NS] = defNet.sn[0x03];
    buffEeprom[ADDR_W5500_GW] = defNet.gw[0x00];
    buffEeprom[ADDR_W5500_GW] = defNet.gw[0x01];
    buffEeprom[ADDR_W5500_GW] = defNet.gw[0x02];
    buffEeprom[ADDR_W5500_GW] = defNet.gw[0x03];

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
  defNet.ip[0x00] = buffEeprom[ADDR_W5500_IP];
  defNet.ip[0x01] = buffEeprom[ADDR_W5500_IP];
  defNet.ip[0x02] = buffEeprom[ADDR_W5500_IP];
  defNet.ip[0x03] = buffEeprom[ADDR_W5500_IP];
  defNet.sn[0x00] = buffEeprom[ADDR_W5500_NS];
  defNet.sn[0x01] = buffEeprom[ADDR_W5500_NS];
  defNet.sn[0x02] = buffEeprom[ADDR_W5500_NS];
  defNet.sn[0x03] = buffEeprom[ADDR_W5500_NS];
  defNet.gw[0x00] = buffEeprom[ADDR_W5500_GW];
  defNet.gw[0x01] = buffEeprom[ADDR_W5500_GW];
  defNet.gw[0x02] = buffEeprom[ADDR_W5500_GW];
  defNet.gw[0x03] = buffEeprom[ADDR_W5500_GW];

  
  
  
  
/*----------------------------------------------------------------------------*/
  
}


void EXTI15_10_IRQHandler(void){
  if((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13){ // Прерывание от EXTI13 //PD13
    Nrf24CheckRadio();
    EXTI->PR |= EXTI_PR_PR13; // Сбросить флаг EXTI13
  }
  
  
  
}

void Setting(void){
  #if defined DEBUG_SETTING
    printf("\t\tStart setting\n\r\n");
  #endif
  SysTick_Config(SystemCoreClock / 1000);   //1ms
  
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  
//  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE + AFIO_MAPR_TIM2_REMAP_FULLREMAP + AFIO_MAPR_TIM4_REMAP + AFIO_MAPR_I2C1_REMAP;
  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
//  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
//  RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

  
  Ee24cxxInit();
  ReadConfig();
  RtcInit();
  CanInit();
  Rs485Init();
  Nrf24Init();
  EthernetInit();
  #if defined DEBUG_SETTING
    printf("\t\tStop setting\n\r\n");
  #endif
  
}
