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
sem_t pro;
sem_t cus;
void *producer(void *arg)
{
    while(1)
    {
        Node *pNew=(Node *)malloc(sizeof(Node));
        pNew->data=rand()%1000;
        sem_wait(&pro);
        pNew->next=head;
        head=pNew;
        printf("producer[%d]:[%d]\n",*(int *)arg,head->data);
        sem_post(&cus);
        usleep(50);
    }
    pthread_exit(NULL);
}
void *customer(void *arg)
{
    while(1)
    {
        sem_wait(&cus);
        Node *tmp=head;
        printf("customer[%d]:[%d]\n",*(int *)arg,tmp->data);
        head=head->next;
        free(tmp);
        sem_post(&pro);
        usleep(200);
    }
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    sem_init(&pro,0,1);
    sem_init(&cus,0,0);
    int i;
    pthread_t p[6];
    pthread_t p2;
    int m=1,n=2;
    int ret;
    if((ret=pthread_create(&p[0],NULL,producer,(void *)&m))!=0)
    {
        printf("pthread_create error:[%s]\n",strerror(ret));
        return -1;
    }
    if((ret=pthread_create(&p2,NULL,producer,(void *)&n))!=0)
    {
        printf("pthread_create error:[%s]\n",strerror(ret));
        return -1;
    }
    int arr[6];
    for(i=1;i<6;i++)
    {
        arr[i]=i;

        if((ret=pthread_create(&p[i],NULL,customer,(void *)&arr[i]))!=0)
        {
            printf("pthread_create error:[%s]\n",strerror(ret));
            return -1;
        }
    }
    pthread_join(p[0],NULL);
    for(i=1;i<6;i++)
    {
        pthread_join(p[i],NULL);
    }
    sem_destroy(&pro);
    sem_destroy(&cus);
    pthread_exit(NULL);
    return 0;
}
