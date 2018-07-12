#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
int main(int argc,char *argv[])
{
    if (access(argv[1], F_OK) == -1) 
    {
        if ((mkfifo(argv[1], 0644) < 0) && (errno != EEXIST))
           {
                printf("Cannot create fifo file\n");
                exit(1);                                    
           }        
    }
    int fd=open(argv[1],O_RDONLY);
    while(1)
    {
        char buf[1024]={0};
        read(fd,buf,1024);
        printf("%s\n",buf);
        sleep(1);
    }
    return 0;
}
