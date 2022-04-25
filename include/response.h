#ifndef __RESPONSE_H_
#define __RESPOMSE_H_

#include "parse.h"
#include "log.h"

#define BAD_REQUEST "HTTP/1.1 400 Bad request\r\n\r\n"
#define NOT_IMPLEMENTED "HTTP/1.1 501 Not Implemented\r\n\r\n"
#define NOT_FOUND "HTTP/1.1 404 Not Found\r\n\r\n"
#define NOT_SUPPORTED "HTTP/1.1 505 HTTP Version not supported\r\n\r\n"
#define _200_OK "HTTP/1.1 200 OK\r\n\r\n"
#define _HEAD "HEAD"
#define _GET  "GET"
#define _POST "POST"
#define _HTTP1_1  "HTTP/1.1"
#define _GOOD_URI "/webServerStartCodes-new/static_site/index.html"
#define _LOCAL_URI "static_site/index.html"
#define _CRLF "\r\n"

int response(Request * request,int readret, char * buf);

enum codes{
    OK_CODE=200,
    BAD_REQUEST_CODE=400,
    NOT_FOUND_CODE=404,
    NOT_IMPLEMENTED_CODE=501,
    NOT_SUPPORTED_CODE=505
};

#endif 