#ifdef USE_SERVER

#include "server/server.h"

#define ADDR_SERVER_BIND "0.0.0.0"

#else

#include "client/client.h"

#define ADDR_SERVER_CONNECT "127.0.0.1"
#endif

#define PORT_SERVER 1234

int main()
{
#ifdef USE_SERVER
    server(ADDR_SERVER_BIND, PORT_SERVER);
#else
    client(ADDR_SERVER_CONNECT, PORT_SERVER);
#endif
    return 0;
}
