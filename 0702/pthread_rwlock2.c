//读写锁:3个子线程写共享资源, 5个子线程读共享资源
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int number = 0;

//创建一把读写锁
pthread_rwlock_t rwlock;

//写线程执行函数
void *thread_write(void *arg)
{
	int n = *(int *)arg;
	
	int cur;
	while(1)
	{
		//加写锁
		pthread_rwlock_wrlock(&rwlock);
		cur = number;
		cur++;
		number = cur;	
		printf("W:[%d]-->[%d]\n", n, number);
		//解写锁
		pthread_rwlock_unlock(&rwlock);
		usleep(200);
	}
}

//读线程执行函数
void *thread_read(void *arg)
{
	int n = *(int *)arg;

	while(1)
	{
		//加读锁
		pthread_rwlock_rdlock(&rwlock);
		printf("R:[%d]-->[%d]\n", n, number);
		//解读锁
		pthread_rwlock_unlock(&rwlock);
		usleep(100);
	}
}

int main()
{
	int i;
	int ret;
	int arr[8];
	pthread_t thread[8];

	//初始化读写锁
	pthread_rwlock_init(&rwlock, NULL);

	//创建3个写线程
	for(i=0; i<3; i++)
	{
		arr[i] = i;
		ret = pthread_create(&thread[i], NULL, thread_write, &arr[i]);
		if(ret!=0)
		{
			printf("create thread error,[%s]\n", strerror(ret));
			exit(-1);
		}
	}	

	//创建5个读线程
	for(i=3; i<8; i++)
	{
		arr[i] = i;
		ret = pthread_create(&thread[i], NULL, thread_read, &arr[i]);
		if(ret!=0)
		{
			printf("create thread error,[%s]\n", strerror(ret));
			exit(-1);
		}
	}	

	//等待回收子线程
	for(i=0; i<8; i++)
	{
		pthread_join(thread[i], NULL);
	}	

	//释放读写锁
	pthread_rwlock_destroy(&rwlock);

	return 0;
}
