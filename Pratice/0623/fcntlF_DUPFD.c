#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    int fd;
    if((fd=open(argv[1],O_RDWR|O_CREAT,0644))<0)
    {
        perror("open");
        return 1;
    }
    printf("fd=%d\n",fd);
    int fd2=fcntl(fd,F_DUPFD,7);
    if(fd2<0)
    {
        perror("fcntl");
        return 1;
    }
    printf("fd=%d\n",fd);
    printf("fd2=%d\n",fd2);
    write(fd,"A",1);
    write(fd2,"B",1);
    return 0;
}
