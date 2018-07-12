#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
int main(int argc,char *argv[])
{
    DIR *dir=opendir(argv[1]);
    if(dir==NULL)
    {
        perror("opendir");
        return -1;
    }
    while(1)
    {
    struct dirent *de=readdir(dir);
    if(de!=NULL)
    {
  
      if(de->d_type==DT_REG)
        {
            /*普通文件*/
            printf("普通文件\t");
        }
       else if(de->d_type==DT_DIR)
        {
            /*目录文件*/
            printf("目录文件\t");
        }
    }
    else
    {
        goto err;
    }
    printf("%s\n",de->d_name);
    }
err:
    closedir(dir);
    return 0;
}
