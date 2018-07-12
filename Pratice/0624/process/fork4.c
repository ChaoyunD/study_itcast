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
    for(int i=0;i<3;i++)
    {
        if((pid=fork())>0)
        {
            sleep(1);
            continue;
        }
        else if(pid==0)
        {
            printf("进程%d,父进程%d\n",getpid(),getppid());
            exit(0);
        }
    }
   //while(1);
    return 0;
}
