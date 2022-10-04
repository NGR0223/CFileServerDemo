#ifndef DEMO_CLIENT_H
#define DEMO_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

enum CONNECT_SERVER_ERROR
{
    CONNECT_SERVER_ERROR_SOCKET = -1,
    CONNECT_SERVER_ERROR_CONNECT = -2
};

/*!
 * Connect to the server with address and port
 * @param pAddrServer the address of server
 * @param unsPortServer the port of server
 * @return if OK, return @nConnectFd
 *         else, return @CONNECT_SERVER_ERROR
 */
int connect_server(char *pAddrServer, uint16_t unsPortServer);

void send_message(int nConnectFd,char *pSendBuf,size_t unlLenSendBuf);

#endif //DEMO_CLIENT_H
