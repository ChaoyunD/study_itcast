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
    printf("signo==[%d]\n",signo);
    sleep(4);
}
int main(int argc,char *argv[])
{
    struct sigaction act;
    act.sa_handler=sighandler;
    sigemptyset(&act.sa_mask);//在信号处理函数执行期间，
    sigaddset(&act.sa_mask,SIGQUIT);
    act.sa_flags=0;
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);
    while(1)
    {
        sleep(10);
    }
    return 0;
}
