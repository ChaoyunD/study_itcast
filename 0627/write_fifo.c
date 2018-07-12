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
    if(argc<2)
    {
        puts("参数过少");
        return -1;
    }
    if(access(argv[1],F_OK)!=0)
    {
        mkfifo(argv[1],0644);
    }
    int fd=open(argv[1],O_WRONLY);
    write(fd,"Hello World",12);
    close(fd);
    return 0;
}
