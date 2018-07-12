//生产者和消费者模型----互斥锁+条件变量
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

typedef struct node
{
	int data;
	struct node *next;
}NODE;

NODE *head = NULL;

//定义一把锁
pthread_mutex_t mutex;

//定义条件变量
pthread_cond_t cond;

//生产者线程执行函数
void *producer(void *arg)
{
	NODE *pNode = NULL;
	while(1)
	{
		pNode = (NODE *)malloc(sizeof(NODE));
		if(pNode==NULL)
		{
			exit(-1);
		}
		pNode->data = rand()%1000;
		
		//加锁
		pthread_mutex_lock(&mutex);
		pNode->next = head;
		head = pNode;	
		printf("P:[%d]\n", head->data);
		//解锁
		pthread_mutex_unlock(&mutex);
		
		//唤醒消费者解除阻塞
		pthread_cond_signal(&cond);
		
		usleep(400);	
	}
}

//消费者线程执行函数
void *consumer(void *arg)
{
	NODE *pNode = NULL;
	while(1)
	{
		//加锁
		pthread_mutex_lock(&mutex);
		if(head==NULL)
		{
			//若条件不满足, 则阻塞等待,同时解锁
			//若条件满足(被生产者线程调用pthread_cond_signal函数唤醒), 则解除阻塞, 同时加锁
			pthread_cond_wait(&cond, &mutex);
		}
		
		printf("C:[%d]\n", head->data);
		pNode = head;
		head = head->next;
		free(pNode);
		pNode = NULL;
		//解锁
		pthread_mutex_unlock(&mutex);
		
		usleep(200);	
	}
}


int main()
{
	int ret;
	pthread_t thread1, thread2;
	
	//初始化互斥锁
	pthread_mutex_init(&mutex, NULL);
	
	//初始化条件变量
	pthread_cond_init(&cond, NULL);
	
	//创建生产者线程
	ret = pthread_create(&thread1, NULL, producer, NULL);
	if(ret!=0)
	{
		printf("create producer error, [%s]\n", strerror(ret));
		return -1;
	}
	
	sleep(1);
	//创建消费者线程
	ret = pthread_create(&thread2, NULL, consumer, NULL);
	if(ret!=0)
	{
		printf("create producer error, [%s]\n", strerror(ret));
		return -1;
	}
	
	//等待回收子线程
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	//释放互斥锁
	pthread_mutex_destroy(&mutex);
	
	//释放条件变量
	pthread_cond_destroy(&cond);
	
	return 0;
}