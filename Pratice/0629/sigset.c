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
    printf("signo=[%d]\n",signo);
}
int main(int argc,char *argv[])
{
    signal(SIGINT,sighandler);
    signal(SIGQUIT,sighandler);

    sigset_t pending,mask,oldmask;

    sigemptyset(&pending);
    sigemptyset(&mask);

    sigaddset(&mask,SIGINT);
    sigaddset(&mask,SIGQUIT);

    //sigprocmask(SIG_BLOCK,&mask,NULL);
    sigprocmask(SIG_BLOCK,&mask,&oldmask);
    int i=1;
    int k=1;
    while(1)
    {
        sigpending(&pending);
        for(i=1;i<32;i++)
        {
            if(sigismember(&pending,i)==1)
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
        }
        printf("\n");
        if(k++%10==0)
        {
            sigprocmask(SIG_SETMASK,&oldmask,NULL);
        }
        else
        {
            sigprocmask(SIG_BLOCK,&mask,NULL);
        }
        sleep(1);
    }

    return 0;
}
