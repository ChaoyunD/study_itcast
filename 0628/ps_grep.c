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
    pid_t pid;
    int i;
    int fd[2];
    pipe(fd);
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
        execlp("ps","ps","aux",NULL);
        exit(0);
    }
    if(i==1)
    {
        close(fd[1]);
        dup2(fd[0],0);
        close(fd[0]);
        execlp("grep","grep","bash",NULL);
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    for(i=0;i<2;i++)
        wait(NULL);
    return 0;
}
