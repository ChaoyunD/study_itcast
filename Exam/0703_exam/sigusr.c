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
        signal(SIGUSR1,father);
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
        signal(SIGUSR2,son);
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
