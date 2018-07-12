#include <stdio.h>
#include <ctype.h>
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
int main(int argc,char *argv[])
{
    int lfd=socket(AF_UNIX,SOCK_STREAM,0);
    if(lfd<0)
    {
        perror("socket error");
        return -1;
    }
    struct sockaddr_un serv;
    bzero(&serv,sizeof(serv));
    serv.sun_family=AF_UNIX;
    strcpy(serv.sun_path,"server.sock");
    unlink("./server.sock");
    bind(lfd,(struct sockaddr *)&serv,sizeof(serv));

    listen(lfd,128);

    struct sockaddr_un client;
    socklen_t len=sizeof(client);
    int cfd=accept(lfd,(struct sockaddr *)&client,&len);
    if(cfd<0)
    {
        perror("accept error");
        return -1;
    }
    printf("accept ok,path=[%s]\n",client.sun_path);
    int n;
    char buf[1024];
    while(1)
    {
        memset(buf,0x00,sizeof(buf));
        n=recv(cfd,buf,sizeof(buf),0);
        if(n<0)
        {
            printf("recv error or client closed,n=[%d]\n",n);
            break;
        }
        for(int i=0;i<n;i++)
        {
            buf[i]=toupper(buf[i]);
        }
        send(cfd,buf,n,0);
    }
    close(cfd);
    close(lfd);
    return 0;
}
