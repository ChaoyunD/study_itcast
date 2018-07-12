#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char *argv[])
{
    pid_t pid=getpid();
    printf("a.c:pid:%d\n",pid);
    return 0;
}
