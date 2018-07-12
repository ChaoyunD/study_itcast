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
typedef struct info
{
    struct sockaddr_in clie;
    int index;
    int cfd;
    pthread_t thread;
}INFO;
INFO thinfo[1024];
void *my_thread(void *arg)
{
    INFO *info=(INFO *)arg;
    printf("idx=[%d],cfd=[%d]\n",info->index,info->cfd);
    char sIP[16];
    memset(sIP,0x00,sizeof(sIP));
    printf("client IP:[%s]-->port:[%d]\n",inet_ntop(AF_INET,&(info->clie.sin_addr.s_addr),sIP,sizeof(sIP)),ntohs(info->clie.sin_port));
    int n;
    int cfd=info->cfd;

    char buf[1024];
    while(1)
    {
        memset(buf,0x00,sizeof(buf));
        n=Read(cfd,buf,sizeof(buf));
        if(n<=0)
        {
            printf("client closed or read error:[%d]\n",n);
            Close(cfd);
            info->cfd=-1;
            pthread_exit(NULL);
        }
        for(int i=0;i<n;i++)
        {
            buf[i]=toupper(buf[i]);
        }
        Write(cfd,buf,n);
        Write(STDOUT_FILENO,buf,n);
    }
    pthread_exit(NULL);
}
void init_INFO()
{
    for(int i=0;i<1024;i++)
        thinfo[i].cfd=-1;
}
int findIndex()
{
    int i;
    for(i=0;i<1024;i++)
    {
        if(thinfo[i].cfd==-1)
            break;
    }
    if(i==1024)
    {
        return -1;
    } 
    return i;
}
int main(int argc,char *argv[])
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    printf("1-------------\n");
    //端口复用
    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt));

    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(6666);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    printf("2-------------\n");

    Listen(lfd,128);
    init_INFO();
    printf("3-------------\n");
    struct sockaddr_in clie;
    socklen_t len;
    int cfd;
    int ret;
    int idx;
    //设置线程分离
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    while(1)
    {
        len=sizeof(clie);
        bzero(&clie,sizeof(clie));
        cfd=Accept(lfd,(struct sockaddr *)&clie,&len);
        printf("cfd=%d\n",cfd);
        printf("4-------------\n");
        idx=findIndex();
        if(idx==-1)
        {
            Close(cfd);
            continue;
        }

        printf("5-------------\n");

        thinfo[idx].index=idx;
        thinfo[idx].cfd=cfd;
        memcpy(&thinfo[idx].clie,&clie,sizeof(clie));

        printf("6-------------\n");
        ret=pthread_create(&thinfo[idx].thread,&attr,my_thread,&thinfo[idx]);
        printf("7-------------\n");
        if(ret!=0)
        {
            printf("create thread error[%s]",strerror(ret));
            exit(-1);   
        }
    }
    Close(lfd);
    pthread_exit(NULL);
    return 0;
}
