#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
int main(int argc,char *argv[])
{
    int fd=open(argv[1],O_RDONLY);
    int len=lseek(fd,0,SEEK_END);
    char *mp=mmap(NULL,len,PROT_READ,MAP_SHARED,fd,0);
    if(mp==MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }
    int n=100;
    while(n--)
    {
        sleep(1);
        printf("%d\t%s\n",n,mp);
    }
    munmap(mp,len);
    close(fd);
    return 0;
}
