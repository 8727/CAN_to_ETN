#ifndef _WEB_H
#define _WEB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "httpServer.h"
#include "webpage.h"
/* Define --------------------------------------------------------------------*/




uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len);
uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len);



void WebInit(void);

#endif /* _WEB_H */
