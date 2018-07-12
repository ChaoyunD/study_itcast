#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#define N 1024
int main(int argc,char *argv[])
{
    int fd;
    if((fd=open(argv[1],O_RDWR|O_CREAT,0644))<0)
    {
        perror("Error for open");
        return 1;
    }
    printf("fd=%d\n",(int)fd);
    off_t off;
    off=lseek(fd,N-1,SEEK_SET);
    printf("off=%d\n",(int)off);
    if(off<0)
    {
        perror("lseek");
        goto err;
    }
    int len_w=write(fd,"B",1);
    if(len_w<0)
    {
        perror("write");
        goto err;
    }
err:
    close(fd);
    return 0;
}
