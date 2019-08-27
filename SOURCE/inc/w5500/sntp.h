#ifndef _SNTP_H_
#define _SNTP_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdint.h>
#include <string.h>
#include "socket.h"
#include "setting.h"


#define MAX_SNTP_BUF_SIZE   sizeof(ntpformat)   // maximum size of DNS buffer. */
#define ntp_port            123                 // ntp server port number
//#define SECS_PERDAY         86400UL             // seconds in a day = 60*60*24
//#define UTC_ADJ_HRS         9                   // SEOUL : GMT+9
//#define EPOCH               1900                // NTP start year

typedef struct _ntpformat{
  uint8_t  dstaddr[0x04];  /* destination (local) address */
  uint8_t  version;        /* version number */
  uint8_t  leap;           /* leap indicator */
  uint8_t  mode;           /* mode */
  uint8_t  stratum;        /* stratum */
  uint8_t  poll;           /* poll interval */
  int8_t   precision;      /* precision */
  uint16_t rootdelay;      /* root delay */
  uint16_t rootdisp;       /* root dispersion */
  uint8_t  refid;          /* reference ID */
  uint64_t reftime;        /* reference time */
  uint64_t org;            /* origin timestamp */
  uint64_t rec;            /* receive timestamp */
  uint64_t xmt;            /* transmit timestamp */
} ntpformat;

void get_seconds_from_ntp_server(uint8_t *buf, uint16_t idx);
void SNTP_init(uint8_t s, uint8_t *ntp_server, uint8_t *buf);
int8_t SNTP_run(void);

#endif /* _SNTP_H_ */
