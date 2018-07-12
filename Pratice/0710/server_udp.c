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
#include <poll.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include "wrap.h"
int main(int argc,char *argv[])
{
    int sockfd=Socket(AF_INET,SOCK_DGRAM,0);
    if(lfd<0)
    {
        perror("socket error");
        return -1;
    }
    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    bind(sockfd,(struct sockaddr *)&serv,sizeof(serv));

    int n;
    char buf[1024];
    struct sockaddr_in client;
    socklen_t len=sizeof(client);
    while(1)
    {
        n=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client,&len);
        //与tcp不同的是，udp在未完整收取数据的时候，剩余的数据会舍弃掉
        if(n<0)
        {
            printf("recvfrom error:n=[%d]\n",n);
            continue;
        }
        else if(n==0)
        {
            printf("read over:n=[%d]\n",n);
            continue;
        }
        else
        {
            printf("buf=[%s]\n",buf);
            for(int i=0;i<n;i++)
            {
                buf[i]=toupper(buf[i]);
            }
            sendto(sockfd,buf,n,0,(struct sockaddr*)&client,sizeof(client));
        }
    }
    Close(sockfd);
    return 0;
}
