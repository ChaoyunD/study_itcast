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
    int fd=open("myfifo",O_RDONLY);
    char buf[1024];
    int len=read(fd,buf,sizeof(buf));
    buf[len]='\0';
    printf("ReadFifo:%s",buf);
    return 0;
}
