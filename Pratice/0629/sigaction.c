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
    sleep(4);
}
int main(int argc,char *argv[])
{
    struct sigaction act;
    act.sa_handler=sighandler;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);//信号处理函数执行中，屏蔽SIGQUIT信号，导致ctrl+c之后再次ctrl+\，会阻塞SIGQUIT，直到函数执行完毕，signal()才会捕获SIGQUIT。而且信号并不支持排队。
    sigaction(SIGINT,&act,NULL);
    
    signal(SIGQUIT,sighandler);//此处语句并没有阻塞ctrl+c,所以先按下ctrl+\再按下ctrl+c，会先后执行，并没有阻塞。而且信号不支持排队。
    while(1)//ctrl+c,ctrl+\,ctrl+c,先显示2，后显示2,最后显示3。可能跟代码段的先后顺序有关。为什么不是后3，最后2.
    {
        sleep(10);
    }
    return 0;
}
