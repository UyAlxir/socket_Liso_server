#include "response.h"

int response(Request * request, int readret ,  char * buf){
    
    //bad request
    if(request==NULL){
        strcpy(buf,BAD_REQUEST);
        readret=strlen(BAD_REQUEST);
        return readret;
    }

    //response get
    if(strcmp(request->http_method,_GET)==0){
        //currently do nothing
        return readret;
    }

    //response head
    if(strcmp(request->http_method,_HEAD)==0){
        //currently do nothing
        return readret;
    }

    //response post
    if(strcmp(request->http_method,_POST)==0){
        //currently do nothing
        return readret;
    }

    //response for other methods
    strcpy(buf,NOT_IMPLEMENTED);
    readret=strlen(NOT_IMPLEMENTED);
    return readret;
}