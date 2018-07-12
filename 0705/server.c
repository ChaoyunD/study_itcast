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
int main(int argc,char *argv[])
{
    //创建socket
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    if(lfd<0)
    {
        perror("socket");
        return -1;
    }
    //绑定bind
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
   // inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    printf("[%x]\n",ntohl(serv.sin_addr.s_addr));
    bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    //监听listen
    listen(lfd,128);
    //
    struct sockaddr_in client;
    int len=sizeof(client);
    int cfd=accept(lfd,(struct sockaddr *)&client,&len);
    if(cfd<0)
    {
        perror("accept error");
        close(lfd);
        return -1;
    }
    char sIP[16];
    memset(sIP,0x00,sizeof(sIP));
    printf("client info:[%s][%d]\n",inet_ntop(AF_INET,&client.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(client.sin_port));
    int n;
    int i;
    int buf;
    while(1)
    {
        memset(&buf,0,sizeof(buf));
        n=read(cfd,&buf,sizeof(buf));
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
        write(cfd,&buf,n);
    }
    close(cfd);//通信文件描述符
    close(lfd);//监听文件描述符
    return 0;
}
