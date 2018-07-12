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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "wrap.h"
#define PORT 6666
#define IP "127.0.0.1"
int main(void)
{
    int cfd;
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&saddr.sin_addr.s_addr);
    cfd=Socket(AF_INET,SOCK_STREAM,0);
    char buf[1024];
    char des[1024];
    Connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr));
    while(1)
    {
        memset(des,0,1024);
        memset(buf,0,1024);
        int n=Read(STDIN_FILENO,buf,1024);
        Write(cfd,buf,n);/*记住这几行代码调试了很长呢时间，应该是buf的问题*/
        int len=Read(cfd,des,n);
        Write(STDOUT_FILENO,des,len);
    }
    close(cfd);
    return 0;
}
