#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(void)
{
    int fd[2];
    pid_t pid;
    pipe(fd);
    int i;
    for(i=0;i<2;i++)
    {
        if((pid=fork())==0)
            break;
    }
    if(i==0)
    {
        close(fd[0]);
        char buf[]="abcdefghijklmnopqrstuvwxyz";
        for(int i=0;i<26;i++)
        {
            write(fd[1],&buf[i],1);//所有读端已经关闭，而写端还在写，会引发sigpipe信号，导致异常关闭
            sleep(2);
        }
        close(fd[1]);
        exit(0);
    }
    if(i==1)
    {
        close(fd[1]);
        char rbuf[9]={0};
        for(int i=0;i<3;i++)
        {
            sleep(16);
            read(fd[0],rbuf,8);
            printf("rbuf=%s\n",rbuf);
        }
        close(fd[0]);
        exit(0);
    }
    //close(fd[0]);//有疑问，如果父进程fd读端和写端都没有关闭，为什么写端的status为0
    //close(fd[1]);
  //  for(int i=0;i<2;i++)
  //      wait(NULL)
    int wstatus;
    for(int i=0;i<2;i++)
    {
    pid_t pid1=wait(&wstatus);
    printf("pid:%d,pid1:%d\n",pid,pid1);
    if(WIFEXITED(wstatus))
     {                                                                                                                                         
       printf("child exit status:%d\n",WEXITSTATUS(wstatus));
     }
    if(WIFSIGNALED(wstatus))
     {
       printf("signal number:%d\n",WTERMSIG(wstatus));
     }
    }
    char buf[9]={0};
    close(fd[1]);
    read(fd[0],buf,9);
    printf("%s\n",buf);
    close(fd[0]);
    return 0;
}
