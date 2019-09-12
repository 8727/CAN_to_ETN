#ifndef _ETHERNET_H
#define _ETHERNET_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "wizchip_conf.h"
#include "dhcp.h"
#include "sntp.h"

/* Define --------------------------------------------------------------------*/
#define ETHERNET_CS_LOW                GPIOA->BSRR = GPIO_BSRR_BR4
#define ETHERNET_CS_HIGHT              GPIOA->BSRR = GPIO_BSRR_BS4

#define ETHERNET_RESET_LOW             GPIOB->BSRR = GPIO_BSRR_BR1
#define ETHERNET_RESET_HIGHT           GPIOB->BSRR = GPIO_BSRR_BS1

/* Define --------------------------------------------------------------------*/

void EthernetDhcpRutine(void);
void EthernetInitIP(void);
void EthernetSettings(void);
void EthernetInit(void);

#endif /* _ETHERNET_H */
