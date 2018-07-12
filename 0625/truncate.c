#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    int ret;
    if((ret=truncate(argv[1],1))<0)
    {
        perror("truncate");
        return -1;
    }
    return 0;
}
