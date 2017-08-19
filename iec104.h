#ifndef IEC104_H
#define IEC104_H
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include<iostream>
#include<netinet/in.h>
#include "iec104_parser.h"

class IEC104: public IEC104Parser
{
public:
    IEC104();

    void connectTCP();

private:
    int sockfd;
    sockaddr_in servaddr;
    sockaddr_in clientaddr;
    socklen_t clientaddrlen;
    int clientfd;

    char buff[1024];

    void listen();

    void sendTCP(char *data, int sz);
    int  readTCP(char *data,int sz);

    void disconnectTCP();
};

#endif // IEC104_H
