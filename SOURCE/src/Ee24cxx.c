#include "Ee24cxx.h"

void WriteData32ToBuffer(uint8_t addresses, uint32_t data, uint8_t* buff){
  buff[addresses] = (data >> 0x18);
  buff[addresses + 0x01] = (data >> 0x10);
  buff[addresses + 0x02] = (data >> 0x08);
  buff[addresses + 0x03] = data; 
}

void WriteData16ToBuffer(uint8_t addresses, uint16_t data, uint8_t* buff){
  buff[addresses] = (data >> 0x08);
  buff[addresses + 0x01] = data;
}

uint32_t ReadData32Buffer(uint8_t addresses, uint8_t* buff){
  uint32_t data = buff[addresses] << 0x18;
  data |= buff[addresses + 0x01] << 0x10;
  data |= buff[addresses + 0x02] << 0x08;
  data |= buff[addresses + 0x03]; 
  return data;
}

uint16_t ReadData16Buffer(uint8_t addresses, uint8_t* buff){
  uint16_t data = buff[addresses] << 0x08;
  data |= buff[addresses + 0x01];
  return data;
}

void Ee24cxxReadConfig(void){
  uint8_t buffEeprom[EEPROM_BUFF];
  Ee24cxxRead(buffEeprom);
  if(0xFF != buffEeprom[ADDR_EEPROM_STATUS_EEPROM]){
    RtcTypeDef dateBuild;
    dateBuild.year  = BUILD_YEAR;
    dateBuild.month = BUILD_MONTH;
    dateBuild.day   = BUILD_DAY;
    dateBuild.hour  = BUILD_TIME_H;
    dateBuild.min   = BUILD_TIME_M;
    dateBuild.sec   = BUILD_TIME_S;
    WriteData32ToBuffer(ADDR_EEPROM_VERSION_BUILD, VERSION_BUILD, buffEeprom);
    WriteData32ToBuffer(ADDR_EEPROM_DATE_BUILD, RtcTimeToCounter(&dateBuild), buffEeprom);
    buffEeprom[ADDR_EEPROM_STATUS_EEPROM] = STATUS_EEPROM;
    buffEeprom[ADDR_EEPROM_DEVICE_NUMBER] = DEVICE_NUMBER;
    buffEeprom[ADDR_EEPROM_DEVICE_TYPE] = DEVICE_TYPE;
    buffEeprom[ADDR_EEPROM_RTC_CALIBRATION] = RTC_CALIBRATION;
    WriteData32ToBuffer(ADDR_EEPROM_CAN_SPEED, CAN_SPEED, buffEeprom);
    buffEeprom[ADDR_EEPROM_NAME] = 0x00; //////////////////////////////////////////
    WriteData16ToBuffer(ADDR_EEPROM_RS485_SPEED, RS485_SPEED, buffEeprom);
    buffEeprom[ADDR_EEPROM_LCD_ROTATION] = LCD_ROTATION;
    WriteData16ToBuffer(ADDR_EEPROM_TIMER_START_FRIDGE, TIMER_START_FRIDGE, buffEeprom);
    WriteData16ToBuffer(ADDR_EEPROM_TIMER_START_CONDITIONER, TIMER_START_CONDITIONER, buffEeprom);
    WriteData16ToBuffer(ADDR_EEPROM_TIMER_START_POWER, TIMER_START_POWER, buffEeprom);
    WriteData16ToBuffer(ADDR_EEPROM_TIMER_START_RELAY_4, TIMER_START_RELAY_4, buffEeprom);
    WriteData32ToBuffer(ADDR_EEPROM_CALIBRATION_ENERGY, CALIBRATION_ENERGY, buffEeprom);
    WriteData16ToBuffer(ADDR_EEPROM_ALARM_POWER, ALARM_POWER, buffEeprom);
    
    Ee24cxxWrite(buffEeprom);
  }
  settings.dateBuild = ReadData32Buffer(ADDR_EEPROM_DATE_BUILD, buffEeprom);
  settings.number = buffEeprom[ADDR_EEPROM_DEVICE_NUMBER];
  
  settings.type = buffEeprom[ADDR_EEPROM_DEVICE_TYPE];
  settings.canDevice = (settings.type << 0x08) | (settings.number << 0x04);
  settings.rtcCalibration = buffEeprom[ADDR_EEPROM_RTC_CALIBRATION];
  settings.canSpeed = ReadData32Buffer(ADDR_EEPROM_CAN_SPEED, buffEeprom);
  settings.rs485Speed = ReadData16Buffer(ADDR_EEPROM_RS485_SPEED, buffEeprom);
  settings.rotation = buffEeprom[ADDR_EEPROM_LCD_ROTATION];
  
}

void Ee24cxxRead(uint8_t* data){
  uint8_t i = 0x00;
  uint8_t len = EEPROM_BUFF;
  I2C1->CR1 |= I2C_CR1_ACK;
  I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB)){}
  (void) I2C1->SR1;
  I2C1->DR = 0xA0;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
  (void) I2C1->SR1;
  (void) I2C1->SR2;
  I2C1->DR = 0x00;
  while (!(I2C1->SR1 & I2C_SR1_TXE)){}
  I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB)){}
  (void) I2C1->SR1;
  I2C1->DR = 0xA1;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
  (void) I2C1->SR1;
  (void) I2C1->SR2;
  if (0x01 < len){
    len--;
    while(len--){
      while (!(I2C1->SR1 & I2C_SR1_RXNE)){}
      data[i++] = I2C1->DR;
    }
  }
  while (!(I2C1->SR1 & I2C_SR1_BTF)){}
  I2C1->CR1 &= ~I2C_CR1_ACK;
  data[i++] = I2C1->DR;
  I2C1->CR1 |= I2C_CR1_STOP;
}

void Ee24cxxWrite(uint8_t* data){
  uint8_t i = 0x00;
  for(uint8_t y = 0x00; y < (EEPROM_BUFF/ 0x08); y++){
    uint8_t len = 0x08;
    //I2C1->CR1 |= I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)){}
    (void) I2C1->SR1;
    I2C1->DR = 0xA0;
    while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
    (void) I2C1->SR1;
    (void) I2C1->SR2;
    I2C1->DR = i;
    while (len--){
      while (!(I2C1->SR1 & I2C_SR1_TXE)){}
      I2C1->DR = data[i++];
    }
    while (!(I2C1->SR1 & I2C_SR1_BTF)){}
    I2C1->CR1 |= I2C_CR1_STOP;
    DelayMs(0x05);
  }
}

void Ee24cxxInitt(void){
  GPIOB->CRL |= (GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
  GPIOB->CRL |= (GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
  
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
  I2C1->CR1 = 0x00;
  I2C1->CR2 = 0x00;
  I2C1->CR2 |= 0x28;
//  I2C1->CR2 |= I2C_CR2_ITEVTEN;
  I2C1->TRISE = 0x0D;
  I2C1->OAR1 |= I2C_OAR1_ADD4;
  I2C1->CCR |= I2C_CCR_FS;
  I2C1->CCR |= 0x0022;
  I2C1->CR1 |= I2C_CR1_PE;
}
