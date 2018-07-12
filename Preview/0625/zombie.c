#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    if((pid=fork())==0)
    {
        printf("pid=%d,ppid=%d\n",getpid(),getppid());
        exit(0);
    }
    else
        while(1);
    return 0;
}
