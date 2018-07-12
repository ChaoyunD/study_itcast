#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
int main(int argc,char *argv[])
{
    int fd[2];
    pipe(fd);
    int fd1=dup(1);
    int i;
    pid_t pid;
    for(i=0;i<2;i++)
    {
        if((pid=fork())==0)
            break;
    }
    if(i==0)
    {
        close(fd[0]);
        dup2(fd[1],1);
        close(fd[1]);
        execl("./write","write",NULL);
        write(fd1,"Error\n",6);
        exit(0);
    }
    else if(i==1)
    {
        close(fd[1]);
        dup2(fd[0],0);
        close(fd[0]);
        execl("./read","read",NULL);
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    for(i=0;i<2;i++)
    {
        wait(NULL);
    }
}
