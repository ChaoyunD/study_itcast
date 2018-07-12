#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	int fd;
	if((fd=open(argv[1],O_RDWR))<0)
	{
		perror("open:");
		exit(1);
	}
	char buf[1024];
	int len;
	len=read(fd,buf,sizeof(buf));
	if(len<0)
	{
	//	perror("read:");
	//	exit(2);
		goto arr;
	}	
	printf("%s\n",buf);
arr:	
	close(fd);
	exit(0);
}
