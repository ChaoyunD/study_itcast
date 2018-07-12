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
int status=10;
void *mythread(void *arg)
{
//    printf("pid=%d,pthread_id=%ld\n",getpid(),pthread_self());
    while(1)
    {
        int a;
        int b;
        pthread_testcancel();
    }
//    pthread_exit(&status);    
}
int main(void)
{
    pthread_t thread;
    int ret=pthread_create(&thread,NULL,mythread,NULL);
    if(ret!=0)
    {
        printf("error:%s\n",strerror(ret));
        return -1;
    }
    if((ret=pthread_cancel(thread))!=0)
    {
        printf("pthread_cancel error:[%s]\n",strerror(ret));
        return -1;
    }
    void *retval=NULL;
    //判断子线程退出状态（主线程阻塞在这里）
    if((ret=pthread_join(thread,&retval))<0)
    {
        printf("pthread_join error:%s\n",strerror(ret));
        return -1;
    }
    pthread_exit(NULL);
    return 0;
}
