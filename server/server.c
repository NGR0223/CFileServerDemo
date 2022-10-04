
#include "server.h"

int InitServer(char *pAddrServer, uint16_t unServerPort)
{
    int nListenFd;  // Listen fd
    struct sockaddr_in tAddrServer;    // Address of server
    socklen_t unLenAddrServer = sizeof(tAddrServer);   // Length of address of server

    // Create socket
    nListenFd = socket(AF_INET, SOCK_STREAM, 0);   // IPv4 and TCP
    if (nListenFd < 0)
    {
        perror("Socket Error");

        return -1;
    }

    // Init address of server
    memset(&tAddrServer, 0, unLenAddrServer);
    tAddrServer.sin_family = AF_INET;
    tAddrServer.sin_port = htons(unServerPort);
    tAddrServer.sin_addr.s_addr = inet_addr(pAddrServer);

    // Set address and port multiplexing
    int opt = 1;
    setsockopt(nListenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void *) &opt, sizeof(opt));

    // Bind listen fd and server address
    if(bind(nListenFd,(struct  sockaddr*)&tAddrServer,unLenAddrServer)<0)
    {
        perror("Bind Error");

        return -2;
    }

}