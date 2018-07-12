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
#include <ctype.h>
#include "wrap.h"
int main(int argc,char *argv[])
{
    //创建socket
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    //绑定bind
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
   // inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    printf("[%x]\n",ntohl(serv.sin_addr.s_addr));
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    //监听listen
    Listen(lfd,128);
    //
    struct sockaddr_in client;
    socklen_t len=sizeof(client);
    int cfd=Accept(lfd,(struct sockaddr *)&client,&len);
    char sIP[16];
    memset(sIP,0x00,sizeof(sIP));
    printf("client info:[%s][%d]\n",inet_ntop(AF_INET,&client.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(client.sin_port));
    int n;
    int buf;
    while(1)
    {
        memset(&buf,0,sizeof(buf));
        n=Read(cfd,&buf,sizeof(buf));
        if(n<=0)
        {
            printf("read error or client closed,n=[%d]\n",n);
            break;
        }
        printf("read over,n=[%d],buf=[%d]\n",n,buf);
        //for(i=0;i<n;i++)
        //{
        //    buf[i]=toupper(buf[i]);
        //}
        buf++;
        Write(cfd,&buf,n);
    }
    Close(cfd);//通信文件描述符
    Close(lfd);//监听文件描述符
    return 0;
}
