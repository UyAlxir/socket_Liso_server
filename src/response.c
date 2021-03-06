#include "response.h"



int response(Request * request, int readret ,  char * buf){

    char _file_cont[BUF_SIZE];
    memset(_file_cont,0,BUF_SIZE);
    
    // 400 bad request
    if(request==NULL){
        // printf("## Received a bad request");
        memset(buf,0,BUF_SIZE);
        strcpy(buf,BAD_REQUEST);
        readret=strlen(BAD_REQUEST);
        Acceess_Log_Null(BAD_REQUEST_CODE,readret);
        return readret;
    }

    // 505 http version not supported
    if(strcmp(request->http_version,_HTTP1_1)!=0){
        memset(buf,0,BUF_SIZE);
        strcpy(buf,NOT_SUPPORTED);
        readret=strlen(NOT_SUPPORTED);
        Acceess_Log(request,NOT_SUPPORTED_CODE,readret);
        return readret;
    }

    //response get
    if(strcmp(request->http_method,_GET)==0){
        memset(buf,0,BUF_SIZE);
        char _path[4096];
        if(strcmp(request->http_uri,"/")==0){
            strcpy(_path,_LOCAL_URI);
        } else {
            strcpy(_path,request->http_uri);
        }
        if(strcmp(_path,_GOOD_URI)!=0){
        }
        int fd_in=open(_path,O_RDONLY);
        if(fd_in<0){
            strcpy(buf,NOT_FOUND);
            readret=strlen(NOT_FOUND);
            Acceess_Log(request,NOT_FOUND_CODE,readret);
            return readret;
        }
        strcpy(buf,_200_OK);
        readret=strlen(_200_OK);
        int file_len = read(fd_in,_file_cont,BUF_SIZE);
        readret+=file_len;
        strcat(buf,_file_cont);
        Acceess_Log(request,OK_CODE,readret);
        return readret;
    }

    //response head
    if(strcmp(request->http_method,_HEAD)==0){
        memset(buf,0,BUF_SIZE);
        strcpy(buf,_200_OK);
        readret=strlen(_200_OK);
        Acceess_Log(request,OK_CODE,readret);
        return readret;
    }

    //response post
    if(strcmp(request->http_method,_POST)==0){
        //currently do nothing
        Acceess_Log(request,OK_CODE,readret);
        return readret;
    }
    // 501 not implemented
    memset(buf,0,BUF_SIZE);
    strcpy(buf,NOT_IMPLEMENTED);
    readret=strlen(NOT_IMPLEMENTED);
    Acceess_Log(request,NOT_SUPPORTED_CODE,readret);
    return readret;
}