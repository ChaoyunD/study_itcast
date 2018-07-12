#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define N 10
int main(int argc,char *argv[])
{
    char *mp=mmap(NULL,N,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
    if(mp==MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }
    mp[0]='a';
    printf("Hello:%c\n",mp[0]);
    return 0;
}
