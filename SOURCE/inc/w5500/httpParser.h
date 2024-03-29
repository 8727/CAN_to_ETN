#ifndef __HTTPPARSER_H__
#define __HTTPPARSER_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "socket.h"

#define _HTTPPARSER_DEBUG_

#define MAX_URI_SIZE        1461
//#define MAX_URI_SIZE        512

#define HTTP_SERVER_PORT    80 /**< HTTP server well-known port number */

/* HTTP Method */
#define METHOD_ERR    0  /**< Error Method. */
#define METHOD_GET    1  /**< GET Method.   */
#define METHOD_HEAD   2  /**< HEAD Method.  */
#define METHOD_POST   3  /**< POST Method.  */

/* HTTP GET Method */
#define PTYPE_ERR     0  /**< Error file. */
#define PTYPE_HTML    1  /**< HTML file.  */
#define PTYPE_GIF     2  /**< GIF file.   */
#define PTYPE_TEXT    3  /**< TEXT file.  */
#define PTYPE_JPEG    4  /**< JPEG file.  */
#define PTYPE_FLASH   5  /**< FLASH file. */
#define PTYPE_MPEG    6  /**< MPEG file.  */
#define PTYPE_PDF     7  /**< PDF file.   */
#define PTYPE_CGI     8  /**< CGI file.   */
#define PTYPE_XML     9  /**< XML file.   */ 
#define PTYPE_CSS     10 /**< CSS file.   */
#define PTYPE_JS      11 /**< JavaScript file.*/
#define PTYPE_JSON    12 /**< JSON (JavaScript Standard Object Notation) file.*/
#define PTYPE_PNG     13 /**< PNG file. */
#define PTYPE_ICO     14 /**< ICON file. */

#define PTYPE_TTF     20 /**< Font type: TTF file. */
#define PTYPE_OTF     21 /**< Font type: OTF file. */
#define PTYPE_WOFF    22 /**< Font type: WOFF file. */
#define PTYPE_EOT     23 /**< Font type: EOT file. */
#define PTYPE_SVG     24 /**< Font type: SVG file. */

/* HTTP response */
#define STATUS_OK           200
#define STATUS_CREATED      201
#define STATUS_ACCEPTED     202
#define STATUS_NO_CONTENT   204
#define STATUS_MV_PERM      301
#define STATUS_MV_TEMP      302
#define STATUS_NOT_MODIF    304
#define STATUS_BAD_REQ      400
#define STATUS_UNAUTH       401
#define STATUS_FORBIDDEN    403
#define STATUS_NOT_FOUND    404
#define STATUS_INT_SERR     500
#define STATUS_NOT_IMPL     501
#define STATUS_BAD_GATEWAY  502
#define STATUS_SERV_UNAVAIL 503

#define HTML_HEADER       "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
#define RES_HTMLHEAD_OK   "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: "
#define RES_TEXTHEAD_OK   "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
#define RES_GIFHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: image/gif\r\nContent-Length: "
#define RES_JPEGHEAD_OK   "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: "
#define RES_PNGHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: "
#define RES_FLASHHEAD_OK  "HTTP/1.1 200 OK\r\nContent-Type: application/x-shockwave-flash\r\nContent-Length: "
#define RES_XMLHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: text/xml\r\nConnection: keep-alive\r\nContent-Length: "
#define RES_CSSHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: "
#define RES_JSHEAD_OK     "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: "
#define RES_JSONHEAD_OK   "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: "
#define RES_ICOHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\nContent-Length: "
#define RES_CGIHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
#define RES_TTFHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: application/x-font-truetype\r\nContent-Length: "
#define RES_OTFHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: application/x-font-opentype\r\nContent-Length: "
#define RES_WOFFHEAD_OK   "HTTP/1.1 200 OK\r\nContent-Type: application/font-woff\r\nContent-Length: "
#define RES_EOTHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: application/vnd.ms-fontobject\r\nContent-Length: "
#define RES_SVGHEAD_OK    "HTTP/1.1 200 OK\r\nContent-Type: image/svg+xml\r\nContent-Length: "

typedef struct _st_http_request{
  uint8_t METHOD;                   /**< request method(METHOD_GET...). */
  uint8_t TYPE;                     /**< request type(PTYPE_HTML...).   */
  uint8_t URI[MAX_URI_SIZE];        /**< request file name.             */
}st_http_request;

// HTTP Parsing functions
void unescape_http_url(char * url);                           /* convert escape character to ascii */
void parse_http_request(st_http_request *, uint8_t *);        /* parse request from peer */
void find_http_uri_type(uint8_t *, uint8_t *);                /* find MIME type of a file */
void make_http_response_head(char *, char, uint32_t);         /* make response header */
uint8_t * get_http_param_value(char* uri, char* param_name);  /* get the user-specific parameter value */
uint8_t get_http_uri_name(uint8_t * uri, uint8_t * uri_buf);  /* get the requested URI name */
#ifdef _OLD_
  uint8_t * get_http_uri_name(uint8_t * uri);
#endif

// Utility functions
uint16_t ATOI(uint8_t * str, uint8_t base);
void mid(char* src, char* s1, char* s2, char* sub);
void inet_addr_(uint8_t * addr, uint8_t * ip);

#endif  /* end of __HTTPPARSER_H__ */
