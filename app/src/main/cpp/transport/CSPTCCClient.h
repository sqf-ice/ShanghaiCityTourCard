//
// Created by wenha_000 on 2017-09-30.
//

#ifndef SHANGHAICITYTOURCARD_CSPTCCCLIENT_H
#define SHANGHAICITYTOURCARD_CSPTCCCLIENT_H

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

class CSPTCCClient {
private:
    char m_ServerIP[64];
    unsigned int m_Port;
    int m_SocketID;
    int m_SndTimeout;
    int m_RcvTimeOut;
public:
    CSPTCCClient();
};


#endif //SHANGHAICITYTOURCARD_CSPTCCCLIENT_H
