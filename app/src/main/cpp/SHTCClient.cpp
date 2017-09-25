//
// Created by wenha_000 on 2017-09-12.
//

#include "SHTCClient.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLINE 1024

int sockfd;

int Connect(char* ip, unsigned short port)
{
    sockfd=socket(AF_INET,SOCK_STREAM,0);//IPPROTO_TCP);
    struct sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET,ip,&sockaddr.sin_addr);

    if((connect(sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )
    {
        printf("connect error %s errno: %d\n",strerror(errno),errno);
        return 1;
    }
    return 0;
}

int Send(void* data,int length)
{
    return 0;
}
