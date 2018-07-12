#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
int main()
{
        for(int i = 0;i < 2; i++)
        {
            fork();
            printf("i = %d\t",i);
                        
        }
           
        sleep(1);

}
