#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	int fd;
	if((fd=open(argv[1],O_RDONLY))<0)
	{
		perror("open:");
		exit(1);
	}
	char buf[24];
	int pos=lseek(fd,2,SEEK_SET);
	if(pos<0)
	{
		perror("lseek");
		exit(2);
	}
	int len;
	if(len=read(fd,buf,sizeof(buf))<0)
	{
		goto arr;
	}
	len=strlen(buf);
	buf[len]='\0';
	printf("%s\n",buf);
			
arr:
	close(fd);
	exit(0);
}
