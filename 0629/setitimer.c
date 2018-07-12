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
void sighandler(int signo)
{
    printf("hello world\n");
}
int main(int argc,char *argv[])
{
    signal(SIGALRM,sighandler);
    struct itimerval tm;
    tm.it_interval.tv_sec=1;
    tm.it_interval.tv_usec=0;
    tm.it_value.tv_sec=3;
    tm.it_value.tv_usec=0;
    setitimer(ITIMER_REAL,&tm,NULL);
    while(1)
    {
        sleep(10);//阻塞函数等待信号处理函数结束后，不会继续执行
    }
    return 0;
}
