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
int main(void)
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));

    Listen(lfd,128);

    int cfd;
    pid_t pid;
    struct sockaddr_in client;
    socklen_t len;
    while(1)
    {
        len=sizeof(client);
        bzero(&client,sizeof(client));
        cfd=Accept(lfd,(struct sockaddr *)&client,&len);
        pid=fork();
        if(pid<0)
        {
            exit(0);
        }
        else if(pid>0)
        {
            Close(cfd);
        }
        else
        {
            int n;
            int i;
            char buf[1024];
            Close(lfd);
            while(1)
            {
                memset(buf,0x00,sizeof(buf));
                n=Read(cfd,buf,sizeof(buf));
                if(n<=0)
                {
                    printf("read error client closed,n==[%d]",n);
                    break;
                }
                for(i=0;i<n;i++)
                {
                    buf[i]=toupper(buf[i]);
                }
                Write(cfd,buf,n);
            }
            Close(cfd);
            exit(0);//若没有该句代码，子进程也会fork()子进程
            //break;
        }
    }
    return 0;
}
