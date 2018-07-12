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
    int fd[2];
    int ret=pipe(fd);
    if(ret<0)
    {
        perror("pipe");
        return -1;
    }
    //父写子读
    pid_t pid=fork();
    if(pid==0)
    {
        close(fd[1]);
        char buf[10]={0};
        read(fd[0],&buf,1);
        printf("子进程:%c\n",buf[0]);
        close(fd[0]);
        /*子进程写*/
        exit(0);
    }
    /*父进程写*/
    sleep(2);
    close(fd[0]);
    char ch='a';
    write(fd[1],&ch,1);
    /*最后*/
    close(fd[1]);
    return 0;
}
