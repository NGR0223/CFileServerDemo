#include <stdio.h>
#include <pthread.h>
#include "client/client.h"

void *testClient(void *arg)
{
    int nConnectFd = connect_server("127.0.0.1", 1234);
    if (nConnectFd < 0)
    {
        puts("Failed to connect the server");

        return NULL;
    }
    //sleep(3);

    char chArrSendBuf[20] = {0};
    sprintf(chArrSendBuf, "Hello %d", *(int *) arg);
    chArrSendBuf[strlen(chArrSendBuf)] = '\0';
    send(nConnectFd, chArrSendBuf, 20, 0);
    puts(chArrSendBuf);

    sleep(3);
    char chArrRecvBuf[20] = {0};
    long lBytesRecv = recv(nConnectFd, chArrRecvBuf, 20, 0);
    if (lBytesRecv < 0)
    {
        perror("Recv Error:");
    }
    else if (lBytesRecv == 0)
    {
        puts("Server has closed...");

        return NULL;
    }
    else
    {
        printf("Server reply: %s\n", chArrRecvBuf);
    }

    close(nConnectFd);
    return (void *) 1;
}

int main(int argc, char *argv[])
{
    long lNumThread = strtol(argv[1], NULL, 10);
    int count = 0;
    for (int i = 0; i < lNumThread; ++i)
    {
        pthread_t tid;
        printf("%d-%d\n", i, count);
        pthread_create(&tid, NULL, (void *) testClient, &count);
        count++;

         pthread_detach(tid);
        // pthread_join(tid, NULL);
    }

    return 0;
}
