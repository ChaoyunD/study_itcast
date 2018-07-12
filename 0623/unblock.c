#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc,char *argv[])
{
    int flag=fcntl(STDIN_FILENO,F_GETFL);//F_GETFL获取文件状态标志
    int arg=flag|O_NONBLOCK;
    fcntl(STDIN_FILENO,F_SETFL,arg);
    char buf[10];
    int len=read(STDIN_FILENO,buf,sizeof(buf));
    write(STDOUT_FILENO,buf,strlen(buf));
    return 0;
}
