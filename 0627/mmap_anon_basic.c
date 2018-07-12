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
    pid_t pid;
    char *mp=mmap(NULL,N,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
    if(mp==MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }
    mp[0]='b';
    pid=fork();
    if(pid==0)
    {
        int i;
        for(i=0;i<N-1;i++)
        {
            mp[i]='a'+i;
        }
        mp[N-1]='\0';
        //mp[0]='a';
        munmap(mp,N);
        exit(0);
    }
    sleep(2);//不靠谱
    printf("Hello:%s\n",mp);
    munmap(mp,N);
    return 0;
}
