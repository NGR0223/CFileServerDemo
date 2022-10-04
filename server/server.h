#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

enum INIT_SERVER_RESULT
{
    INIT_SERVER_RESULT_OK = 0,
    INIT_SERVER_RESULT_SOCKET = -1,
    INIT_SERVER_RESULT_BIND = -2;
    INIT_SERVER_RESULT_
};
int InitServer(char *pAddrServer, uint16_t unServerPort);

#endif //DEMO_SERVER_H
