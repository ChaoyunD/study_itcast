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
#include "wrap.h"
int main(int argc,char *argv[])
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);

    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    Listen(lfd,128);

    int epfd=epoll_create(1024);

    struct sockaddr_in clie;
    socklen_t len;
    char sIP[16];

    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=lfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&event);
    struct epoll_event evn[1024];
    while(1)
    {
        int nready=epoll_wait(epfd,evn,1024,-1);
        if(nready<0)
        {
            if(errno==EINTR)
            {
                continue;
            }
            exit(-1);
        }
        for(int i=0;i<nready;i++)
        {
            int sockfd=evn[i].data.fd;
            if(lfd==sockfd && (evn[i].events==EPOLLIN))
            {
                memset(&clie,0x00,sizeof(clie));
                len=sizeof(clie);
                int cfd=Accept(sockfd,(struct sockaddr *)&clie,&len);
                event.events=EPOLLIN|EPOLLET;//设置边缘触发ET
                event.data.fd=cfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&event);
                //将cfd设置非阻塞模式
                int flags=fcntl(cfd,F_GETFL,0);
                flags|=O_NONBLOCK;
                fcntl(cfd,F_SETFL,flags);
                memset(sIP,0x00,sizeof(sIP));
                printf("client ip:[%s]-->port:[%d]\n",inet_ntop(AF_INET,&clie.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(clie.sin_port));
                continue;
            }
            char buf[1024];
            memset(buf,0x00,sizeof(buf));
            int n=Read(sockfd,buf,3);
            if(n<=0)
            {
                printf("client closed or read error:[%d]\n",n);
                Close(sockfd);
                epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
            }
            else
            {
                printf("buf=[%s]\n",buf);
                for(int k=0;k<n;k++)
                    buf[k]=toupper(buf[k]);
                Write(sockfd,buf,n);
            }
        }
    }
    Close(lfd);
    Close(epfd);
    return 0;
}
