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
    //if((pid=fork())<0)
    //{

    //}
    int i;
    for(i=0;i<2;i++)
    {
        if((pid=fork())==0)
            break;
    }
    if(i==0)//哪个子进程先执行完，父进程就对其收尸
    {
        sleep(4);
        printf("i=0,pid=%d\n",getpid());
        exit(0);
    }
    if(i==1)
    {
        sleep(2);
        printf("i=1,pid=%d\n",getpid());
        exit(0);
    }
    while(i--)
    {
        sleep(1);
        pid_t pid1=wait(NULL);
        printf("pid=%d\t,pid1=%d\n",pid,pid1);
    }
    return 0;
}
