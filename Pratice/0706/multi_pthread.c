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
char buf[1024];
int n,i;
void *my_thread(void *arg)
{
    int cfd=*(int *)arg;
    while(1)
    {

        memset(buf,0x00,sizeof(buf));
        n=Read(cfd,buf,sizeof(buf));
        if(n<=0)
        {
            printf("client closed or read error:[%d]\n",n);
            Close(cfd);
            pthread_exit(NULL);
        }
        for(i=0;i<n;i++)
        {
            buf[i]=toupper(buf[i]);
        }
        Write(cfd,buf,n);
        Write(STDOUT_FILENO,buf,n);
    }
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    //端口复用
    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt));

    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);

    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    Listen(lfd,128);
    struct sockaddr_in clie;
    socklen_t len=sizeof(clie);
    char sIP[1024];
    int cfd;
    pthread_t thread;
    //设置线程分离
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    while(1)
    {
        bzero(&clie,sizeof(clie));
        memset(sIP,0x00,sizeof(sIP));
        cfd=Accept(lfd,(struct sockaddr *)&clie,&len);
        printf("client IP:[%s]-->port:[%d]\n",inet_ntop(AF_INET,&clie.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(clie.sin_port));
        pthread_create(&thread,&attr,my_thread,(void *)&cfd);
    }
    pthread_exit(NULL);
    return 0;
}
