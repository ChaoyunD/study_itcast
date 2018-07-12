#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define MAX 100
int main(int argc,char *argv[])
{
    pid_t pid;
    char *p=(char*)mmap(NULL,MAX,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if((pid=fork())==0)
    {
        sleep(1);
        printf("child got a message:%s\n",p);
        sprintf(p,"%s","Hi,dad,this is son!");
        munmap(p,MAX);
        exit(0);
    }
    sprintf(p,"%s","Hi,son.This is Dad!");
    sleep(2);
    printf("dad got a message:%s\n",p);
    return 0;    
}
