#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <signal.h>
void sighandler(int signo)
{
   time_t stime;
   time(&stime);
   char str[100]={0};
   //ctime(&stime);
   sprintf(str,"%s",ctime(&stime));
   int fd=open("/home/itcast/study_Guo/Preview/0629/daemon_file",O_RDWR|O_CREAT|O_APPEND,0644);
   write(fd,str,sizeof(str));
   write(fd,"\n",1);
   close(fd);
}
int main(int argc,char *argv[])
{
    pid_t pid;
    if((pid=fork())<0)
    {
        perror("fork");
        return 1;
    }
    if(pid==0)
    {
        if((pid=setsid())<0)
        {
            perror("setsid");
            exit(0);
        }
        chdir("/");
        umask(0000);
        close(STDIN_FILENO);
        close(STDERR_FILENO);
        close(STDOUT_FILENO);
        struct sigaction act;
        act.sa_flags=0;
        sigemptyset(&act.sa_mask);
        act.sa_handler=sighandler;
        sigaction(SIGALRM,&act,NULL);
        struct itimerval tm;
        tm.it_interval.tv_sec=2;
        tm.it_interval.tv_usec=0;
        tm.it_value.tv_sec=3;
        tm.it_value.tv_usec=0; 
        setitimer(ITIMER_REAL,&tm,NULL);
        while(1)
        {
            
        }
    }
    return 0;
}
