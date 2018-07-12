//取消子线程
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void *mythread(void *arg)
{
	while(1)
	{
		int a;
		int b;

		//设置取消点
		//pthread_testcancel();
		sleep(100);
		
	}
}

int main()
{
	int ret;

	pthread_t thread;
	
	ret = pthread_create(&thread, NULL, mythread, NULL);
	if(ret!=0)
	{
		printf("pthread_create error, [%s]\n", strerror(ret));
	}
	
	//取消子线程
	pthread_cancel(thread);

	//等待回收
	pthread_join(thread, NULL);

	return 0;
}
