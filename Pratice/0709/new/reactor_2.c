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
#define BUF_SIZ 1024
#define EVE_SIZ 1024
typedef struct xx_event
{
    int fd;
    int events;
    void (*call_back)(int fd,int events,void *arg);
    char buf[1024];
    int buflen;
}xevent;
xevent myevents[EVE_SIZ+1];
int epfd;
void readdata(int fd,int events,void *arg);
void eventadd(int fd,int events,void(*call_back)(int ,int,void *),xevent *ev)
{
    printf("begin call:[%s]\n",__FUNCTION__);
    ev->fd=fd;
    ev->events=events;
    ev->call_back=call_back;

    struct epoll_event epv;
    epv.events=events;
    epv.data.ptr=ev;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&epv);
}
void eventset(int fd,int events,void(*call_back)(int ,int,void *),xevent *ev)
{
    printf("begin call:[%s]\n",__FUNCTION__);
    ev->fd=fd;
    ev->events=events;
    ev->call_back=call_back;

    struct epoll_event epv;
    epv.events=events;
    epv.data.ptr=ev;
    epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&epv);
}
void eventdel(int fd,int events,xevent *ev)
{
    printf("begin call:[%s]\n",__FUNCTION__);
    ev->call_back=NULL;
    ev->events=0;
    ev->fd=0;

    struct epoll_event epv;
    epv.data.ptr=NULL;
    epv.events=events;
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&epv);

}
void senddata(int fd,int events,void *arg)
{
    printf("begin call:[%s]\n",__FUNCTION__);
    xevent *ev=(xevent *)arg;
    Write(fd,ev->buf,ev->buflen);
    eventset(fd,EPOLLIN,readdata,ev);
}
void readdata(int fd,int events,void *arg)
{
    printf("begin call:[%s]\n",__FUNCTION__);
    xevent *ev=(xevent *)arg;

    memset(ev->buf,0x00,sizeof(ev->buf));
    ev->buflen=Read(fd,ev->buf,sizeof(ev->buf));
    if(ev->buflen>0)
    {
        eventset(fd,EPOLLOUT,senddata,ev);
    }
    else
    {
        Close(fd);
        eventdel(fd,EPOLLIN,ev);
    }
}
void initAccept(int fd,int events,void *arg)
{
    printf("begin call:[%s]\n",__FUNCTION__);
    struct sockaddr_in client;
    socklen_t len=sizeof(client);
    int cfd=Accept(fd,(struct sockaddr*)&client,&len);

    int i;
    for(i=0;i<EVE_SIZ;i++)
    {
        if(myevents[i].fd==0)
            break;
    }
    eventadd(cfd,EPOLLIN,readdata,&myevents[i]);
}
int main(void)
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv;
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) ;
    Bind(lfd,(struct sockaddr*)&serv,sizeof(serv));
    Listen(lfd,128);
    epfd=epoll_create(1024);
    struct epoll_event events[EVE_SIZ];
    eventadd(lfd,EPOLLIN,initAccept,&myevents[EVE_SIZ]);
    while(1)
    {
        int nready=epoll_wait(epfd,events,1024,-1);
        if(nready>0)
        {
            xevent *ev;
            for(int i=0;i<nready;i++)
            {
                ev=events[i].data.ptr;
                if(ev->events & events[i].events)
                    ev->call_back(ev->fd,ev->events,ev);
            }
        }
    }
    Close(lfd);
    return 0;
}
