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
    sleep(4);//在信号处理函数期间，多次按下会阻塞，信号不会再次发生。临时性设置，
}
int main(int argc,char *argv[])
{
    signal(SIGINT,sighandler);
    while(1)
    {
        sleep(10);//阻塞函数等待信号处理函数结束后，不会继续执行
    }
    return 0;
}
