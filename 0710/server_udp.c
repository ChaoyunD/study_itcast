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
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    bind(sockfd,(struct sockaddr *)&serv,sizeof(serv));

    int n;
    char buf[1024];

    struct sockaddr_in client;
    socklen_t len;
    while(1)
    {
        len=sizeof(client);
        memset(buf,0x00,sizeof(buf));
        bzero(&client,sizeof(client));
        n=recvfrom(sockfd,buf,3,0,(struct sockaddr *)&client,&len);
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
            int i;
            printf("recvfrom over,n==[%d],buf=[%s]\n",n,buf);
            for(i=0;i<n;i++)
            {
                buf[i]=toupper(buf[i]);
            }
            sendto(sockfd,buf,n,0,(struct sockaddr *)&client,len);
        }
    }
    close(sockfd);
    return 0;
}
