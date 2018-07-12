#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
int fifo=0;
int reg=0;
int lnk=0;
int sock=0;
int blk=0;
int chr=0;
int d=0;
int unknown=0;
void recurserFindDir(const char *path)
{
    DIR *dir;
    if((dir=opendir(path))==NULL)
    {
        perror("opendir");
        return ;
    }
//    printf("%d\n",d);
    struct dirent *det;
    while((det=readdir(dir))!=NULL)
    {
        if(strcmp(det->d_name,".")==0 || strcmp(det->d_name,"..")==0)
            continue;
        switch(det->d_type)
        {
        case DT_REG:++reg;break;
        case DT_BLK:++blk;break;
        case DT_FIFO:++fifo;break;
        case DT_SOCK:++sock;break;
        case DT_CHR:++chr;break;
        case DT_LNK:++lnk;break;
        case DT_DIR:
            {
                ++d;
                char name[1024]={0};
                sprintf(name,"%s/%s",path,det->d_name);
                recurserFindDir(name);
                break;

            }
        default:++unknown;break;
        }
    } 

    closedir(dir);
    return;
}
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        puts("命令行参数过少");
        return 1;
    }
    recurserFindDir(argv[1]);
    printf("reg=%d\nblk=%d\nchr=%d\nsock=%d\nlnk=%d\nfifo=%d\ndir=%d\nunknown=%d\n",reg,blk,chr,sock,lnk,fifo,d,unknown);
    return 0;
}
