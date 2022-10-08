#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>

#define MAX_EVENT 2048

enum INIT_SERVER_ERROR
{
    INIT_SERVER_ERROR_SOCKET = -1,
    INIT_SERVER_ERROR_BIND = -2,
    INIT_SERVER_ERROR_LISTEN = -3
};

/*!
 * A server
 * @param pAddrServer the address of server
 * @param unsPortServer the port of server
 */
void server(char *pAddrServer, uint16_t unsPortServer);

/*!
 * Init the server with address and port
 * @param pAddrServer the address of server
 * @param unsPortServer the port of server
 * @return if OK, return the @nListenFd
 *         else, return @INIT_SERVER_ERROR
 */
int init_server(char *pAddrServer, uint16_t unsPortServer);

/*!
 * echo the message received from client
 * @param nConnectFd the connect fd
 * @param pMsg the message
 * @param unLenMsg the length of message
 */
void echo_message(int nConnectFd, char *pMsg, uint32_t unLenMsg);

#endif //DEMO_SERVER_H
