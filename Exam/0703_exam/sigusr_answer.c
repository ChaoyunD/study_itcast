#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
int num=1;
int flag;
void father(int signo)
{
    printf("father process:num[%d]\n",num);
    num+=2;;
    flag=0;
    sleep(1);
}
void son(int signo)
{
    printf("child process:num[%d]\n",num);
    num+=2;
    flag=0;
    sleep(1);

}
void sigint(int signo)
{

}
int main(int argc,char *argv[])
{
    int pid;
    if((pid=fork())<0)
    {
        perror("fork");
        return -1;
    }
    if(pid>0)
    {
        num=0;
        flag=1;
        struct sigaction act;
        act.sa_flags=0;
        act.sa_handler=father;
        sigaddset(&act.sa_mask,SIGUSR1);
        sigprocmask(SIG_BLOCK,&act.sa_mask,NULL);
        /*不加sigprocmask()的意思是，在信号处理函数中，再次发生SIGUSR1会阻塞，而不是不能触发SIGUSR1信号。信号处理函数正常运行。
         * 加入sigprocmask()之后，阻塞信号集相应位变为1，未决信号集为1时，不会执行信号处理函数。-->信号不会被处理(信号处理函数不运行)*/
        sigaction(SIGUSR1,&act,NULL);
        //signal(SIGUSR1,father);
        //signal(SIGINT,sigint);
        while(1)
        {
            if(flag==0)
            {
                kill(pid,SIGUSR2);
                flag=1;
            }
        }
    }
    if(pid==0)
    {
        num=1;
        flag=0;
        struct sigaction act;
        act.sa_flags=0;
        act.sa_handler=son;
        sigaddset(&act.sa_mask,SIGUSR2);
        sigprocmask(SIG_BLOCK,&act.sa_mask,NULL);
        sigaction(SIGUSR2,&act,NULL);
        //signal(SIGUSR2,son);
        //signal(SIGINT,sigint);
        while(1)
        {
            if(flag==0)
            {
                kill(getppid(),SIGUSR1);
                flag=1;
            }
        }
    }
    return 0;
}
