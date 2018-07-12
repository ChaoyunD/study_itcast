#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    if(argc<3)
    {
        puts("命令行参数过少");
        return 1;
    }
    int ret=rename(argv[1],argv[2]);
    if(ret<0)
    {
        perror("rename");
        return 2;
    }
    return 0;
}
