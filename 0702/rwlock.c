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
int number=0;
pthread_rwlock_t lock;
void *thread_write(void *arg)
{
    while(1)
    {
        pthread_rwlock_wrlock(&lock);
        printf("W-->[%d],[%d]\n",*(int *)arg,number++);
        pthread_rwlock_unlock(&lock);
        usleep(50);
    }
}
void *thread_read(void *arg)
{
    while(1)
    {
        pthread_rwlock_rdlock(&lock);
        printf("R-->[%d],[%d]\n",*(int *)arg,number);
        pthread_rwlock_unlock(&lock);
        usleep(50);
    }
}
int main(int argc,char *argv[])
{
    int i;
    int arr[8];
    int ret;
    pthread_rwlock_init(&lock,NULL);
    pthread_t p[8];
    for(i=0;i<3;i++)
    {
        arr[i]=i;
        if((ret=pthread_create(&p[i],NULL,thread_write,(void *)&arr[i]))!=0)
        {
            printf("pthread_create error:[%s]\n",strerror(ret));
            return -1;
        }
    }
    for(i=3;i<8;i++)
    {
        arr[i]=i;
        if((ret=pthread_create(&p[i],NULL,thread_read,(void *)&arr[i]))!=0)
        {
            printf("pthread_create error:[%s]\n",strerror(ret));
            return -1;
        }
    }
    for(i=0;i<8;i++)
    {
        pthread_join(p[i],NULL);
    }
    pthread_rwlock_destroy(&lock);
    return 0;
}
