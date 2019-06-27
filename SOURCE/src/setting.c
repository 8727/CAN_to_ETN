#include "setting.h"

static __IO uint32_t msTicks;
struct settingsInitTypeDef settings;
struct powerInitTypeDef power;

void SysTick_Handler(void){
  msTicks++;
}

uint32_t GetTick(void){
  return msTicks;
}

void DelayMs(uint32_t ms){
  uint32_t tickstart = GetTick();
  while((GetTick() - tickstart) < ms){}
}

void Setting(void){
  SysTick_Config(SystemCoreClock / 1000);   //1ms
  
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  
//  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE;  //NO DEBUG
  
  Ee24cxxInitt();
  Ee24cxxReadConfig();
  RtcInit();
  Rs485Init();
  CanInit();
}
