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
#include <strings.h>
#include <poll.h>
#include <sys/epoll.h>
#include <event.h>
#include <sys/un.h>
int main(int argc,char *argv[])
{
    //获取libevent后端支持的方法
    char **methods=event_get_supported_methods();
    int i=0;
    for(i=0;methods[i]!=NULL;i++)
    {
        printf("%s\n",methods[i]);
    }
    return 0;
}
