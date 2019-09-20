#ifndef _WEBPAGE_H_
#define _WEBPAGE_H_

static const char ERROR_HTML_PAGE[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\
\r\n\r\n<HTML>\r\n<BODY>\
\r\n<p style='text-align: center; font-size: 100px; margin: 0; padding: 0.5em 0;'>404</p>\
\r\n<p style='text-align: center; font-size: 40px; margin: 0;'>Sorry, the page you requested was not found.</p>\
\r\n</BODY>\r\n</HTML>\r\n\0";

static const char ERROR_UNAUTHORIZED_PAGE[] = "HTTP/1.1 401 Unauthorized\r\nContent-Type: text/html\
\r\nContent-Length: 50\r\n\r\nWWW-Authenticate: Basic realm=Data-Eth\r\n\0";
static const char ERROR_REQUEST_PAGE[] = "HTTP/1.1 400 OK\r\nContent-Type: text/html\r\nContent-Length: 50\r\n\r\n<HTML>\r\n<BODY>\r\nInvalid request.\r\n</BODY>\r\n</HTML>\r\n\0";

#define INDEX_PAGE "<!DOCTYPE html>"\
"<html lang='ru'>"\
"<head>"\
"<meta charset='UTF-8'>"\
"<meta name='viewport' content='width=device-width,minimum-scale=1,maximum-scale=10'>"\
"<title>DatNet</title>"\
"<style>"\
"</style>"\
"</head>"\
"<body>"\
"<div class='content-wrapper'>"\
"<header class='header'>"\
"<p style='font-size: 30px; margin: 0; padding: 10px 1em;'>DatNet</p>"\
"</header>"\
"<div class='container clearfix'>"\
"<main class='content'>"\
"<p style='text-align: center; font-size: 30px; margin: 0; padding: 1.5em 0;'>Hello, html!</p>"\
"</main>"\
"<aside class='sidebar sidebar1'>"\
"<p style='text-align: center;'>sidebar 1</p>"\
"</aside>"\
"<aside class='sidebar sidebar2'>"\
"<p style='text-align: center;'>sidebar 2</p>"\
"</aside>"\
"</div>"\
"<footer class='footer'>"\
"<p style='font-size: 30px; margin: 0; padding: 10px 1em;'>Footer</p>"\
"</footer>"\
"</div>"\
"</body>"\
"</html>"




#endif
