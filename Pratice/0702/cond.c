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
        printf("producer:[%d]\n",head->data);
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
        if(head==NULL)
        {
            pthread_mutex_unlock(&mutex);
            continue;
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
    int i;
    int arr[5];
    pthread_t pro;
    pthread_t cus[5];
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);
    ret=pthread_create(&pro,NULL,producer,NULL);
	if(ret!=0)
	{
		printf("pthread_create error[%s]\n",strerror(ret));
		return -1;
	}
    for(i=0;i<5;i++)
    {
        arr[i]=i+1;
        if((ret=pthread_create(&cus[i],NULL,customer,(void *)&arr[i]))!=0)
        {
            printf("pthread_create error:[%s]\n",strerror(ret));
            return -1;
        }
    }
    for(i=0;i<5;i++)
    {
        pthread_join(cus[i],NULL);
    }
    pthread_join(pro,NULL);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
    return 0;
}
