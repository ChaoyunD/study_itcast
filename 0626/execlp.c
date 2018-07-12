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
    printf("b.c:pid:%d\n",pid);
    execlp(argv[1],"a",NULL);
    printf("b.c:pid:%d\n",pid);
    return 0;
}
