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
    pid_t pid;
    int fd[2];
    pipe(fd);
    if((pid=fork())==0)
    {
        close(fd[0]);
        char buf[]="abcdefghigklmnopqrstuvwxyz";
        //for(int i=0;i<24;i++)
            write(fd[1],buf,24);
        close(fd[1]);
        exit(0);
    }
    close(fd[1]);
    char rbuf[2]={0};
    for(int i=0;i<24;i++)
    {
        read(fd[0],&rbuf[0],1);
        printf("rbuf=%s\n",rbuf);
    }
    close(fd[0]);
    return 0;
}
