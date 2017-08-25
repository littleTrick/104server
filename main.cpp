#include <iostream>
#include <tcpserver.h>
#include "logging.h"

using namespace std;

struct Args {
    int fd;
};

void* thread_main(void *arg)
{
    Args *args = reinterpret_cast<Args*>(arg);
    IEC104Parser *parser = new IEC104Parser(args->fd);
    parser->start();

    delete parser;
    delete args;

    return 0;
}

void on_new_connection(int clientfd, sockaddr_in clientaddr)
{
    LOG(INFO) << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << " connected";
    Args *arg = new Args;
    arg->fd = clientfd;

    // 创建线程
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread_main, arg))
    {
        perror("pthread_create");
    }
    else
    {
        LOG(INFO) << "fd=" << clientfd << "thread id=" << (unsigned int)tid;
    }
}

int main()
{    
    TCPServer server(on_new_connection);
    server.run();
    return 0;
}
