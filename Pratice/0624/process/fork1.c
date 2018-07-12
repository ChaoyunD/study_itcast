#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    int i=0;
    if((pid=fork())<0)
    {
        perror("fork");
        return 1;
    }
    //else if(pid>0)
    //{
    //    puts("-------------");
    //    printf("子进程的pid=%d\n",pid);
    //    printf("这是父进程,当前进程的pid=%d,当前进程的父进程的pid=%d\n",getpid(),getppid());
    //    puts("-------------");
    //    sleep(5);
    //}
    while(1)
    {
        printf("%d\n",i++);
        sleep(2);
    }
    //else if(pid==0)
    //{
    //    puts("-------------");
    //    printf("这是子进程,当前进程的pid=%d,父进程的pid=%d\n",getpid(),getppid());
    //    sleep(5);
    //    puts("-------------");
    //    sleep(5);
    //}
    return 0;
}
