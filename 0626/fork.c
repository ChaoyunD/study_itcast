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
    int i;
    for(i=0;i<10;i++)
    {
        pid=fork();
        if(pid==0)
        {
            break;
        }
    }

    if(pid<0)
    {/**/
        
    }
    switch(i)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:exit(0);
    }
    //if(i==0)
    //{/*第0个子进程*/
    //    //printf("我是第一个子进程\n");
    //}
    //if(i==1)
    //{/*第一个子进程*/
    //    
    //}
    if(i==9)
    {
        printf("我是第%d个子进程\n",i);
        exit(0);
    }
    //if(pid==0)
    //{/*子进程*/
    //    printf("子进程:i=%d\n",i);
    //    exit(0);
    //}
    /*父进程*/
    printf("父进程:i=%d\n",i);
    while(1);
    return 0;
}
