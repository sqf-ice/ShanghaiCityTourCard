//
// Created by wenha_000 on 2017-09-12.
//

#ifndef SHANGHAICITYTOURCARD_SHTCCLIENT_H
#define SHANGHAICITYTOURCARD_SHTCCLIENT_H

#ifdef __cplusplus
extern "C"{
#endif

int Connect(char* ip, unsigned short port);
int Send(void* data,int length);

#ifdef __cplusplus
}
#endif

#endif //SHANGHAICITYTOURCARD_SHTCCLIENT_H
