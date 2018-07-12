#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include "wrap.h"
#include <ctype.h>
int main(int argc,char *argv[])
{
    int cfd=Socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un client;
    bzero(&client,sizeof(client));
    unlink("./sock_tcp_client");
    client.sun_family=AF_UNIX;
    strcpy(client.sun_path,"./sock_tcp_client");
    Bind(cfd,(struct sockaddr *)&client,sizeof(client));
    struct sockaddr_un server;
    server.sun_family=AF_UNIX;
    strcpy(server.sun_path,"./sock_tcp");
    connect(cfd,(struct sockaddr *)&server,sizeof(server));

    int n;
    char buf[1024];
    while(1)
    {
        memset(buf,0x00,sizeof(buf));
        n=Read(STDIN_FILENO,buf,sizeof(buf));
        Write(cfd,buf,n);
        memset(buf,0x00,sizeof(buf));
        n=Read(cfd,buf,sizeof(buf));
        printf("buf=[%s]\n",buf);
    }
    return 0;
}
