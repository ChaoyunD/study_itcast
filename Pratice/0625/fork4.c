#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    int status;
    for(int i=0;i<3;i++)
    {
        if((pid=fork())==0)
        {
            printf("第%d个子进程,子进程%d,父进程%d\n",i+1,getpid(),getppid());
            exit(0);
        }
        else if(pid>0)
        {
           pid=wait(&status);
           printf("wait的返回值(子进程的pid)=%d,status为%d\n",pid,status);
           continue;
        }
    }
   //while(1);
    return 0;
}
