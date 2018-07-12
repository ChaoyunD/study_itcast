//编写守护进程: 每隔2S钟获取一次系统时间，并将这个时间写入磁盘文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

//信号捕捉函数
void sighander(int signo)
{
	//打开文件
	int fd = open("./mydemon.log", O_RDWR | O_CREAT | O_APPEND, 0777);
	if(fd<0)
	{	
		exit(-1);
	}
	
	//获取系统事件
	time_t t;
	time(&t);
	//char *ctime(const time_t *timep);
	char *p = ctime(&t);
	
	//将时间字符串写入文件
	write(fd, p, strlen(p));
	
	close(fd);
}

int main()
{
	//fork子进程, 然后退出
	pid_t pid = fork();
	if(pid<0 || pid>0)
	{
		exit(0);
	}
	
	//子进程调用setsid函数创建会话
	setsid();
	
	//改变当前的工作目录
	chdir("/home/itcast");
	
	//设置文件掩码
	umask(0000);
	
	//关闭文件描述符:STDIN_FILENO STDOUT_FILENO, STDERR_FILENO
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	//核心步骤
	//int setitimer(int which, const struct itimerval *new_value,
    //                 struct itimerval *old_value);
    
    //注册SIGALRM信号捕捉函数
    struct sigaction act;
    act.sa_handler = sighander;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
	sigaction(SIGALRM, &act, NULL);
	
    //设置时钟, 每隔2秒发送SIGALRM信号
    struct itimerval tm;
    tm.it_interval.tv_sec = 2;
    tm.it_interval.tv_usec = 0;
    tm.it_value.tv_sec = 3;
    tm.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &tm, NULL);
    
    while(1)
    {
    	sleep(100);
    }
     
    return 0;
}
