#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    if(argc<3)
    {
        fprintf(stderr,"Please input:%s<mode>\n",argv[0]);
    }
    int mode=strtol(argv[2],NULL,8);
    int ret;
    if((ret=chmod(argv[1],(mode_t)mode))<0)//创建会受umask影响，修改不受影响
    {
        perror("chmod");
        return -1;
    }
    return 0;
}
