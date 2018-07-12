#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char *argv[])
{
    int fd[2];
    int ret=pipe(fd);
    if(ret<0)
    {
        perror("pipe");
        return -1;
    }
    char buf[2]="a";
    char rbuf[2]={0};
    write(fd[1],buf,1);
    read(fd[0],rbuf,1);
    printf("rbuf=%s\n",rbuf);
    return 0;
}
