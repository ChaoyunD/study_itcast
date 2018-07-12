#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
union
{
    int i;
    char x[2]
}a;
int main()
{
    a.x[0]=10;
    a.x[1]=1;
    printf("%d\n",a.i);
    return 0;
}
