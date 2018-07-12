#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    struct stat st;
    int ret=stat(argv[1],&st);
    if(ret<0)
    {
        perror("stat");
        return -1;
    }
    /*st.st_mode:获取文件类型*/
    //if((st.st_mode & S_IFMT)==S_IFREG)
    //{
    //    /*普通文件*/    
    //}
    char perm[11]={0};
    switch(st.st_mode & S_IFMT)
    {
    case S_IFREG:perm[0]='-';break;
    case S_IFDIR:perm[0]='d';break;
    default:/**/break;
    }
    perm[1]=((st.st_mode & S_IRWXU) & S_IRUSR)?'r':'-';
    perm[2]=((st.st_mode & S_IRWXU) & S_IWUSR)?'w':'-';
    perm[3]=((st.st_mode & S_IRWXU) & S_IXUSR)?'x':'-';
    char *atime=ctime(&st.st_atime);
    printf("%s %s %s",perm,atime,argv[1]);
    return 0;
}
