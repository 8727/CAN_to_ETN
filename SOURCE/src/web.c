#include "web.h"

uint8_t RX_BUF[DATA_BUF_SIZE];
uint8_t TX_BUF[DATA_BUF_SIZE];
uint8_t socknumlist[] = {W5500_SOCK_HTTP_0, W5500_SOCK_HTTP_1, W5500_SOCK_HTTP_2, W5500_SOCK_HTTP_3};


void WebInit(void){
  httpServer_init(TX_BUF, RX_BUF, W5500_MAX_HTTPSOCK, socknumlist);
  reg_httpServer_cbfunc(NULL, NULL);
  for(uint8_t i = 0; i < W5500_MAX_HTTPSOCK; i++) httpServer_run(i);  // HTTP server handler
}
