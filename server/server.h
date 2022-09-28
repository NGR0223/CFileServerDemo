#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


int InitServer(char *pServerAddr, u_int16_t server_port);

#endif //DEMO_SERVER_H
