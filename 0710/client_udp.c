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
#include <poll.h>
#include <sys/epoll.h>
int main()
{
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("sockfd error");
        return -1;
    }
    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);

    int n;
    char buf[1024];

    while(1)
    {
        int len=sizeof(serv);
        memset(buf,0x00,sizeof(buf));
        n=read(STDIN_FILENO,buf,sizeof(buf));
        if(n<0)
        {
            break;
        }
        sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&serv,len);
        memset(buf,0x00,sizeof(buf));
        n=recvfrom(sockfd,buf,sizeof(buf),0,NULL,NULL);
        if(n<0)
        {
            printf("recvfrom error,n=[%d]",n);
            continue;
        }
        else if(n==0)
        {
            printf("no data to read,n=[%d]\n",n);
            continue;
        }
        else
        {
            printf("recvfrom over,n==[%d],buf=[%s]\n",n,buf);
        }
    }
    close(sockfd);
    return 0;
}
