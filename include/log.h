#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include "parse.h"
#include <time.h>

#define SIZE_OF_TIMEBUF 21
// 输出到终端的日志，可通过注释此宏定义来取消终端日志（终端消息）
// the choise of log to termoinal 
#define Log_to_terminal

// 输出到文件的日志，可通过注释此宏定义来取消文件日志
// the choise of log to file
#define Log_to_file

// 日志文件
FILE * log;
// 日志文件名称
char filename[32];

// 初始化日志和结束日志
int log_init();
void log_close();

// 获取当前系统时间，并以 YYYY-MM-DD HH:MM:SS 的形式放到curtime 串中，用于在日志中输出
char * gettime();
char curtime[21];

// 客户端在连接的用户的ip存于此变量中，获取ic的代码在server文件中
char cli_ip[32];


// 方便调试 , to terminal 
#ifdef Log_to_terminal
// 输出到终端的日志信息，所以是stdout，标准输出
// \33[1;32m ~~~ \33[0m  定义在终端中输出的颜色
// __FILE__ 当前文件 
// __LINE__ 当前代码所在行（原文件中）
// __func__ 当前代码所在函数
//  fromat,... 和   fromat ## __VA_ARGS__ 配套使用，会在format 位置填充 用户在使用该宏定义时的任意输出数据以及形式
#define Log(format, ...) \
	do { \
		fprintf(stdout, "\33[1;32m[%s,%d,%s] " format "\33[0m\n", \
				__FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    } while(0)

//  跟楼上的Log 唯一的在于颜色，error 日志输出为红色
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
// 该宏定义日志方法来自nemu 源码
// 先打开log文件，再把日志信息输出到文件中，再关闭文件，为一次日志的完整执行
// 输出形式按照apache 的日志给出 如下
// 127.0.0.1 - [2022-04-26 13:50:32] "BAD REQUEST" 400 28
// cli_ip 以在sercer 代码中获取并放到 cli_ip 串中
// gettime() 获取时间在log.c 文件中包装好了
// 余下三个信息为当前request 的mothod , uri 以及 http_version
// 最后两个威为 当前respond 的代码 和 返回消息的长度
#define Acceess_Log(a,c,l)\
	do{\
		log=fopen(filename,"a+");\
		fprintf(log,"%s - [%s] \"%s %s %s\" %d %d\n",cli_ip,gettime(),a->http_method,a->http_uri,a->http_version,c,l);\
		fclose(log);\
	}while(0)
// 当bad request 时，因为ruquest 结构体为空，故不能通过传进来request 来输出信息，续作特殊处理
#define Acceess_Log_Null(c,l)\
	do{\
		log=fopen(filename,"a+");\
		fprintf(log,"%s - [%s] \"BAD REQUEST\" %d %d\n",cli_ip,gettime(),c,l);\
		fclose(log);\
	}while(0)
#define Error_Log(format,...)\
	do{\
		log=fopen(filename,"a+");\
		fprintf(log,"[%s] [%s] [error] [%s,%d,%s] " format "\n" \
				,cli_ip,gettime(),__FILE__,__LINE__,__func__,## __VA_ARGS__);\
		fclose(log);\	
	}while(0)
#else
#define Acceess_Log(format,...)
#define Acceess_Log_Null(format,...)
#define Error_Log(format,...)
#endif


#endif