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
    while(1)
    {
        int a;
        int b;
        //设置取消点
        //pthread_testcancel();
        sleep(1);
    }
}
int main()
{
    int ret;
    int i=0;
    pthread_t thread;
    ret=pthread_create(&thread,NULL,mythread,NULL);
    if(ret!=0)
    {
        printf("pthread_join error,[%s]\n",strerror(ret));
    }
    //取消子线程
    pthread_cancel(thread);
    //等待子线程结束
    pthread_join(thread,NULL);
    return 0;
}
