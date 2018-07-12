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
        puts("命令行参数不够");
        return 1;
    }
    int ret=access(argv[1],R_OK|W_OK|F_OK);//查看当前用户对文件的权限
    if(ret<0)
    {
        perror("access");
        return 2;
    }
    else
    {
        puts("当前用户对文件具有读权限");
        puts("当前用户对文件具有写权限");
    }
    ret=chmod(argv[1],0777);//修改权限
    if(ret<0)
    {
        perror("chmod");
        return 3;
    }
    ret=chown(argv[1],1001,1001);//修改文件所属用户和群组
    //需有root权限
    if(ret<0)
    {
        perror("chown");
        return 4;
    }
    ret=truncate(argv[1],1024);
    if(ret<0)
    {
        perror("truncate");
        return 4;
    }

    return 0;
}
