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
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;
void *mythread1(void *arg)
{
    for(int i=0;i<100000;i++)
    {
        pthread_mutex_lock(&mutex_a);
        printf("HELLO\n");
        pthread_mutex_lock(&mutex_b);
        //usleep(50);
        printf("WORLD\n");
        pthread_mutex_unlock(&mutex_b);
        pthread_mutex_unlock(&mutex_a);
        usleep(50);
    }
}
void *mythread2(void *arg)
{
    for(int i=0;i<100000;i++)
    {
        pthread_mutex_lock(&mutex_b);
        printf("hello\n");
        pthread_mutex_lock(&mutex_a);
        //usleep(50);
        printf("world\n");
        sleep(1);
        pthread_mutex_unlock(&mutex_a);
        pthread_mutex_unlock(&mutex_b);
        usleep(50); 
    }
}
int main(int argc,char *argv[])
{
    int ret;
    int i=1,j=2;
    pthread_mutex_init(&mutex_a,NULL);
    pthread_mutex_init(&mutex_b,NULL);
    
    pthread_t thread[2];
    if((ret=pthread_create(&thread[0],NULL,mythread1,(void *)&i))!=0||(ret=pthread_create(&thread[1],NULL,mythread2,(void *)&j))!=0)
    {
        printf("pthread_create error:[%s]\n",strerror(ret));
        return -1;
    }
    pthread_join(thread[0],NULL);
    pthread_join(thread[1],NULL);
    pthread_mutex_destroy(&mutex_a);
    pthread_mutex_destroy(&mutex_b);
    pthread_exit(NULL);
    return 0;
}
