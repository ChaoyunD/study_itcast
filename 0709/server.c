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

    fd_set tmpset,readset;
    FD_ZERO(&tmpset);
    FD_ZERO(&readset);
    int maxfd=lfd;
    FD_SET(lfd,&readset);
    int nready=0;
    while(1)
    {
        FD_ZERO(&tmpset);
        tmpset=readset;
        nready=select(maxfd+1,&tmpset,NULL,NULL,NULL);
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
        if(FD_ISSET(lfd,&tmpset))
        {
            memset(&clie,0x00,sizeof(clie));
            len=sizeof(clie);
            int cfd=Accept(lfd,(struct sockaddr *)&clie,&len);
            if(cfd>0)
            {
                memset(sIP,0x00,sizeof(sIP));
                printf("client ip:[%s]-->port:[%d]\n",inet_ntop(AF_INET,&clie.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(clie.sin_port));
                FD_SET(cfd,&readset);
                if(maxfd<cfd)
                {
                    maxfd=cfd;
                }
                if(--nready==0)
                    continue;
            }
        }
        int n,i,k;
        int socked;
        char buf[1024];
        for(i=lfd+1;i<=maxfd;i++)
        {
            socked=i;
            if(FD_ISSET(socked,&tmpset))
            {
                memset(buf,0x00,sizeof(buf));
                n=Read(socked,buf,sizeof(buf));
                if(n<=0)
                {
                    printf("client closed or read error:[%d]\n",n);
                    close(socked);
                    FD_CLR(socked,&readset);
                }
                else
                {
                    for(k=0;k<n;k++)
                        buf[k]=toupper(buf[k]);
                    Write(socked,buf,n);
                }
                if(--nready==0)
                    break;
            }
        }
    }

}
