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
        Node *pNew=(Node *)malloc(sizeof(Node));
        pNew->data=rand()%1000;
        pNew->next=head;
        head=pNew;
        printf("[%d]producer:[%d]\n",*(int *)arg,head->data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        usleep(50);
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
        printf("[%d]customer:[%d]\n",*(int *)arg,tmp->data);
        free(tmp);
        pthread_mutex_unlock(&mutex);
        usleep(200);
    }
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    int ret;
    pthread_t p[8];
    int i;
    int arr[8];
    for(i=0;i<3;i++)
    {
        arr[i]=i+1;
        if((ret=pthread_create(&p[i],NULL,producer,&arr[i]))!=0)
        {
            printf("pthread_create error:[%s]\n",strerror(ret));
            return -1;
        }
    }
    for(i=3;i<8;i++)
    {
        arr[i]=i+1;
        if((ret=pthread_create(&p[i],NULL,customer,&arr[i]))!=0)
        {
            printf("pthread_create error:[%s]\n",strerror(ret));
            return -1;
        }
    }
    for(i=0;i<8;i++)
    {
        pthread_join(p[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    return 0;
}
