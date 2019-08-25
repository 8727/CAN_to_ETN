#include "w5500.h"

struct w5500InitTypeDef w5500;

uint8_t W5500WriteRead(uint8_t byte){
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = byte;
  while(!(SPI1->SR & SPI_SR_RXNE));
  return SPI1->DR;
}


void W5500WriteReg(uint16_t addres, uint8_t op, uint8_t data)
{
  W5500_CS_LOW;
  W5500WriteRead(addres >> 0x08);
  W5500WriteRead(addres);
  W5500WriteRead(op|(RWB_WRITE << 0x02));
  W5500WriteRead(data);
  W5500_CS_HIGHT;
}
void W5500Init(void){
  GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
  GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;
  GPIOA->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7;   // GPIO SPI Ethernet
  
  GPIOB->CRL &= (GPIO_CRL_CNF1);
  GPIOB->CRL |= GPIO_CRL_MODE1; // reset
  
  W5500_RESET_HIGHT;
  
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  SPI1->CR1 &= ~SPI_CR1_BR;
  SPI1->CR1 &= ~SPI_CR1_CPOL;
  SPI1->CR1 &= ~SPI_CR1_CPHA;
  SPI1->CR1 &= ~SPI_CR1_DFF;
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI1->CR1 |= SPI_CR1_MSTR;
  SPI1->CR1 |= SPI_CR1_SPE;     // SPI Ethernet
  
  DelayMs(0x05);
  W5500_RESET_LOW;
  DelayMs(0x05);
  W5500_RESET_HIGHT;
  W5500WriteReg(MR, (BSB_COMMON << 0x03) | OM_FDM1, 0x80); //Soft Reset
  DelayMs(0x64);
  
  W5500WriteReg((BSB_S0 << 0x03) | OM_FDM1, Sn_PORT0, 80 >> 0x08); //Настраиваем сокет 0
  W5500WriteReg((BSB_S0 << 0x03) | OM_FDM1, Sn_PORT1, 80);
  
  
  
  
  
  
  
  
  #if defined DEBUG_W25QXX
    printf("< OK >    Initialization ethernet\r\n");
  #endif
}
