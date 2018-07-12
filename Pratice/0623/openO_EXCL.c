#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
    int fd;
    if((fd=open(argv[1],O_RDWR|O_CREAT|O_EXCL,0644))<0)
    {
        perror("open");
        return 1;
    }

    return 0;
}
