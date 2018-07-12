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
#include <ctype.h>
#include "wrap.h"
int main(int argc,char *argv[])
{
    int cfd=Socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_port=htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&client.sin_addr.s_addr);
    int n;
    char buf[1024];
    while(1)
    {
        socklen_t len=sizeof(client);
        memset(buf,0x00,sizeof(buf));
        n=Read(STDIN_FILENO,buf,sizeof(buf));
        if(n<=0)
        {
            printf("Read error:n=[%d]\n",n);
            break;
        }
        sendto(cfd,buf,n,0,(struct sockaddr *)&client,sizeof(client));
        memset(buf,0x00,sizeof(buf));
        n=recvfrom(cfd,buf,sizeof(buf),0,(struct sockaddr *)&client,&len);
        if(n<0)
        {
            printf("recvfrom error:n=[%d]\n",n);
            break;
        }
        else if(n==0)
        {
            printf("read over:n=[%d]\n",n);
            continue;
        }
        else
        {
            printf("buf=[%s]\n",buf);
        }
    }
    Close(cfd);
    return 0;
}
