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
pthread_mutex_t mutex;
void *mythread1(void *arg)
{
    int cur;
    int n=*(int *)arg;
    for(int i=0;i<100000;i++)
    {
        cur=number;
        cur++;
        pthread_mutex_lock(&mutex);
        number=cur;
        pthread_mutex_unlock(&mutex);
        printf("[%d]:[%d]\n",n,number);
        usleep(50);
    }
}
void *mythread2(void *arg)
{
    int cur;
    int n=*(int *)arg;
    for(int i=0;i<100000;i++)
    {
        cur=number;
        cur++;
        pthread_mutex_lock(&mutex);
        number=cur;
        pthread_mutex_unlock(&mutex);
        printf("[%d]:[%d]\n",n,number);
        usleep(50);
    }
}
int main(int argc,char *argv[])
{
    int ret;
    int i=1,j=2;
    pthread_mutex_init(&mutex,NULL);
    
    pthread_t thread[2];
    if((ret=pthread_create(&thread[0],NULL,mythread1,(void *)&i))!=0||(ret=pthread_create(&thread[1],NULL,mythread2,(void *)&j))!=0)
    {
        printf("pthread_create error:[%s]\n",strerror(ret));
        return -1;
    }
    pthread_detach(thread[0]);
    pthread_detach(thread[1]);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
    return 0;
}
