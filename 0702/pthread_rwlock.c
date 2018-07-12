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
//3个子线程写共享资源，5个子线程读共享资源
pthread_rwlock_t lock;
void *thread_write(void *arg)
{
    int n=*(int *)arg;
    int cur;
    while(1)
    {
        pthread_rwlock_wrlock(&lock);
        cur=number;
        cur++;
        number=cur;
        printf("W:[%d]-->[%d]\n",n,number);
        pthread_rwlock_unlock(&lock);
        usleep(200);
    }
}
void *thread_read(void *arg)
{
    int n=*(int *)arg;
    while(1)
    {
        pthread_rwlock_rdlock(&lock);
        printf("R:[%d]-->[%d]\n",n,number);
        pthread_rwlock_unlock(&lock);
        usleep(100);
    }
}
int main(int argc,char *argv[])
{
    int i;
    int ret;
    int arr[8];
    pthread_t thread[8];
    pthread_rwlock_init(&lock,NULL);
    //创建3个写线程
    for(i=0;i<3;i++)
    {
        arr[i]=i;
        ret=pthread_create(&thread[i],NULL,thread_write,(void *)&arr[i]);
        if(ret!=0)
        {
            printf("create thread error:[%s]\n",strerror(ret));
            exit(-1);
        }
    }
    //创建5个读线程
    for(i=3;i<8;i++)
    {
        arr[i]=i;
        ret=pthread_create(&thread[i],NULL,thread_read,(void *)&arr[i]);
        if(ret!=0)
        {
            printf("create thread error:[%s]\n",strerror(ret));
            exit(-1);
        }
    }
    for(i=0;i<8;i++)
    {
        pthread_join(thread[i],NULL);
    }
    pthread_rwlock_destroy(&lock);
    return 0;
}
