#ifndef SNTP_H_
#define SNTP_H_

#include "setting.h"
#include <stdint.h>
#include <string.h>
#include "socket.h"

#define MAX_SNTP_BUF_SIZE  sizeof(ntpformat)    //< maximum size of DNS buffer. */
#define ntp_port            123                 //ntp server port number
#define SECS_PERDAY         86400UL             // seconds in a day = 60*60*24
#define UTC_ADJ_HRS         9                   // SEOUL : GMT+9
#define EPOCH               1900                // NTP start year

/* for ntpclient */
typedef unsigned int tdist;

typedef struct _ntpformat{
  uint8_t  dstaddr[4];     /* destination (local) address */
  uint8_t  version;        /* version number */
  uint8_t  leap;           /* leap indicator */
  uint8_t  mode;           /* mode */
  uint8_t  stratum;        /* stratum */
  uint8_t  poll;           /* poll interval */
  uint8_t  precision;      /* precision */
  tdist    rootdelay;      /* root delay */
  tdist    rootdisp;       /* root dispersion */
  uint8_t  refid;          /* reference ID */
  uint32_t reftime;        /* reference time */
  uint32_t org;            /* origin timestamp */
  uint32_t rec;            /* receive timestamp */
  uint32_t xmt;            /* transmit timestamp */
}ntpformat;

//void get_seconds_from_ntp_server(uint8_t *buf, uint16_t idx);
void SNTP_init(uint8_t s, uint8_t *ntp_server, uint8_t tz, uint8_t *buf);
int8_t SNTP_run(uint32_t *time);
//tstamp changedatetime_to_seconds(void);
//void calcdatetime(tstamp seconds);


#endif /* SNTP_H_ */
