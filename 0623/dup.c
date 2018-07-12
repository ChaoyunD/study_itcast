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

    int fd2=dup(fd);
    printf("fd2=%d\n",fd2);
err:
    close(fd);
    return 0;
}
