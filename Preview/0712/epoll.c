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
int main(void)
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv;
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    serv.sin_port=htons(8888);
    serv.sin_family=AF_INET;
    Bind(lfd,(struct sockaddr*)&serv,sizeof(serv));
    Listen(lfd,128);
    int epfd=epoll_create(1024);
    struct epoll_event event[1024];
    struct epoll_event ev;
    ev.data.fd=lfd;
    ev.events=EPOLLIN;
    char sIP[16];
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
    while(1)
    {
        int nready=epoll_wait(epfd,event,1024,-1);
        if(nready>0)
        {
            for(int i=0;i<nready;i++)
            {
                int sockfd=event[i].data.fd;
                if(lfd==sockfd)
                {
                    memset(sIP,0x00,sizeof(sIP));
                    struct sockaddr_in client;
                    socklen_t len=sizeof(client);
                    int cfd=Accept(sockfd,(struct sockaddr*)&client,&len);
                    ev.data.fd=cfd;
                    ev.events=EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
                    printf("client IP=[%s]---port=[%d]\n",
                           inet_ntop(AF_INET,&client.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(client.sin_port));
                    continue;
                }
                int n;
                char buf[1024];
                memset(buf,0x00,sizeof(buf));
                n=Read(sockfd,buf,sizeof(buf));
                if(n<=0)
                {
                    printf("read over or client close,n=[%d]\n",n);
                    Close(sockfd);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,&ev);
                }
                else
                {
                    for(int i=0;i<n;i++)
                    {
                        buf[i]=toupper(buf[i]);
                    }
                    Write(sockfd,buf,n);
                }
            }
        }
    }
    Close(lfd);
    return 0;
}
