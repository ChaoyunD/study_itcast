#include <stdio.h>
#include "wrap.h"
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
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
    Connect(lfd,(struct sockaddr *)&serv,sizeof(serv));
    int buf;
    while(1)
    {
        memset(&buf,0,sizeof(buf));
        scanf("%d",&buf);
        Write(lfd,&buf,sizeof(buf));
        int n=Read(lfd,&buf,sizeof(buf));
        printf("%d\n",buf);
    }
    close(lfd);//监听文件描述符
    return 0;
}
