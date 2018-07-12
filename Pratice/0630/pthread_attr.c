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
void *mythread(void *arg)
{
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    int ret;
    pthread_attr_t arr;
    if((ret=pthread_attr_init(&arr))!=0)
    {
        printf("pthread_attr_init error:[%s]\n",strerror(ret));
        return -1;
    }
    if((ret=pthread_attr_setdetachstate(&arr,PTHREAD_CREATE_DETACHED))!=0)
    {
        printf("pthread_attr_setdetachstate error:[%s]\n",strerror(ret));
        return -1;
    }
    pthread_t thread;
    if((ret=pthread_create(&thread,&arr,mythread,NULL))!=0)
    {
        printf("pthread_create error:[%s]\n",strerror(ret));
        return -1;
    }
    if((ret=pthread_join(thread,NULL))!=0)
    {
        printf("pthread_join error:[%s]\n",strerror(ret));
        return -1;
    }
    if((ret=pthread_attr_destroy(&arr))!=0)
    {
        printf("pthread_attr_destroy error:[%s]\n",strerror(ret));
        return -1;
    }
    return 0;
}
