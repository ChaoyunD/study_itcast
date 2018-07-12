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
    int fd=open(argv[1],O_RDWR);
    int len=lseek(fd,0,SEEK_END);
    char *mp=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(mp==MAP_FAILED)
    {
        perror("mmap");
        goto err;
    }
    *mp='b';
    sleep(100);
    munmap(mp,len);
    //memcpy(mp,buf,sizeof(buf));//buf的大小大于映射
err:
    close(fd);
    return 0;
}
