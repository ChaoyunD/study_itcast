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
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    if((pid=fork())==0)
    {
        close(fd1[0]);
        close(fd2[1]);
        char buf[]="abcdefghigklmnopqrstuvwx";
        //for(int i=0;i<24;i++)
            write(fd1[1],buf,24);
        close(fd1[1]);
        char temp[2]={0};
        for(int i=0;i<24;i++)
        {
            read(fd2[0],temp,1);
            printf("%s",temp);
        }
        printf("\n");
        close(fd2[0]);
        exit(0);
    }
    close(fd1[1]);
    close(fd2[0]);
    char rbuf[25]={0};
    read(fd1[0],rbuf,24);
    close(fd1[0]);
    printf("rbuf=%s\n",rbuf);
    write(fd2[1],&rbuf,24);
    close(fd2[1]);
    sleep(1);
    return 0;
}
