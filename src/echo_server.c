/******************************************************************************
* echo_server.c                                                               *
*                                                                             *
* Description: This file contains the C source code for an echo server.  The  *
*              server runs on a hard-coded port and simply write back anything*
*              sent to it by connected clients.  It does not support          *
*              concurrent clients.                                            *
*                                                                             *
* Authors: Athula Balachandran <abalacha@cs.cmu.edu>,                         *
*          Wolf Richter <wolf@cs.cmu.edu>                                     *
*                                                                             *
*******************************************************************************/

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "parse.h"
#include "response.h"
#include "log.h"

#define ECHO_PORT 9999

extern FILE *log;

int close_socket(int sock)
{
    if (close(sock))
    {
        Log("Failed closing socket.");
        Error_Log("Failed closing socket.");
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int sock, client_sock;
    ssize_t readret;
    socklen_t cli_size;
    struct sockaddr_in addr, cli_addr;
    char buf[BUF_SIZE];
    char ans[24][BUF_SIZE];
    int lens[24];
    char res[BUF_SIZE*24];
    int cur[1];

    Log("----- Liso Server -----");
    log_init();
    
    /* all networked programs must create a socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        Log_er("Failed creating socket.");
        Error_Log("Failed creating socket.");
        return EXIT_FAILURE;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(ECHO_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    /* servers bind sockets to ports---notify the OS they accept connections */
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)))
    {
        close_socket(sock);
        Log_er("Failed binding socket.");
        Error_Log("Failed binding socket.");
        return EXIT_FAILURE;
    }

    if (listen(sock, 5))
    {
        close_socket(sock);
        Log_er("Error listening on socket.");
        Error_Log("Error listening on socket.");
        return EXIT_FAILURE;
    }

    /* finally, loop waiting for input and then write it back */
    while (1)
    {
        cli_size = sizeof(cli_addr);
        if ((client_sock = accept(sock, (struct sockaddr *) &cli_addr,
                                    &cli_size)) == -1)
        {
            close(sock);
            Log_er("Error accepting connection.");
            Error_Log("Error accepting connection.");
            return EXIT_FAILURE;
        }
        // get current client ip
        memset(cli_ip,0,sizeof cli_ip);
        strcpy(cli_ip,inet_ntoa(cli_addr.sin_addr));

        readret = 0;
        int cnt=0;

        while((readret = recv(client_sock, buf, BUF_SIZE, 0)) >= 1)
        {           
            cur[0]=0;
            int cnt=0;
            while(cur[0]<readret){
                int cur_before=cur[0];
                Request *request = parse(buf+cur[0],readret-cur[0],sock,cur);
                if(cur[0]-cur_before==2&&request==NULL)continue;
                strncpy(ans[cnt],buf+cur_before,cur[0]-cur_before);
                lens[cnt]= response(request,cur[0]-cur_before,ans[cnt]);
                cnt++;
                if(cnt>=24)break;
            }
            memset(buf, 0, BUF_SIZE); 
            memset(res,0,sizeof(res));
            int reslen=0;
            for(int i=0;i<cnt;i++){
                strncat(res+reslen,ans[i],lens[i]);
                reslen+=lens[i];
                memset(ans[i],0,BUF_SIZE);
            }
            if (send(client_sock, res,reslen , 0) != reslen)
            {
                close_socket(client_sock);
                close_socket(sock);
                Log_er("Error sending to client.");
                Error_Log("Error sending to client.");
                return EXIT_FAILURE;
            }
            memset(res,0,sizeof(res));
        } 

        if (readret == -1)
        {
            close_socket(client_sock);
            close_socket(sock);
            Log_er("Error reading from client socket.");
            Error_Log("Error reading from client socket.");
            return EXIT_FAILURE;
        }

        if (close_socket(client_sock))
        {
            close_socket(sock);
            Log_er("Error closing client socket.");
            Error_Log("Error closing client socket.");
            return EXIT_FAILURE;
        }
    }

    close_socket(sock);
    log_close();  

    return EXIT_SUCCESS;
}
