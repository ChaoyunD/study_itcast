#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    pid=fork();
    if(pid==0)
    {
        sleep(60);
        exit(125);
    }
    int wstatus;
    pid_t pid1=waitpid(pid,&wstatus,0);
    printf("pid:%d,pid1:%d\n",pid,pid1);
    if(WIFEXITED(wstatus))
    {
        printf("child exit status:%d\n",WEXITSTATUS(wstatus));
    }
    if(WIFSIGNALED(wstatus))
    {
        printf("signal number:%d\n",WTERMSIG(wstatus));
    }
    return 0;
}
