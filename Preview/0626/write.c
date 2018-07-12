#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char *argv[])
{
    if(access(argv[1],F_OK)==-1)
    {
        if(mkfifo(argv[1],0644)<0 &&(errno!=EEXIST))
        {
            printf("Cant create fifo file\n");
            exit(0);
        }
    }
    int fd=open(argv[1],O_WRONLY);
    if(fd<0)
    {
        perror("open");
        return -1;
    }
    while(1)
    {
        char buf[]="abcdefghigklmnopqrstuvwx";
        write(fd,&buf,24);
        sleep(1);
    }
    return 0;
}
