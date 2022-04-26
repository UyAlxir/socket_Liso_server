#include "log.h"

// 输出日日志的文件，全局变量，在log.h 中
extern FILE *log;

// 初始化日志，所谓初始化，也就是创建/清空输出日志的文件
// 如果第一次打开文件未成功，就返回初始化失败
// 第一次能成功打开，后续用宏定义输出日志时也就不用担心打不开的问题了（可能不带太严谨）
int log_init(){
    strcpy(filename,"log.txt");
    log=fopen(filename,"w+");
    if(log==NULL){
        Log_er("falied to open log file");
        return EXIT_FAILURE;
    } 
    else{
        Log("log init success");
    }
    char *timebuf;
    timebuf=gettime();
    fprintf(log,"-- Liso Server -- log begin at[%s]\n",timebuf);
    fclose(log);
}


// 最后一次确认关闭打开的文件
void log_close(){
    fclose(log);
}


// 获取当前时间
// 并转化成YYYY-MM-DD HH:MM:SS 的形式
// 放到全局变量 certime 中（此变量专门用来存当前时刻，因为经常使用，所以在log.h 文件中定义成了全局变量）
char * gettime(){
    memset(curtime,0,sizeof(curtime));
    time_t ltime;
    struct tm *cur;
    time(&ltime);
    cur = localtime(&ltime);
    strftime(curtime,21,"%F %T",cur);
    return curtime;
}
