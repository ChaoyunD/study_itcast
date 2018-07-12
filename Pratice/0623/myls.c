#include <time.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
void file_mode_and_type(const struct stat *buf)
{
    
    switch(buf->st_mode&S_IFMT)
    {
    case S_IFBLK:printf("b");break;
    case S_IFCHR:printf("c");break;
    case S_IFDIR:printf("d");break;
    case S_IFIFO:printf("p");break;
    case S_IFLNK:printf("l");break;
    case S_IFREG:printf("-");break;
    case S_IFSOCK:printf("s");break;
    default:printf("unknown?");break;
    }    
    char type[10]={0};
    if(buf->st_mode & S_IRUSR)
    {
        type[0]='r';
    }
    else
    {
        type[0]='-';
    }
    if(buf->st_mode & S_IWUSR)
    {
        type[1]='w';
    }
    else
    {
        type[1]='-';
    }
    if(buf->st_mode & S_IXUSR)
    {
        type[2]='x';
    }
    else
    {
        type[2]='-';
    }
    if(buf->st_mode & S_IRGRP)
    {
        type[3]='r';
    }
    else
    {
        type[3]='-';
    }
    if(buf->st_mode & S_IWGRP)
    {
        type[4]='w';
    }
    else
    {
        type[4]='-';
    }
    if(buf->st_mode & S_IXGRP)
    {
        type[5]='x';
    }
    else
    {
        type[5]='-';
    }
    if(buf->st_mode & S_IROTH)
    {
        type[6]='r';
    }
    else
    {
        type[6]='-';
    }
    if(buf->st_mode & S_IWOTH)
    {
        type[7]='w';
    }
    else
    {
        type[7]='-';
    }
    if(buf->st_mode & S_IXOTH)
    {
        type[8]='x';
    }
    else
    {
        type[8]='-';
    }
    type[9]='\0';
    printf("%s ",type);
}
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        puts("命令行参数不够");
        return 1;
    }
    struct stat buf;
    int ret=stat(argv[1],&buf);
    if(ret<0)
    {
        perror("stat");
        return 1;
    }
    file_mode_and_type(&buf);
    printf("%d ",(int)buf.st_nlink);
    struct passwd *user=getpwuid(buf.st_uid);
    struct group *group=getgrgid(buf.st_gid);
    printf("%s %s ",user->pw_name,group->gr_name);
    printf("%d ",(int)buf.st_size);
    
    struct timespec m_time=buf.st_mtim;
    clock_gettime(CLOCK_REALTIME,&m_time);
    printf("%.24s ",ctime(&m_time.tv_sec));
    printf("%s \n",argv[1]);
    return 0;
}
