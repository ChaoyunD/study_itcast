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
#include <sys/socket.h>
#include <arpa/inet.h>
union
{
    int i;
    char ch[sizeof(int)];
}un;
int main(void)
{
    un.i=0x12345678;
    printf("[%x][%x][%x][%x]\n",un.ch[0],un.ch[1],un.ch[2],un.ch[3]);
    return 0;
}
