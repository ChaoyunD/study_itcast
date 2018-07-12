#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
int main(int argc,char *argv[])
{
    int fd;
    if((fd=open(argv[1],O_RDWR|O_APPEND))<0)
    {
        perror("Error for open");
        return 1;
    }
    printf("fd=%d\n",fd);
    char buf[1024]="bbbbbbbbbbbbbb\n";
    write(fd,buf,strlen(buf));
    close(fd);
    return 0;
}
