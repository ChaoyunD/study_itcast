#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc,char *argv[])
{
    int fd;
    if((fd=open("/dev/tty",O_RDONLY))<0)
    {
        perror("open");
    }
    int flag=fcntl(fd,F_GETFL);//F_GETFL获取文件状态标志
    int arg=flag|O_NONBLOCK;
    flag=fcntl(fd,F_SETFL,arg);
    if(flag<0)
    {
        perror("flag");
        return 1;
    }
    char buf[10];
    while(1)
    {
        int len=read(fd,buf,sizeof(buf));
        if(len<0)
        {
            if(errno!=EAGAIN)
              {
                  return 1;

              }   
        }
        else
        {
           break; 
        }
        sleep(2);
        printf("没有数据\n");
    }
        write(STDOUT_FILENO,buf,strlen(buf));
    return 0;
}
