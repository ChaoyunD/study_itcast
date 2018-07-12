#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    int status;
    int ret;
    if((pid=fork())==0)
    {
        printf("pid=%d,ppid=%d\n",getpid(),getppid());
        sleep(15);
        exit(1);
    }
    ret=wait(&status);
    if(WIFEXITED(status))
    {
        printf("正常退出,status=%d\n",WEXITSTATUS(status));
    }
    if(WIFSIGNALED(status))
    {
        printf("非正常退出,被某个信号杀死了,signal=%d\n",WTERMSIG(status));
        sleep(10);
    }
    return 0;
}
