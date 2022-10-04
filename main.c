#include <stdio.h>

#ifdef USE_SERVER
#include <sys/epoll.h>

#include "server/server.h"

#define ADDR_SERVER_BIND "0.0.0.0"

#define MAX_EVENT 2048
#else

#include "client/client.h"

#define ADDR_SERVER_CONNECT "127.0.0.1"
#endif

#define PORT_SERVER 1234

int main(int argc, char *argv[])
{
#ifdef USE_SERVER
    int nListenFd = init_server(ADDR_SERVER_BIND, PORT_SERVER);
    if (nListenFd < 0)
    {
        puts("Init server Error");

        return -1;
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
                    if (strncmp("quit", chArrRecvBuf,4) == 0)
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
#else
    int nConnectFd = connect_server(ADDR_SERVER_CONNECT, PORT_SERVER);
    if (nConnectFd < 0)
    {
        puts("Failed to connect the server");

        return -1;
    }

    char chArrSendBuf[BUFSIZ] = {0};
    char chArrRecvBuf[BUFSIZ] = {0};
    for (;;)
    {
        // Get input
        memset(chArrSendBuf, 0, BUFSIZ);
        fgets(chArrSendBuf, BUFSIZ, stdin);
        size_t unlLenSendBuf = strlen(chArrSendBuf);
        chArrSendBuf[unlLenSendBuf] = '\0';

        // If input is "quit", break out the loop
        if (strncmp("quit", chArrSendBuf, 4) == 0)
        {
            break;
        };

        // Send message
        send_message(nConnectFd, chArrSendBuf, unlLenSendBuf);

        // Recv the reply from server
        memset(chArrRecvBuf, 0, BUFSIZ);
        long lBytesRecv = recv(nConnectFd, chArrRecvBuf, BUFSIZ, 0);
        if (lBytesRecv < 0)
        {
            perror("Recv Error:");

            continue;
        }
        else if (lBytesRecv == 0)
        {
            puts("Server has closed...");

            return 0;
        }
        else
        {
            printf("Server reply: %s\n", chArrRecvBuf);
        }
    }

    close(nConnectFd);

    return 0;
#endif
}
