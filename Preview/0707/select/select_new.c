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
#include "wrap.h"
int main(int argc,char *argv[])
{
    //创建socket
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    //设置端口复用
    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //绑定
    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    //监听
    Listen(lfd,128);

    fd_set tmpset,allset;
    FD_ZERO(&tmpset);
    FD_ZERO(&allset);
    FD_SET(lfd,&allset);
    int nready=0;
    int maxfd=lfd;
    int cfd;
    while(1)
    {
        FD_ZERO(&tmpset);
        tmpset=allset;
        nready=select(maxfd+1,&tmpset,NULL,NULL,NULL);
        if(nready<0)
        {
            continue;
        }
        printf("nready=[%d]\n",nready);
        if(FD_ISSET(lfd,&tmpset))
        {
            cfd=Accept(lfd,NULL,NULL);
            FD_SET(cfd,&allset);
            printf("new client,cfd=[%d]\n",cfd);
            if(maxfd<cfd)
                maxfd=cfd;
            if(--nready==0)
                continue;
        }
        //读数据
        int i;
        int sockfd;
        char buf[1024];
        int n;
        for(i=lfd+1;i<=maxfd;i++)
        {
            sockfd=i;
            if(FD_ISSET(sockfd,&tmpset))
            {
                memset(buf,0x00,sizeof(buf));
                n=Read(sockfd,buf,sizeof(buf));
                if(n<=0)
                {
                    printf("client closed or read error,n=[%d]\n",n);
                    Close(sockfd);
                    FD_CLR(sockfd,&allset);
                }
                else
                {
                    printf("n=[%d],buf=[%s]\n",n,buf);
                    for(int k=0;k<n;k++)
                        buf[k]=toupper(buf[k]);
                    Write(sockfd,buf,n);
                }
                if(--nready==0)
                    break;
            }
        }

    }
    Close(lfd);
    return 0;
}
