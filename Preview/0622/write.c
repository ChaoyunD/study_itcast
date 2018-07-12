#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	int fd;
	if((fd=open(argv[1],O_RDWR|O_CREAT|O_APPEND,0644))<0)
	{
		perror("open:");
		exit(1);
	}
	char buf[1024]="aaaaaaaaaaaaaaaa\n";
	int len;
	if(len=write(fd,buf,strlen(buf))<0)
	{
		goto arr;
	}
		
arr:
	close(fd);
	exit(0);
}
