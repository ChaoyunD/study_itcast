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
    pid_t pid;
    pid=fork();
    if(pid==0)
    {
        execl("/bin/ls","ls",NULL);
        perror("execl");
        exit(0);
    }
    sleep(1);
    printf("Hello World\n");
    return 0;
}
