#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    int fd;
    if((fd=open(argv[1],O_RDWR|O_CREAT,0644))<0)
    {
        perror("Error for open");
        return 1;
    }
    printf("fd=%d\n",fd);

    int fd3;
    if((fd3=open(argv[2],O_RDWR|O_CREAT,0644))<0)
    {
        perror("Error for open");
        return 1;
    }
    int fd2=dup2(fd,fd3);//dup2()会将fd3指向的文件关闭。而使fd3可用。
    printf("fd3=%d\n",fd3);
    printf("fd2=%d\n",fd2);
    write(fd,"A",1);
    write(fd2,"B",1);
err:
    close(fd);
    return 0;
}
