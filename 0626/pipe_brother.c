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
    /*int ret=*/pipe(fd);
    /*创建两进程*/
    int i;
    pid_t pid;
    for(i=0;i<2;i++)
    {
        if((pid=fork())==0)
            break;
    }
    if(i==0)
    {/*写*/
        close(fd[0]);
        char ch1='a';
        write(fd[1],&ch1,1);
        close(fd[1]);
        exit(0);
    }
    else if(i==1)
    {/*读*/
        close(fd[1]);
        char ch2;
        read(fd[0],&ch2,1);
        printf("子进程:%d,ch=%c\n",getpid(),ch2);
        exit(0);
    }
    //close(fd[0]);//明天问老师
    //close(fd[1]);
    /*父进程*/
    for(i=0;i<2;i++)
        wait(NULL);
    return 0;
}
