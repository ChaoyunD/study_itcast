//设置子线程为分离状态
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
	ret = pthread_create(&thread, NULL, mythread, (void *)&test);
	if(ret!=0)
	{
		printf("pthread_create error, [%s]\n", strerror(ret));
	}

	//设置子线程为分离状态
	pthread_detach(thread);

	ret = pthread_join(thread, NULL);
	if(ret!=0)
	{
		printf("pthread_join error, [%s]\n", strerror(ret));
	}
	
	printf("child id[%ld], main:[%ld]\n", thread, pthread_self());
	printf("main pid==[%d]\n", getpid());

	return 0;
}
