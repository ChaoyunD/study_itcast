#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	int fdr,fdw;
          if((fdr=open(argv[1],O_RDWR))<0)
          {
                  perror("open:");
                  exit(1);
          }
          char buf[1024];
          int len;
          len=read(fdr,buf,sizeof(buf));
	if(len<0)
	{
		return 0;
	}
	buf[len]='\0';
//	printf("%d",fdw);
	len=write(STDIN_FILENO,buf,strlen(buf));
	if(len<0)
	{
		goto arr;
	}
//	printf("%s\n",buf);
arr:
	close(fdr);
	exit(0);
}
