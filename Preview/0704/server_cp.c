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
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 6666
#define IP "127.0.0.1"
int main(void)
{
    char buf[1024];
    int sfd,cfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr,caddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    bind(sfd,(struct sockaddr *)&saddr,sizeof(saddr));
    listen(sfd,128);
    socklen_t len=sizeof(caddr);
    cfd=accept(sfd,(struct sockaddr *)&caddr,&len);
    while(1)
    {
        int n=read(cfd,buf,sizeof(buf));
        for(int i=0;i<n;i++)
            buf[i]=toupper(buf[i]);
        write(cfd,buf,n);
    }
    close(cfd);
    close(sfd);
    return 0;
}
