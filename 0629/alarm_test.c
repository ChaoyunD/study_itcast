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
}
int main(int argc,char *argv[])
{
    //signal(SIGALRM,sighandler);
    alarm(1);
    //printf("first :n==[%d]\n",n);

    //sleep(1);
    //n=alarm(5);
     
    //printf("second :n==[%d]\n",n);
    int i=0;
    while(1)
    {
        printf("[%d]",i++);
    }
    return 0;
}
