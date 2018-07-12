#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        puts("命令行参数过少");
        return 1;
    }
    umask(0000);
    int ret;
    if((ret=mkdir(argv[1],0777))<0)
    {
        perror("mkdir");
        return 1;
    }
    sleep(15);
    if((ret=rmdir(argv[1]))<0)
    {
        perror("rmdir");
        return 2;
    }
    return 0;

}
