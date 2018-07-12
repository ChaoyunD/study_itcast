#include <stdio.h>
#include <errno.h>
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
int main(int argc,char *argv[])
{
    int cfd=socket(AF_UNIX,SOCK_STREAM,0);
    if(cfd<0)
    {
        perror("socket error");
        return -1;
    }
    unlink("./client.sock");
    struct sockaddr_un client;
    bzero(&client,sizeof(client));
    client.sun_family=AF_UNIX;
    strcpy(client.sun_path,"client.sock");
    bind(cfd,(struct sockaddr *)&client,sizeof(client));

    struct sockaddr_un serv;
    serv.sun_family=AF_UNIX;
    strcpy(serv.sun_path,"server.sock");
    connect(cfd,(struct sockaddr *)&serv,sizeof(serv));

    int n;
    char buf[1024];
    while(1)
    {
        memset(buf,0x00,sizeof(buf));
        n=read(STDIN_FILENO,buf,sizeof(buf));
        if(n<0)
        {
            if(errno==EINTR)
            {
                continue;
            }
            break;
        }
        send(cfd,buf,n,0);
        memset(buf,0x00,sizeof(buf));
        n=recv(cfd,buf,sizeof(buf),0);
        if(n<0)
        {
            break;
        }
        printf("buf=[%s]\n",buf);
    }
    close(cfd);
    return 0;
}
