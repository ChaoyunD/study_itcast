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
int fn(int x)
{
    int f1, f2;
    if (x == 1|| x == 2)
        return 1;
    else
        f1= fn(x - 1);
    f2= fn(x - 2);
    return f1+f2;

}
void main()
{
    int y = fn(6);
    printf("%d ", y);
}

