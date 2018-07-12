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
#include "wrap.h"
int main(int argc,char *argv[])
{
    int cfd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_port=htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&client.sin_addr.s_addr);

    connect(cfd,(struct sockaddr *)&client,sizeof(client));

    fd_set tmpset,allset;
    FD_ZERO(&tmpset);
    FD_SET(cfd,&allset);
    FD_SET(STDIN_FILENO,&allset);
    int maxfd=cfd;
    int nready=0;
    char buf[1024];
    int n;
    while(1)
    {
        FD_ZERO(&tmpset);
        tmpset=allset;
        nready=select(maxfd+1,&tmpset,NULL,NULL,NULL);
        printf("nready=[%d]\n",nready);
        if(nready<0)
        {
            if(errno==EINTR)
            {
                perror("select");
                continue;
            }
            close(cfd);
            exit(-1);
        }
        if(FD_ISSET(STDIN_FILENO,&tmpset))
        {
            memset(buf,0x00,sizeof(buf));
            n=Read(STDIN_FILENO,buf,sizeof(n));
            Write(cfd,buf,n);
            if(--nready==0)
                continue;
        }
        if(FD_ISSET(cfd,&tmpset))
        {
            memset(buf,0x00,sizeof(buf));
            n=Read(cfd,buf,sizeof(buf));
            if(n<=0)
            {
                printf("read error or server closed:[%d]\n",n);
                Close(cfd);
                exit(-1);
            }
            Write(STDOUT_FILENO,buf,n);
            if(--nready==0)
                continue;
        }

    }
    close(cfd);
    return 0;
}
