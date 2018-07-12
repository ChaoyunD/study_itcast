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
//    pid_t pid=getpid();
//    printf("b.c:pid:%d\n",pid);
//    execl("./a","a",NULL);
//    printf("b.c:pid:%d\n",pid);
    pid_t pid;
    pid=fork();
    if(pid==0)
    {
        return 0;
    }
    printf("pid:%d\t childpid:%d\n",getpid(),pid);
    while(1);
    return 0;
}
