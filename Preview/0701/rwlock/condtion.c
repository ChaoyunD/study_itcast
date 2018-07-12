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
//创建节点结构
typedef struct node
{
    int data;
    struct node *next;
}Node;
//永远指向链表头部的指针
Node *head=NULL;

//线程同步需要互斥锁
pthread_mutex_t mutex;
//条件变量 阻塞线程
pthread_cond_t cond;
void *producer(void *arg)
{
    while(1)
    {
        //创建一个链表的节点
        Node *pnew=(Node *)malloc(sizeof(Node));
        //节点的初始化
        pnew->data=rand()%1000;
        //指针域
        pthread_mutex_lock(&mutex);//开始加锁
        pnew->next=head;
        head=pnew;
        printf("======producer:%lu,%d\n",pthread_self(),pnew->data);
        pthread_mutex_unlock(&mutex);
        //通知阻塞的消费者线程，解除阻塞
        pthread_cond_signal(&cond);
        sleep(rand()%3);
    }
    return NULL;
}
void *customer(void *arg)
{
    while(1)
    {
        //判断链表是否为空
        pthread_mutex_lock(&mutex);
        if(head==NULL)
        {
            //线程阻塞
            //该函数对互斥锁解锁
            pthread_cond_wait(&cond,&mutex);
            //解除阻塞之后，对互斥锁做加锁操作
        }
        //链表不为空i,删除头结点
        Node *pdel=head;
        head=head->next;
        printf("-----customer:%lu,%d\n",pthread_self(),pdel->data);
        free(pdel);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(void)
{
    pthread_t p1,p2;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    //创建生产者线程
    pthread_create(&p1,NULL,producer,NULL);
    //创建消费者线程
    pthread_create(&p2,NULL,customer,NULL);

    //阻塞回收
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    return 0;
}
