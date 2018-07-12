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
void *handler(void *i)
{
    if((*(int *)i)==3)
        pthread_exit(NULL);
    printf("%d\n",*(int *)i);
}
int main(int argc,char *argv[])
{
    int arr[5]={1,2,3,4,5};
    pthread_t pth[5];
    //pthread_create(&pth,NULL,handler,(int *)5);
    for(int i=0;i<5;i++)
    {
        pthread_create(&pth[i],NULL,handler,(int *)&arr[i]);
    }
    sleep(6);
    return 0;
}
