#include "httpUtil.h"

uint8_t http_get_cgi_handler(uint8_t * uri_name, uint8_t * buf, uint32_t * file_len){
  uint8_t ret = HTTP_OK;
  uint16_t len = 0;
//  if(predefined_get_cgi_processor(uri_name, buf, &len)){
//    
//    
//    
//    
//    
//    ;
//  }else
//    if(strcmp((const char *)uri_name, "example.cgi") == 0){
//    // To do
//    ;
//  }else{
//    // CGI file not found
//    ret = HTTP_FAILED;
//  }
  if(ret)	*file_len = len;
  return ret;
}

uint8_t http_post_cgi_handler(uint8_t * uri_name, st_http_request * p_http_request, uint8_t * buf, uint32_t * file_len){
  uint8_t ret = HTTP_OK;
  uint16_t len = 0;
  uint8_t val = 0;
//  if(predefined_set_cgi_processor(uri_name, p_http_request->URI, buf, &len)){ 
//    
//    
//    
//    
//    ;
//  }else
//    if(strcmp((const char *)uri_name, "example.cgi") == 0){
//      
//      
//      // To do
//      val = 1;
//      len = sprintf((char *)buf, "%d", val);
//    }else{
//      // CGI file not found
//      ret = HTTP_FAILED;
//    }
  if(ret) *file_len = len;
  return ret;
}
