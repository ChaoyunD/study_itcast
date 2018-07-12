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
void sighandler(int signo)
{
    while(1)
    {
        int wpid=waitpid(-1,NULL,WNOHANG);
        if(wpid>0)
        {

        }
        if(wpid==0)
        {
            break;
        }
        if(wpid<0)
        {
            break;
        }
    }
    return ;
}
int main(int argc,char *argv[])
{
    pid_t pid;
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
    char buf[1024];
    int n,i;
    int cfd;

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGCHLD);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    while(1)
    {
        //屏蔽信号SIGCHLD
        bzero(&clie,sizeof(clie));
        memset(sIP,0x00,sizeof(sIP));
        cfd=Accept(lfd,(struct sockaddr *)&clie,&len);
        printf("client IP:[%s]-->port:[%d]\n",inet_ntop(AF_INET,&clie.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(clie.sin_port));
        pid=fork();
        if(pid<0)
        {
            perror("fork");
            return -1;
        }
        else if(pid==0)
        {
            Close(lfd);//关闭监听描述符
            break;
        }
        else
        {
            Close(cfd);//关闭客户端文件描述符

            struct sigaction act;
            act.sa_flags=0;
            act.sa_handler=sighandler;
            sigemptyset(&act.sa_mask);
            sigaction(SIGCHLD,&act,NULL);
            sigprocmask(SIG_UNBLOCK,&mask,NULL);
        }
    }
    if(pid==0)
    {
        while(1)
        {
            memset(buf,0x00,sizeof(buf));
            n=Read(cfd,buf,sizeof(buf));
            if(n<=0)
            {
                printf("client closed or read error:[%d]\n",n);
                Close(cfd);
                exit(0);
            }
            for(i=0;i<n;i++)
            {
                buf[i]=toupper(buf[i]);
            }
            Write(cfd,buf,n);
            Write(STDOUT_FILENO,buf,n);
        }
        exit(0);
    }

    return 0;
}
