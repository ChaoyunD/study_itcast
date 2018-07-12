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
    int lfd=Socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un serv;
    bzero(&serv,sizeof(serv));
    serv.sun_family=AF_UNIX;
    unlink("sock_tcp");
    strcpy(serv.sun_path,"sock_tcp");
    Bind(lfd,(struct sockaddr*)&serv,sizeof(serv));
    Listen(lfd,128);
    struct sockaddr_un client;
    socklen_t len=sizeof(client);
    int cfd=Accept(lfd,(struct sockaddr*)&client,&len);
    if(cfd<0)
    {
        perror("accept error");
        return -1;
    }
    printf("accept ok:client path=[%s]\n",client.sun_path);
    int n;
    char buf[1024];
    while(1)
    {
        memset(buf,0x00,sizeof(buf));
        n=Read(cfd,buf,sizeof(buf));
        if(n<0)
        {
            printf("read error:n=[%d]\n",n);
            close(cfd);
            continue;
        }
        else if(n==0)
        {
            printf("client close:n=[%d]\n",n);
            break;
        }
        else
        {
            for(int i=0;i<n;i++)
                buf[i]=toupper(buf[i]);
            Write(cfd,buf,n);
        }
    }
    return 0;
}
