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
pthread_mutex_t chopstick[6] ;
void *eat_think(void *arg)
{
    char phi = *(char *)arg;
    int left,right; //左右筷子的编号
    switch (phi){
    case 'A':
        left = 5;
        right = 1;
        break;
    case 'B':
        left = 1;
        right = 2;
        break;
    case 'C':
        left = 2;
        right = 3;
        break;
    case 'D':
        left = 3;
        right = 4;
        break;
    case 'E':
        left = 4;
        right = 5;
        break;

    }

    int i;
    for(;;){
        usleep(500); //思考
        pthread_mutex_lock(&chopstick[left]); //拿起左手的筷子
        printf("Philosopher %c fetches chopstick %d\n", phi, left);
        if(!pthread_mutex_trylock(&chopstick[right]))
        { //拿起右手的筷子    
            printf("Philosopher %c release chopstick %d\n", phi, left);
            pthread_mutex_unlock(&chopstick[left]); //如果右边筷子被拿走放下左手的筷子
            continue;
        }

        //pthread_mutex_lock(&chopstick[right]); //拿起右手的筷子，如果想观察死锁，把上一句if注释掉，再把这一句的注释去掉
        printf("Philosopher %c fetches chopstick %d\n", phi, right);
        printf("Philosopher %c is eating.\n",phi);
        usleep(500); //吃饭
        printf("Philosopher %c release chopstick %d\n", phi, left);
        pthread_mutex_unlock(&chopstick[left]); //放下左手的筷子
        printf("Philosopher %c release chopstick %d\n", phi, right);
        pthread_mutex_unlock(&chopstick[right]); //放下左手的筷子
        sleep(3);

    }

}
int main(){
    pthread_t A,B,C,D,E; //5个哲学家

    int i;
    for (i = 1; i < 6; i++)
        pthread_mutex_init(&chopstick[i],NULL);
    pthread_create(&A,NULL, eat_think, "A");
    pthread_create(&B,NULL, eat_think, "B");
    pthread_create(&C,NULL, eat_think, "C");
    pthread_create(&D,NULL, eat_think, "D");
    pthread_create(&E,NULL, eat_think, "E");

    pthread_join(A,NULL);
    pthread_join(B,NULL);
    pthread_join(C,NULL);
    pthread_join(D,NULL);
    pthread_join(E,NULL);
    return 0;

}
