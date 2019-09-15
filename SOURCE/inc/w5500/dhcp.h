#ifndef _DHCP_H_
#define _DHCP_H_

//#define _DHCP_DEBUG_

/* Retry to processing DHCP */
#define MAX_DHCP_RETRY          2        ///< Maxium retry count
#define DHCP_WAIT_TIME          10       ///< Wait Time 10s

/* UDP port numbers for DHCP */
#define DHCP_SERVER_PORT        67       ///< DHCP server port number
#define DHCP_CLIENT_PORT        68       ///< DHCP client port number
#define MAGIC_COOKIE            0x63825363  ///< Any number. You can be modifyed it any number
#define DCHP_HOST_NAME          "Data-Etn\0"

enum{
  DHCP_FAILED = 0,  ///< Procssing Fail
  DHCP_RUNNING,     ///< Procssing DHCP proctocol
  DHCP_IP_ASSIGN,   ///< First Occupy IP from DHPC server      (if cbfunc == null, act as default default_ip_assign)
  DHCP_IP_CHANGED,  ///< Change IP address by new ip from DHCP (if cbfunc == null, act as default default_ip_update)
  DHCP_IP_LEASED,   ///< Stand by 
  DHCP_STOPPED      ///< Stop procssing DHCP protocol
};

void DHCP_init(uint8_t s, uint8_t * buf);
void DHCP_time_handler(void);
void reg_dhcp_cbfunc(void(*ip_assign)(void), void(*ip_update)(void), void(*ip_conflict)(void)); 
uint8_t DHCP_run(void);
void    DHCP_stop(void);
void getIPfromDHCP(uint8_t* ip);
void getGWfromDHCP(uint8_t* ip);
void getSNfromDHCP(uint8_t* ip);
void getDNSfromDHCP(uint8_t* ip);
uint32_t getDHCPLeasetime(void);

#endif	/* _DHCP_H_ */
