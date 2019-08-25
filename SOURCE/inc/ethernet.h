#ifndef _ETHERNET_H
#define _ETHERNET_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
//#include "wizchip_conf.h"



/* Define --------------------------------------------------------------------*/
#define W5500_CS_LOW                GPIOA->BSRR = GPIO_BSRR_BR4
#define W5500_CS_HIGHT              GPIOA->BSRR = GPIO_BSRR_BS4

#define W5500_RESET_LOW             GPIOB->BSRR = GPIO_BSRR_BR1
#define W5500_RESET_HIGHT           GPIOB->BSRR = GPIO_BSRR_BS1

/* Define --------------------------------------------------------------------*/
#define BSB_COMMON                  0x00

#define BSB_S0                      0x01
#define BSB_S0_TX                   0x02
#define BSB_S0_RX                   0x03

#define RWB_WRITE                   0x01

#define OM_FDM0                     0x00 //режим передачи данных переменной длины
#define OM_FDM1                     0x01 //режим передачи данных по одному байту
#define OM_FDM2                     0x02 //режим передачи данных по два байта
#define OM_FDM3                     0x03 //режим передачи данных по четыре байта

#define MR                          0x0000  //Mode Register

#define GWR0                        0x0001  //Gateway IP Address Register MSB
#define GWR1                        0x0002
#define GWR2                        0x0003
#define GWR3                        0x0004  // LSB

#define SUBR0                       0x0005  //Subnet Mask Register MSB
#define SUBR1                       0x0006
#define SUBR2                       0x0007
#define SUBR3                       0x0008  // LSB

#define SHAR0                       0x0009  //Source Hardware Address Register MSB
#define SHAR1                       0x000A
#define SHAR2                       0x000B
#define SHAR3                       0x000C
#define SHAR4                       0x000D
#define SHAR5                       0x000E  // LSB

#define SIPR0                       0x000F  //Source IP Address Register MSB
#define SIPR1                       0x0010
#define SIPR2                       0x0011
#define SIPR3                       0x0012  // LSB

/* Define --------------------------------------------------------------------*/
#define Sn_PORT0                    0x0004 // Socket 0 Source Port Register MSB
#define Sn_PORT1                    0x0005 // Socket 0 Source Port Register LSB

/* Define --------------------------------------------------------------------*/





//#define W5500_IO_BASE_              0x0000
//#define W5500_SPI_READ              (0x00 << 0x02) //< SPI interface Read operation in Control Phase
//#define W5500_SPI_WRITE             (0x01 << 0x02) //< SPI interface Write operation in Control Phase
//#define WIZCHIP_CREG_BLOCK          0x00  //< Common register block
//#define WIZCHIP_SREG_BLOCK(N)       (0x01 + 0x04 * N) //< Socket N register block
//#define WIZCHIP_TXBUF_BLOCK(N)      (0x02 + 0x04 * N) //< Socket N Tx buffer address block
//#define WIZCHIP_RXBUF_BLOCK(N)      (0x03 + 0x04 * N) //< Socket N Rx buffer address block
//#define WIZCHIP_OFFSET_INC(ADDR, N)    (ADDR + (N << 0x08)) //< Increase offset address

struct w5500InitTypeDef{
  uint8_t mac[0x06];   //< Source Mac Address
  uint8_t ip[0x04];    //< Source IP Address
  uint8_t sn[0x04];    //< Subnet Mask 
  uint8_t gw[0x04];    //< Gateway IP Address
  uint8_t dns[0x04];   //< DNS server IP Address
  uint8_t settings; //< 1 - Static, 2 - DHCP
};

extern struct w5500InitTypeDef w5500;

uint8_t W5500WriteRead(uint8_t byte);
void W5500Init(void);

#endif /* _ETHERNET_H */
