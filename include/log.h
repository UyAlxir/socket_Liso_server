#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include "parse.h"
#include <time.h>

#define SIZE_OF_TIMEBUF 21

// the choise of log to termoinal 
#define Log_to_terminal

// the choise of log to file
#define Log_to_file

FILE * log;
char filename[32];
int log_init();
void log_close();

char * gettime();
char curtime[21];
char cli_ip[32];


// 方便调试 , to terminal 
#ifdef Log_to_terminal
#define Log(format, ...) \
	do { \
		fprintf(stdout, "\33[1;32m[%s,%d,%s] " format "\33[0m\n", \
				__FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    } while(0)

#define Log_er(format, ...) \
	do { \
		fprintf(stdout, "\33[1;31m[%s,%d,%s] " format "\33[0m\n", \
				__FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    } while(0)
#else
#define Log(format, ...)
#define Log_Error(format, ...)
#endif

// Access LOg and Error Log , to file
#ifdef Log_to_file
#define Acceess_Log(a,c,l)\
	do{\
		log=fopen(filename,"a+");\
		fprintf(log,"%s - [%s] \"%s %s %s\" %d %d\n",cli_ip,gettime(),a->http_method,a->http_uri,a->http_version,c,l);\
		fclose(log);\
	}while(0)
#define Acceess_Log_Null(c,l)\
	do{\
		log=fopen(filename,"a+");\
		fprintf(log,"%s - [%s] \"BAD REQUEST\" %d %d\n",cli_ip,gettime(),c,l);\
		fclose(log);\
	}while(0)
#define Error_Log(format,...)\
	do{\
		log=fopen(filename,"a+");\
		fprintf(log,"[%s] [error] [%s,%d,%s] " format "\n"\
				,cli_ip,gettime(),__FILE__,__LINE__,__func__,## __VA_ARGS__);\
		fclose(log);\	
	}while(0)
#else
#define Acceess_Log(format,...)
#define Acceess_Log_Null(format,...)
#define Error_Log(format,...)
#endif


#endif