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
    char buf[10]={0};
    int len=read(0,buf,10);
    printf("#%s\n",buf);
    return 0;
}
