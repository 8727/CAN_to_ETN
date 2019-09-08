#include "web.h"

uint8_t RX_BUF[DATA_BUF_SIZE];
uint8_t TX_BUF[DATA_BUF_SIZE];
uint8_t socknumlist[] = {W5500_SOCK_HTTP_0, W5500_SOCK_HTTP_1, W5500_SOCK_HTTP_2, W5500_SOCK_HTTP_3};


void WebInit(void){
  httpServer_init(TX_BUF, RX_BUF, W5500_MAX_HTTPSOCK, socknumlist);
  reg_httpServer_cbfunc(NULL, NULL);
  reg_httpServer_webContent((uint8_t *)"index.html", (uint8_t *)index_page);				// index.html 		: Main page example
  reg_httpServer_webContent((uint8_t *)"netinfo.html", (uint8_t *)netinfo_page);			// netinfo.html 	: Network information example page
  reg_httpServer_webContent((uint8_t *)"netinfo.js", (uint8_t *)wiz550web_netinfo_js);	// netinfo.js 		: JavaScript for Read Network configuration 	(+ ajax.js)
//  reg_httpServer_webContent((uint8_t *)"img.html", (uint8_t *)img_page);					// img.html 		: Base64 Image data example page

  // Example #1
  reg_httpServer_webContent((uint8_t *)"dio.html", (uint8_t *)dio_page);					// dio.html 		: Digital I/O control example page
  reg_httpServer_webContent((uint8_t *)"dio.js", (uint8_t *)wiz550web_dio_js);			// dio.js 			: JavaScript for digital I/O control 	(+ ajax.js)

  // Example #2
  reg_httpServer_webContent((uint8_t *)"ain.html", (uint8_t *)ain_page);					// ain.html 		: Analog input monitor example page
  reg_httpServer_webContent((uint8_t *)"ain.js", (uint8_t *)wiz550web_ain_js);			// ain.js 			: JavaScript for Analog input monitor	(+ ajax.js)

  // Example #3
//  reg_httpServer_webContent((uint8_t *)"ain_gauge.html", (uint8_t *)ain_gauge_page);		// ain_gauge.html 	: Analog input monitor example page; using Google Gauge chart
//  reg_httpServer_webContent((uint8_t *)"ain_gauge.js", (uint8_t *)ain_gauge_js);			// ain_gauge.js 	: JavaScript for Google Gauge chart		(+ ajax.js)

  // AJAX JavaScript functions
  reg_httpServer_webContent((uint8_t *)"ajax.js", (uint8_t *)wiz550web_ajax_js);			// ajax.js
  display_reg_webContent_list();
//  for(uint8_t i = 0; i < W5500_MAX_HTTPSOCK; i++) httpServer_run(i);  // HTTP server handler
}
