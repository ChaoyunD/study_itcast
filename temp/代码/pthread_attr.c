//在创建子线程的时候设置线程为分离状态
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

struct TEST
{
	int data;
	char name[64];
};

void *mythread(void *arg)
{
	int var = *(int *)arg;
	printf("child thread:[%ld]\n", pthread_self());
	printf("child pid==[%d]\n", getpid());
	//printf("var==[%d]\n", var);
	printf("[%d][%s]\n", ((struct TEST *)arg)->data, ((struct TEST *)arg)->name);
	
	pthread_exit(NULL);
}

int main()
{
	int ret;
	int var = 99;
	struct TEST test;
	memset(&test, 0x00, sizeof(test));
	test.data = 9;
	strcpy(test.name, "hello world");

	pthread_t thread;
	
	//ret = pthread_create(&thread, NULL, mythread, NULL);
	//ret = pthread_create(&thread, NULL, mythread, (void *)&var);
	
	//定义线程属性变量
	pthread_attr_t attr;
	
	//初始化线程属性变量
	pthread_attr_init(&attr);
	
	//设置线程属性为分离状态
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	ret = pthread_create(&thread, &attr, mythread, (void *)&test);
	if(ret!=0)
	{
		printf("pthread_create error, [%s]\n", strerror(ret));
	}
	
	printf("child id[%ld], main:[%ld]\n", thread, pthread_self());
	printf("main pid==[%d]\n", getpid());

	ret = pthread_join(thread, NULL);
	if(ret!=0)
	{
		printf("pthread_join error:[%s]\n", strerror(ret));
	}
	
	//释放线程属性资源
	pthread_attr_destroy(&attr);
	
	pthread_exit(NULL);
}
