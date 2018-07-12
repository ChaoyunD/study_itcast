#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char file_type(const struct stat *buf)
{   
    char ch='-';
    if(S_ISREG(buf->st_mode))
    {
        ch='-';
    }
    else if(S_ISDIR(buf->st_mode))
    {
        ch='d';
    }

    return ch;
}

char *file_mode(char mode[],const struct stat *buf)
{
    if(buf->st_mode & S_IRUSR)
    {
        mode[0]='r';
    }
    else
    {
        mode[0]='-';
    }
    if(buf->st_mode & S_IWUSR)
    {
        mode[1]='w';
    }
    else
    {
        mode[1]='-';
    }
    if(buf->st_mode & S_IXUSR)
    {
        mode[2]='x';
    }
    else
    {
        mode[2]='-';
    }
    return mode;
}
char *file_time(char time[],const struct stat *buf)
{
    return time;
}
int main(int argc,char *argv[])
{
    struct stat buf;
    int ret=stat(argv[1],&buf);
    if(ret<0)
    {
        perror("stat");
        return 1;
    }
    char time[]="6月\t23\t14:28";
    //char mode[10]="xw-r--r--";
    char mode[10]={0};
    printf("%c\n%s\n%d\n%s\n",
    file_type(&buf),
    file_mode(mode,&buf),
    (int)buf.st_nlink,
    file_time(time,&buf));
    return 0;
}
