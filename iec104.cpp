#include "iec104.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include<iostream>
#include<iomanip>
#include<netinet/in.h>

using namespace std;

IEC104::IEC104()
{

}

int IEC104::readTCP(char *data, int size)
{
    int nread = 0;
    while (nread < size) {
        ssize_t n = read(clientfd, data + nread, size - nread);
        if(n < 0)
        {
            perror("read");
            return 0;
        }
        if(n == 0)
        {
            cout << "connection closed by client fd=" << clientfd << endl;
            return 0;
        }

        nread += n;
    }
    return nread;
}

void IEC104::sendTCP(char *data, int sz)
{
    ssize_t nwrite = write(clientfd,data,sz);
    if(nwrite < 0)
    {
        perror("write");
    }
}

void IEC104::disconnectTCP()
{
    int r = close(clientfd);
    if(r != 0)
    {
        perror("close");
    }
}

void IEC104::connectTCP()
{
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0) {
        perror("socket");
    }
    int value = 1;
    setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(2404);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int r = bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(r < 0)
    {
        perror("bind");
    }

    r = ::listen(sockfd,10);
    if(r < 0)
    {
        perror("listen");
    }

    clientaddrlen = sizeof(clientaddr);
    while (1) {
        clientfd = accept(sockfd,(struct sockaddr *)&clientaddr,&clientaddrlen);
        if(clientfd > 0)
        {
            cout << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << " connected fd=" << clientfd  << endl;
            readFromMaster();
            close(clientfd);
        }
        else
        {
            perror("accept");
            break;
        }
    }
}
