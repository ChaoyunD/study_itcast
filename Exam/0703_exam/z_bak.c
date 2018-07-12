#include <stdio.h>
#include <pthread.h>
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
#include <time.h>
int main(void)
{
    unsigned int x = 3, y = -1, z = 2;
    if (x > y)
        if (y < 0)
            z = 0;
        else
            z += 1;
    printf("%d\n", z);
    return 0;
}


