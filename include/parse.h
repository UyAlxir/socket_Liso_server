#ifndef __PARSE_H_
#define __PARSE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define SUCCESS 0
#define BUF_SIZE 8192*2

//Header field
typedef struct
{
	char header_name[4096];
	char header_value[4096];
} Request_header;

//HTTP Request Header
typedef struct
{
	char http_version[50];
	char http_method[50];
	char http_uri[4096];
	Request_header *headers;
	int header_count;
} Request;

Request* parse(char *buffer, int size,int socketFd,int *cur);

// functions decalred in parser.y
int yyparse();
void set_parsing_options(char *buf, size_t i, Request *request);

#endif
