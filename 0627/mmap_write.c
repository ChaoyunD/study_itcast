#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define FILE_SIZE 1024
int main(int argc,char *argv[])
{
    int fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0644);
    lseek(fd,FILE_SIZE-1,SEEK_SET);//扩宽文件
    //lseek仅将当前的文件偏移量记录在内核中，并不引起任何I/O操作，
    //然后，该偏移量用于下一个读或写操作。
    write(fd,"a",1);
    int len=lseek(fd,0,SEEK_END);
    printf("len=%d\n",len);
    char *mp=mmap(NULL,len,PROT_WRITE,MAP_SHARED,fd,0);
    if(mp==MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }
    int n=100;
    while(n--)
    {
        sleep(1);
        memcpy(mp,"Hello World",12);
    }
    munmap(mp,len);
    close(fd);
    return 0;
}
