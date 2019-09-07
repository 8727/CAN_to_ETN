#ifndef SNTP_H_
#define SNTP_H_

#include <stdint.h>
#include <string.h>
#include "setting.h"
#include "socket.h"

#define MAX_SNTP_BUF_SIZE   sizeof(ntpformat) //< maximum size of DNS buffer. */
#define ntp_port            123               //ntp server port number

typedef struct _ntpformat{
  uint8_t  dstaddr[4];     /* destination (local) address */
  uint8_t  version;        /* version number */
  uint8_t  leap;           /* leap indicator */
  uint8_t  mode;           /* mode */
  uint8_t  stratum;        /* stratum */
  uint8_t  poll;           /* poll interval */
  uint8_t  precision;      /* precision */
  uint32_t rootdelay;      /* root delay */
  uint32_t rootdisp;       /* root dispersion */
  uint8_t  refid;          /* reference ID */
  uint64_t reftime;        /* reference time */
  uint64_t org;            /* origin timestamp */
  uint64_t rec;            /* receive timestamp */
  uint64_t xmt;            /* transmit timestamp */
}ntpformat;

void SNTP_init(uint8_t s, uint8_t *ntp_server, uint8_t *buf);
int8_t SNTP_run(void);

#endif /* SNTP_H_ */
