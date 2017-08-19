#include "tcpserver.h"
#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <netinet/in.h>
#include "logging.h"

using namespace std;

TCPServer::TCPServer(ConnectionCallback cb)
{
    cb_ = cb;
    sockfd_ = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd_ < 0)
    {
        perror("socket");
        abort();
    }
    int value = 1;
    setsockopt(sockfd_,SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));
    memset(&servaddr_, 0, sizeof(servaddr_));
    servaddr_.sin_family = AF_INET;
    servaddr_.sin_port = htons(2404);
    servaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    int r = bind(sockfd_,(struct sockaddr *)&servaddr_,sizeof(servaddr_));
    if(r < 0)
    {
        perror("bind");
    }
}


void TCPServer::run()
{
    int r = ::listen(sockfd_,10);
    if(r < 0)
    {
        perror("listen");
    }

    sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);
    while (1)
    {
        int clientfd = accept(sockfd_,(struct sockaddr *)&clientaddr,&clientaddrlen);
        if(clientfd > 0)
        {
            cb_(clientfd, clientaddr);
        }
        else
        {
            LOG(ERROR) << strerror(errno);
        }
    }
}
