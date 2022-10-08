#include "client.h"

/*!
 * A client
 * @param pAddrServer the address of server
 * @param unsPortServer the port of server
 */
void client(char *pAddrServer, uint16_t unsPortServer)
{
    int nConnectFd = connect_server(pAddrServer, unsPortServer);
    if (nConnectFd < 0)
    {
        puts("Failed to connect the server");

        return;
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
        }

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

            return;
        }
        else
        {
            printf("Server reply: %s\n", chArrRecvBuf);
        }
    }

    close(nConnectFd);
}

/*!
 * Connect to the server with address and port
 * @param pAddrServer the address of server
 * @param unsPortServer the port of server
 * @return if OK, return @nConnectFd
 *         else, return @CONNECT_SERVER_ERROR
 */
int connect_server(char *pAddrServer, uint16_t unsPortServer)
{
    // Init address of server
    struct sockaddr_in tAddrServer;
    socklen_t unLenAddrServer = sizeof(tAddrServer);
    memset(&tAddrServer, 0, unLenAddrServer);
    tAddrServer.sin_family = AF_INET;
    tAddrServer.sin_port = htons(unsPortServer);
    tAddrServer.sin_addr.s_addr = inet_addr(pAddrServer);

    // Create socket
    int nConnectFd = socket(AF_INET, SOCK_STREAM, 0);
    if (nConnectFd < 0)
    {
        perror("Socket Error:");

        return CONNECT_SERVER_ERROR_SOCKET;
    }

    int ret = connect(nConnectFd, (struct sockaddr *) &tAddrServer, unLenAddrServer);
    if (ret < 0)
    {
        perror("Connect Error");

        return CONNECT_SERVER_ERROR_CONNECT;
    }

    return nConnectFd;
}

/*!
 * Send message to server
 * @param nConnectFd the connect fd
 * @param pSendBuf the send buffer
 * @param unlLenSendBuf the length of send buffer
 */
void send_message(int nConnectFd, char *pSendBuf, size_t unlLenSendBuf)
{
    send(nConnectFd, pSendBuf, unlLenSendBuf, 0);
}
