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
int number=0;
pthread_rwlock_t lock;
void *write_lock(void *arg)
{
    while(1)
    {
        pthread_rwlock_wrlock(&lock);
        number++;
        printf("write=[%ld]:[%d]\n",pthread_self(),number);
        pthread_rwlock_unlock(&lock);
        usleep(500);
    }
    pthread_exit(NULL);
}
void *read_lock(void *arg)
{
    while(1)
    {
        pthread_rwlock_rdlock(&lock);
        printf("read=[%ld]:[%d]\n",pthread_self(),number);
        pthread_rwlock_unlock(&lock);
        usleep(500);
    }
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    pthread_t thread[8];
    //初始化读写锁
    pthread_rwlock_init(&lock,NULL);
    int i;
    for(i=0;i<3;i++)
    {
        pthread_create(&thread[i],NULL,write_lock,NULL);
    }
    for(i=3;i<8;i++)
    {
        pthread_create(&thread[i],NULL,read_lock,NULL);
    }
    for(i=0;i<8;i++)
    {
        pthread_join(thread[i],NULL);
    }
    pthread_rwlock_destroy(&lock);
    pthread_exit(NULL);
    return 0;
}
