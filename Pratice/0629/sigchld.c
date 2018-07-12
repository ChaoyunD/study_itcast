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
void sighandler(int signo)
{
    int wpid;
    while(1)
    {
        wpid=waitpid(-1,NULL,WNOHANG);
        if(wpid>0)
        {
            printf("child quit,pid=[%d]\n",wpid);
        }
        else if(wpid==0)
        {
            printf("child is living,pid=[%d]\n",wpid);
            break;
        }
        else if(wpid<0)
        {
            printf("no child is living\n");
            break;
        }
    }
}
int main(int argc,char *argv[])
{
    pid_t pid;
    int i;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGCHLD);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    for(i=0;i<3;i++)
    {
        if((pid=fork())==0)
            break;
    }
    if(i==3)
    {
        struct sigaction act;
        act.sa_flags=0;
        act.sa_handler=sighandler;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD,&act,NULL);
        sleep(5);
        sigprocmask(SIG_UNBLOCK,&mask,NULL);
        sleep(20);
    }
    if(i==0)
    {
        printf("i=[%d],fpid=[%d],pid=[%d]\n",i,getppid(),getpid());
        sleep(2);
        exit(0);
    }
    if(i==1)
    {
        printf("i=[%d],fpid=[%d],pid=[%d]\n",i,getppid(),getpid());
        sleep(3);
        exit(0);
    }
    if(i==2)
    {
        printf("i=[%d],fpid=[%d],pid=[%d]\n",i,getppid(),getpid());
        sleep(4);
        exit(0);
    }
    return 0;
}
