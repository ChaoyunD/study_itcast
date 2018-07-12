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
struct stat buf;
void sighandler(int signo)
{
    time_t t;
    time(&t);
    char *p=ctime(&t);
    char buf[1024]={0};
    strcpy(buf,"daemon");
    strcat(buf,p);
    int fd=open(buf,O_RDWR|O_CREAT,0644);
    if(fd<0)
    {
        perror("open");
        exit(-1);
    }

    close(fd);
}
int main(int argc,char *argv[])
{
    int ret=mkdir("./HOME",0777);
    if(ret<0)
    {
        perror("mkdir");
        return -1;
    }
    if((ret=mkdir("./HOME/log",0777))<0)
    {
        perror("mkdir");
        return -1;
    }
    pid_t pid=fork();
    if(pid<0||pid>0)
    {
        exit(0);
    }
    setsid();
    //改变当前的工作目录
    //设置文件掩码
    umask(0000);
    chdir("./HOME/log");
    //关闭文件描述符
    close(STDIN_FILENO);
    close(STDERR_FILENO);
    //close(STDOUT_FILENO);

    //注册信号捕捉函数
    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler=sighandler;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    //核心代码
    struct itimerval tm;
    tm.it_interval.tv_sec=60;
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
