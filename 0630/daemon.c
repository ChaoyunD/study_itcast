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
#include <time.h>
void sighandler(int signo)
{
    //打开文件
    int fd=open("./mydaemon.log",O_RDWR|O_CREAT|O_APPEND,0644);
    if(fd<0)
    {
        exit(-1);
    }
    //获取系统时间
    time_t t;
    time(&t);
    char *p=ctime(&t);
    //字符串写入文件
    write(fd,p,strlen(p));
    close(fd);
}
int main(int argc,char *argv[])
{
    pid_t pid=fork();
    if(pid<0||pid>0)
    {
        exit(0);
    }
    setsid();
    //改变当前的工作目录
    chdir("/home/itcast");
    //设置文件掩码
    umask(0000);
    //关闭文件描述符
    close(STDIN_FILENO);
    close(STDERR_FILENO);
    close(STDOUT_FILENO);

    //注册信号捕捉函数
    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler=sighandler;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    //核心代码
    struct itimerval tm;
    tm.it_interval.tv_sec=2;
    tm.it_interval.tv_usec=0;
    tm.it_value.tv_sec=3;
    tm.it_value.tv_usec=0;
    setitimer(ITIMER_REAL,&tm,NULL);

    while(1)
    {
        sleep(100);
    }
    return 0;
}
