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
}
int main(int argc,char *argv[])
{
    signal(SIGINT,sighandler);
    signal(SIGALRM,sighandler);
    int n=alarm(5);
    printf("first :n==[%d]\n",n);

    sleep(2);
    n=alarm(5);
     
    printf("second :n==[%d]\n",n);
    while(1)
    {
        sleep(10);//阻塞函数等待信号处理函数结束后，不会继续执行
    }
    return 0;
}
