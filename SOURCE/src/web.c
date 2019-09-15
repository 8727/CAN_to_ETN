#include "web.h"

uint8_t RX_BUF[DATA_BUF_SIZE];
uint8_t TX_BUF[DATA_BUF_SIZE];
uint8_t socknumlist[] = {W5500_SOCK_HTTP_0, W5500_SOCK_HTTP_1, W5500_SOCK_HTTP_2, W5500_SOCK_HTTP_3};


// Pre-defined Get CGI functions
void make_json_netinfo(uint8_t * buf, uint16_t * len);

uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len){

  return 0;
}

uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len){

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Get CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void make_json_netinfo(uint8_t * buf, uint16_t * len){
  wiz_NetInfo netinfo;
  ctlnetwork(CN_GET_NETINFO, (void*) &netinfo);
  // DHCP: 1 - Static, 2 - DHCP
  *len = sprintf((char *)buf, "NetinfoCallback({\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\
                      \"ip\":\"%d.%d.%d.%d\",\
                      \"gw\":\"%d.%d.%d.%d\",\
                      \"sn\":\"%d.%d.%d.%d\",\
                      \"dns\":\"%d.%d.%d.%d\",\
                      \"dhcp\":\"%d\"\
                      });",
                      netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
                      netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
                      netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
                      netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
                      netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
                      netinfo.dhcp
                      );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Set CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebInit(void){
  httpServer_init(TX_BUF, RX_BUF, W5500_MAX_HTTPSOCK, socknumlist);
  reg_httpServer_cbfunc(NULL, NULL);
  
  reg_httpServer_webContent((uint8_t *)"index.html", (uint8_t *)INDEX_PAGE);
  

  
  display_reg_webContent_list();
}
