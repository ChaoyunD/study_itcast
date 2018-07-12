#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "head.h"
int main()
{
    int a=5,b=5;
    printf("a+b=%d\n",add(a,b));
    printf("a-b=%d\n",sub(a,b));
    return 0;
}
