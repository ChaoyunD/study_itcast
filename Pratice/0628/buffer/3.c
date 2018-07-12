#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
int main()
{
    fork();
    printf("pid = %d, ppid = %d",getpid(),getppid());
    fork();
    sleep(1);
}
