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
#include "wrap.h"
#define _BUF_LEN_ 1024
#define _EVENT_SIZE_ 1024
int gepfd=0;
typedef struct xx_event
{
    int fd;
    int events;
    void (*call_back)(int fd,int events,void *arg);
    char buf[1024];
    int buflen;
    int epfd;
}xevent;
xevent myevents[_EVENT_SIZE_+1];
void readData(int fd,int events,void *arg);
void eventadd(int fd,int events,void (*call_back)(int,int,void *),xevent *ev)
{
    ev->fd=fd;
    ev->call_back=call_back;
    ev->events=events;

    struct epoll_event epv;
    epv.data.ptr=ev;
    epv.events=events;
    epoll_ctl(gepfd,EPOLL_CTL_ADD,fd,&epv);
}
void eventset(int fd,int events,void (*call_back)(int,int,void *),xevent *ev)
{
    ev->fd=fd;
    ev->events=events;
    ev->call_back=call_back;

    struct epoll_event epv;
    epv.data.ptr=ev;
    epv.events=events;
    epoll_ctl(gepfd,EPOLL_CTL_MOD,fd,&epv);
}
void eventdel(int fd,int events,xevent *ev)
{
    printf("begin call:%s\n",__FUNCTION__);
    ev->fd=0;
    ev->events=0;
    ev->call_back=NULL;
    memset(ev->buf,0x00,sizeof(ev->buf));
    ev->buflen=0;

    struct epoll_event epv;
    epv.data.ptr=NULL;
    epv.events=0;
    epoll_ctl(gepfd,EPOLL_CTL_DEL,fd,&epv);
}
void senddata(int fd,int events,void *arg)
{
    printf("begin call:%s\n",__FUNCTION__);

    xevent *ev=arg;
    Write(fd,ev->buf,ev->buflen);
    eventset(fd,EPOLLIN,readData,ev);
}
void readData(int fd,int events,void *arg)
{
    printf("begin call:%s\n",__FUNCTION__);
    xevent *ev=arg;

    ev->buflen=Read(fd,ev->buf,sizeof(ev->buf));
    if(ev->buflen>0)
    {
        eventset(fd,EPOLLOUT,senddata,ev);
    }
    else if(ev->buflen==0)
    {
        Close(fd);
        eventdel(fd,EPOLLIN,ev);
    }
}
void initAccept(int fd,int events,void *arg)
{
    printf("begin call %s,gepfd=%d\n",__FUNCTION__,gepfd);
    int i;
    struct sockaddr_in client;
    socklen_t len=sizeof(client);
    int cfd=Accept(fd,(struct sockaddr *)&client,&len);

    for(i=0;i<_EVENT_SIZE_;i++)
    {
        if(myevents[i].fd==0)
            break;
    }
    eventadd(cfd,EPOLLIN,readData,&myevents[i]);
}
int main(int argc,char *argv[])
{
    int lfd = Socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    Listen(lfd,128);

    gepfd=epoll_create(1024);
    printf("gepfd=[%d]\n",gepfd);
    struct epoll_event events[1024];
    eventadd(lfd,EPOLLIN,initAccept,&myevents[_EVENT_SIZE_]);

    while(1)
    {
        int nready=epoll_wait(gepfd,events,1024,-1);
        if(nready<0)
        {
            perr_exit("epoll_wait error");
        }
        else if(nready>0)
        {
            int i=0;
            for(i=0;i<nready;i++)
            {
                xevent *xe=events[i].data.ptr;
                printf("fd=%d\n",xe->fd);
                if(xe->events & events[i].events)
                {
                    xe->call_back(xe->fd,xe->events,xe);
                }
            }
        }
    }
    Close(lfd);
    return 0;
}
