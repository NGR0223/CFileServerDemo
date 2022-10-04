#include "server.h"

/*!
 * Init the server with address and port
 * @param pAddrServer the address of server
 * @param unsPortServer the port of server
 * @return if OK, return the @nListenFd
 *         else, return @INIT_SERVER_ERROR
 */
int init_server(char *pAddrServer, uint16_t unsPortServer)
{
    // Create socket
    int nListenFd = socket(AF_INET, SOCK_STREAM, 0);   // IPv4 and TCP
    if (nListenFd < 0)
    {
        perror("Socket Error");

        return INIT_SERVER_ERROR_SOCKET;
    }

    // Init address of server
    struct sockaddr_in tAddrServer;
    socklen_t unLenAddrServer = sizeof(tAddrServer);
    memset(&tAddrServer, 0, unLenAddrServer);
    tAddrServer.sin_family = AF_INET;
    tAddrServer.sin_port = htons(unsPortServer);
    tAddrServer.sin_addr.s_addr = inet_addr(pAddrServer);

    // Set address and port multiplexing
    int opt = 1;
    setsockopt(nListenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void *) &opt, sizeof(opt));

    // Bind listen fd and server address
    if (bind(nListenFd, (struct sockaddr *) &tAddrServer, unLenAddrServer) < 0)
    {
        perror("Bind Error");

        return INIT_SERVER_ERROR_BIND;
    }

    // Listen
    if (listen(nListenFd, SOMAXCONN) < 0)
    {
        perror("Listen Error:");

        return INIT_SERVER_ERROR_LISTEN;
    }
    puts("Waiting for connection...");

    return nListenFd;
}

void echo_message(int unCFd, char *pMsg, uint32_t unLenMsg)
{
    printf("Client(%d): %s\n", unCFd, pMsg);
    send(unCFd, pMsg, unLenMsg, 0);
}

