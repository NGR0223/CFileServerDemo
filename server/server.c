#include "server.h"

/*!
 * A server
 * @param pAddrServer the address of server
 * @param unsPortServer the port of server
 */
void server(char *pAddrServer, uint16_t unsPortServer)
{
    int nListenFd = init_server(pAddrServer, unsPortServer);
    if (nListenFd < 0)
    {
        puts("Init server Error");

        return;
    }

    // Init epoll with listen fd
    int nEpFd = epoll_create(MAX_EVENT);
    struct epoll_event tAllEpEv[MAX_EVENT + 1], tListenEpEv;
    tListenEpEv.events = EPOLLIN;
    tListenEpEv.data.fd = nListenFd;
    epoll_ctl(nEpFd, EPOLL_CTL_ADD, nListenFd, &tListenEpEv);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;)
    {
        int nNewFds = epoll_wait(nEpFd, tAllEpEv, sizeof(tAllEpEv) / sizeof(tAllEpEv[0]), -1);
        for (int i = 0; i < nNewFds; ++i)
        {
            int nCurrentFd = tAllEpEv[i].data.fd;
            // New connection from client
            if (nCurrentFd == nListenFd)
            {
                struct sockaddr_in tAddrClient;
                socklen_t unLenAddrClient = sizeof(tAddrClient);
                int nConnectFd = accept(nListenFd, (struct sockaddr *) &tAddrClient, &unLenAddrClient);
                if (nConnectFd == -1)
                {
                    perror("Accept Error:");

                    continue;
                }
                // Print the address and port
                printf("Client has connected from %s:%d\n", inet_ntoa(tAddrClient.sin_addr),
                       htons(tAddrClient.sin_port));

                struct epoll_event tTmpEpEv;
                tTmpEpEv.events = EPOLLIN;
                tTmpEpEv.data.fd = nConnectFd;
                epoll_ctl(nEpFd, EPOLL_CTL_ADD, nConnectFd, &tTmpEpEv);
            }
            else
            {
                // Avoid non read operation
                if (!(tAllEpEv[i].events & EPOLLIN))
                {
                    continue;
                }

                char chArrRecvBuf[BUFSIZ];
                memset(chArrRecvBuf, 0, BUFSIZ);
                long lBytesRecv = recv(nCurrentFd, &chArrRecvBuf, BUFSIZ, 0);
                if (lBytesRecv < 0)
                {
                    perror("Recv Error:");

                    continue;
                }
                else if (lBytesRecv == 0)
                {
                    printf("Client(%d) has disconnected...\n", nCurrentFd);

                    epoll_ctl(nEpFd, EPOLL_CTL_DEL, nCurrentFd, NULL);
                    close(nCurrentFd);
                }
                else
                {
                    if (strncmp("quit", chArrRecvBuf, 4) == 0)
                    {
                        printf("Client(%d) has sent 'quit' and disconnected...\n", nCurrentFd);

                        epoll_ctl(nEpFd, EPOLL_CTL_DEL, nCurrentFd, NULL);
                        close(nCurrentFd);
                    }
                    else
                    {
                        echo_message(nCurrentFd, chArrRecvBuf, lBytesRecv);
                    }
                }
            }
        }
    }
#pragma clang diagnostic pop
}

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

/*!
 * echo the message received from client
 * @param nConnectFd the connect fd
 * @param pMsg the message
 * @param unLenMsg the length of message
 */
void echo_message(int nConnectFd, char *pMsg, uint32_t unLenMsg)
{
    printf("Client(%d): %s\n", nConnectFd, pMsg);
    send(nConnectFd, pMsg, unLenMsg, 0);
}

