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
    int ret=chdir(argv[1]);//改变的是程序自己的路径，而不是shell的路径
    if(ret<0)
    {
        perror("chdir");
        return 2;
    }
    printf("changed to %s succeed\n",argv[1]);
    char buf[1024];
    printf("当前进程工作目录:%s\n",getcwd(buf,sizeof(buf)));
    printf("buf=%s\n",buf);
    return 0;
}
