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
int gvar=99;
void *mythread(void *arg)
{
    printf("[%d]:[%ld]\n",*(int *)arg,pthread_self());
    //线程退出
    //sleep(100);
    pthread_exit(&gvar);
}
int main()
{
    int ret;
    int i=0;
    int arr[5];
    pthread_t thread[5];
    for(i=0;i<5;i++)
    {
        arr[i]=i;
        ret=pthread_create(&thread[i],NULL,mythread,(void *)&arr[i]);
    }
    void *ptr=NULL;
    for(int i=0;i<5;i++)
        //pthread_exit(NULL);
        pthread_join(thread[i],&ptr);
    printf("[%p][%p],[%d]\n",ptr,&gvar,*(int *)ptr);
    return 0;
}
