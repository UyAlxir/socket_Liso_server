#include "log.h"

extern FILE *log;

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

void log_close(){
    fclose(log);
}

char * gettime(){
    memset(curtime,0,sizeof(curtime));
    time_t ltime;
    struct tm *cur;
    time(&ltime);
    cur = localtime(&ltime);
    strftime(curtime,21,"%F %T",cur);
    return curtime;
}
