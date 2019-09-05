#include "sntp.h"

ntpformat NTPformat;
uint8_t ntpmessage[48];
uint8_t *data_buf;
uint8_t NTP_SOCKET;
uint16_t ntp_retry_cnt = 0x00; //counting the ntp retry number

uint32_t get_seconds_from_ntp_server(uint8_t *buf, uint16_t idx){
  	uint32_t seconds = 0;
	uint8_t i=0;
	for (i = 0; i < 4; i++)
	{
		seconds = (seconds << 8) | buf[idx + i];
	}
  printf("Date %d\r\n\r\n", seconds);
  return seconds;
}

void SNTP_init(uint8_t s, uint8_t *ntp_server, uint8_t tz, uint8_t *buf){
  NTP_SOCKET = s;
  data_buf = buf;

  NTPformat.dstaddr[0] = ntp_server[0];
  NTPformat.dstaddr[1] = ntp_server[1];
  NTPformat.dstaddr[2] = ntp_server[2];
  NTPformat.dstaddr[3] = ntp_server[3];
  
  uint8_t Flag;
  NTPformat.leap = 0;           /* leap indicator */
  NTPformat.version = 4;        /* version number */
  NTPformat.mode = 3;           /* mode */
  NTPformat.stratum = 0;        /* stratum */
  NTPformat.poll = 0;           /* poll interval */
  NTPformat.precision = 0;      /* precision */
  NTPformat.rootdelay = 0;      /* root delay */
  NTPformat.rootdisp = 0;       /* root dispersion */
  NTPformat.refid = 0;          /* reference ID */
  NTPformat.reftime = 0;        /* reference time */
  NTPformat.org = 0;            /* origin timestamp */
  NTPformat.rec = 0;            /* receive timestamp */
  NTPformat.xmt = 1;            /* transmit timestamp */

  Flag = (NTPformat.leap << 0x06) + (NTPformat.version << 0x03) + NTPformat.mode; //one byte Flag
  memcpy(ntpmessage,(void const*)(&Flag),1);
}

int8_t SNTP_run(uint32_t *time){
  uint16_t RSR_len;
  uint32_t destip = 0x00;
  uint16_t destport;
  uint16_t startindex = 40; //last 8-byte of data_buf[size is 48 byte] is xmt, so the startindex should be 40

  switch(getSn_SR(NTP_SOCKET)){
    case SOCK_UDP:
      if((RSR_len = getSn_RX_RSR(NTP_SOCKET)) > 0x00){
        if(RSR_len > MAX_SNTP_BUF_SIZE) RSR_len = MAX_SNTP_BUF_SIZE;  // if Rx data size is lager than TX_RX_MAX_BUF_SIZE
        recvfrom(NTP_SOCKET, data_buf, RSR_len, (uint8_t *)&destip, &destport);
         
        time = get_seconds_from_ntp_server(data_buf,startindex);
        
        ntp_retry_cnt = false;
        close(NTP_SOCKET);
        return true;
      }
      if(ntp_retry_cnt < 0xFFFF){
        if(ntp_retry_cnt == 0x00){ //first send request, no need to wait
          sendto(NTP_SOCKET, ntpmessage, sizeof(ntpmessage), NTPformat.dstaddr, ntp_port);
          ntp_retry_cnt++;
        }else{ // send request again? it should wait for a while
          if((ntp_retry_cnt % 0xFFF) == 0x00){ //wait time
            sendto(NTP_SOCKET, ntpmessage, sizeof(ntpmessage), NTPformat.dstaddr, ntp_port);
            #ifdef DEBUG_SNTP
              printf("ntp retry: %d\r\n", ntp_retry_cnt);
            #endif
            ntp_retry_cnt++;
          }
        }
      }else{ //ntp retry fail
        ntp_retry_cnt = false;
        #ifdef DEBUG_SNTP
          printf("ntp retry failed!\r\n");
        #endif
        close(NTP_SOCKET);
      }
    break;
    case SOCK_CLOSED:
      socket(NTP_SOCKET, Sn_MR_UDP, ntp_port, false);
    break;
  }
  return false;// 0 - failed / 1 - success
}
