#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include "iec104_parser.h"

class TCPServer
{
public:
    // 回调函数指针
    typedef void(*ConnectionCallback)(int fd, sockaddr_in addr);

    TCPServer(ConnectionCallback cb);

    void run();

private:
    int sockfd_;
    sockaddr_in servaddr_;
    ConnectionCallback cb_;
};

#endif // TCP_SERVER_H
