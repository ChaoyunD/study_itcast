#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define N 1024
int main(int argc,char *argv[])
{
    int fd;
    if(access(argv[1],F_OK)<0)
    {
        fd=open(argv[1],O_RDWR|O_CREAT,0644);
    }
    else
    {
        fd=open(argv[1],O_RDWR);
    }
    lseek(fd,N-1,SEEK_SET);
    write(fd,"b",1);
    int len=lseek(fd,0,SEEK_END);
    char *mp=mmap(NULL,len,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    if(mp==MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }
    while(1)
    {
        printf("%s\n",mp);
        sleep(1);
    }
    munmap(mp,len);
    return 0;
}
