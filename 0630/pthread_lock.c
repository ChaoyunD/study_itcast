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
//定义一把互斥锁
pthread_mutex_t mutex;
void *mythread1(void *arg)
{
    int n=*(int *)arg;
    int i=0;
    int cur;
    while(i++<100000)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        cur=number;
        cur++;
        number=cur;
        printf("[%d]:[%d]\n",n,number);
        pthread_mutex_unlock(&mutex);
    }
}
void *mythread2(void *arg)
{
    int n=*(int *)arg;
    int i=0;
    int cur;
    while(i++<100000)
    {
        pthread_mutex_lock(&mutex);
        cur=number;
        cur++;
        number=cur;
        printf("[%d]:[%d]\n",n,number);
        pthread_mutex_unlock(&mutex);
    }
}
int main()
{
    int ret;
    int i=1;
    pthread_t thread[2];

    pthread_mutex_init(&mutex,NULL);
    ret=pthread_create(&thread[0],NULL,mythread1,(void *)&i);
    int j=2;
    ret=pthread_create(&thread[1],NULL,mythread2,(void *)&j);
    pthread_mutex_destroy(&mutex);
    
    pthread_exit(NULL);
}
