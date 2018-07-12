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
#include <semaphore.h>
typedef struct node
{
    int data;
    struct node *next;
}Node;
Node *head=NULL;
pthread_mutex_t mutex;
pthread_cond_t cond;
void *producer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        Node *pnew=(Node *)malloc(sizeof(Node));
        pnew->data=rand()%1000;
        pnew->next=head;
        head=pnew;
        printf("producer[%d]:[%d]\n",*(int *)arg,head->data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        //sleep(1);
        usleep(100);
    }
    pthread_exit(NULL);
}
void *customer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(head==NULL)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        Node *tmp=head;
        head=head->next;
        printf("customer[%d]:[%d]\n",*(int *)arg,tmp->data);
        free(tmp);
        pthread_mutex_unlock(&mutex);
        //sleep(2);
        usleep(400);
    }
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    int ret;
    pthread_t thread[4];
    int i=1,j=2;
    int q=1,p=2;
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);
    ret=pthread_create(&thread[0],NULL,producer,(void *)&q);
	if(ret!=0)
	{
		printf("pthread_create error[%s]\n",strerror(ret));
		return -1;
	}
    ret=pthread_create(&thread[3],NULL,producer,(void *)&p);
	if(ret!=0)
	{
		printf("pthread_create error[%s]\n",strerror(ret));
		return -1;
	}
//    sleep(2);
    ret=pthread_create(&thread[1],NULL,customer,(void *)&i);
	if(ret!=0)
	{
		printf("pthread_create error[%s]\n",strerror(ret));
		return -1;
	}
    ret=pthread_create(&thread[2],NULL,customer,(void *)&j);
	if(ret!=0)
	{
		printf("pthread_create error[%s]\n",strerror(ret));
		return -1;
	}
    pthread_join(thread[0],NULL);
    pthread_join(thread[1],NULL);
    pthread_join(thread[2],NULL);
    pthread_join(thread[3],NULL);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
    return 0;
}
