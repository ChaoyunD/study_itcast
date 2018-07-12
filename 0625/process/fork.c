#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    int i;
    char *msg;
    pid_t pid;
    pid_t id=getpid();
    printf("父进程pid:  %u\n",id);
    pid=fork();
    if(pid>0)
    {
        printf("父进程cpid:  %u\n",pid);
        sleep(1);
        //msg="我是父进程";
    }
    else if(pid==0)
    {
        pid_t cpid=getpid();
        pid_t cppid=getppid();
        printf("子进程: pid:%d\tppid:%u\n",cpid,cppid);
        //msg="我是子进程";
    }
    //for(i=0;i<5;i++)
    //{
    //    sleep(1);
    //    printf("进程创建:%s\n",msg);
    //}
    return 0;
}
