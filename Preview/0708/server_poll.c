#include <stdio.h>
#include <poll.h>
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
#include <ctype.h>
#include "wrap.h"
int main(void)
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

    struct sockaddr_in clie;
    socklen_t len;
    char sIP[16];

    int maxfd=0;
    int nready=0;
    struct pollfd client[FD_SETSIZE];
    client[0].fd=lfd;
    client[0].events=POLLIN;
    int i;
    for(i=1;i<FD_SETSIZE;i++)
    {
        client[i].fd=-1;
    }
    while(1)
    {
        nready=poll(client,maxfd+1,-1);
        if(nready<0)
        {
            if(errno==EINTR)
            {
                printf("select");
                continue;
            }
            close(lfd);
            exit(-1);
        }
        printf("nready=[%d]\n",nready);
        if((client[0].fd==lfd)&&(client[0].revents & POLLIN))
        {
            memset(&clie,0x00,sizeof(clie));
            len=sizeof(clie);
            int cfd=Accept(lfd,(struct sockaddr *)&clie,&len);
            if(cfd>0)
            {
                memset(sIP,0x00,sizeof(sIP));
                printf("client ip:[%s]-->port:[%d]\n",inet_ntop(AF_INET,&clie.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(clie.sin_port));
                for(i=1;i<FD_SETSIZE;i++)
                {
                    if(client[i].fd==-1)
                    {
                        client[i].fd=cfd;
                        client[i].events=POLLIN;
                        break;
                    }
                }
                if(i==FD_SETSIZE)
                {
                    fputs("too many clients\n",stderr);
                    exit(-1);
                }
                if(maxfd<i)
                    maxfd=i;
                if(--nready==0)
                    continue;
            }
        }
        int n,k;
        int socked;
        char buf[1024];
        for(i=1;i<=maxfd;i++)
        {
            if(client[i].fd==-1)
                continue;
            if(client[i].revents & POLLIN)
            {
                socked=client[i].fd;
                memset(buf,0x00,sizeof(buf));
                n=Read(socked,buf,sizeof(buf));
                if(n<=0)
                {
                    printf("client closed or read error:[%d]\n",n);
                    close(socked);
                    client[i].fd=-1;
                }
                else
                {
                    printf("buf=[%s]\n",buf);
                    for(k=0;k<n;k++)
                        buf[k]=toupper(buf[k]);
                    Write(socked,buf,n);
                }
                if(--nready==0)
                    break;
            }
        }
    }
    close(lfd);
    return 0;
}
