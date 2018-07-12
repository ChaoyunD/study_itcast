//循环创建n个子线程
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void *mythread(void *arg)
{
	printf("[%d]:child id==[%ld]\n", *(int *)arg, pthread_self());
}

int main()
{
	int i = 0;
	int ret;
	int arr[5];
	pthread_t thread[5];

	for(i=0; i<5; i++)
	{
		arr[i] = i;
		//ret = pthread_create(&thread[i], NULL, mythread, (void *)&i);
		ret = pthread_create(&thread[i], NULL, mythread, (void *)&arr[i]);
		if(ret!=0)
		{
			printf("pthread_create error, [%s]\n", strerror(ret));
			return -1;
		}
		//sleep(1);
	}

	sleep(1);
	return 0;
}
