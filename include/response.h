#ifndef __RESPONSE_H_
#define __RESPOMSE_H_

#include "parse.h"

#define BAD_REQUEST "HTTP/1.1 400 Bad request\r\n\r\n"
#define NOT_IMPLEMENTED "HTTP/1.1 501 Not Implemented\r\n\r\n"
#define _HEAD "HEAD"
#define _GET  "GET"
#define _POST "POST"

int response(Request * request,int readret, char * buf);


#endif 