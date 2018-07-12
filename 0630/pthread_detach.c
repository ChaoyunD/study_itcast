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
    printf("[%d]:[%ld]\n",*(int *)arg,pthread_self());
}
int main()
{
    int ret;
    int i=0;
    pthread_t thread;
    ret=pthread_create(&thread,NULL,mythread,(void *)&i);
    pthread_detach(thread);
    ret=pthread_join(thread,NULL);
    if(ret!=0)
    {
        printf("pthread_join error,[%s]\n",strerror(ret));
    }
    sleep(1);
    return 0;
}
