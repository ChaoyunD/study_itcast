//两个顾客2秒钟吃一次，店主2秒钟生产一次。
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
        Node *pnew=(Node *)malloc(sizeof(Node));
        pnew->data=rand()%1000;
        pthread_mutex_lock(&mutex);
        pnew->next=head;
        head=pnew;
        printf("====:producer:%lu,%d\n",pthread_self(),pnew->data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        sleep(2);
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
            //continue;
            pthread_cond_wait(&cond,&mutex);
        }
        Node *tmp=head;
        head=head->next;
        printf("====customer %d:%lu,%d\n",*(int *)arg,pthread_self(),tmp->data);
        free(tmp);
        sleep(1);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    pthread_t p1,p2,p3;
    int i=1,j=2;
    pthread_mutex_lock(&mutex);
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_create(&p1,NULL,producer,NULL);
    pthread_create(&p2,NULL,customer,(void *)&i);
    pthread_create(&p3,NULL,customer,(void *)&j);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    return 0;
}
