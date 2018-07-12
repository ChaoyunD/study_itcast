#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc,char *argv[])
{
    char buf[10];
    int len=read(STDIN_FILENO,buf,sizeof(buf));
    write(STDOUT_FILENO,buf,strlen(buf));
    return 0;
}
