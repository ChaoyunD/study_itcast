#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <event.h>
struct event *readEv;
void readcb(evutil_socket_t fd,short event,void *arg)
{
    int n;
    char buf[1024];
    memset(buf,0x00,sizeof(buf));
    n=read(fd,buf,sizeof(buf));
    if(n<=0)
    {
        printf("client close or err,n=[%d]\n",n);
        event_del(readEv);
        close(fd);
    }
    else
    {
        for(int i=0;i<n;i++)
            buf[i]=toupper(buf[i]);
        write(fd,buf,n);
    }
}
void conncb(evutil_socket_t fd,short events,void *arg)
{
    struct event_base *base=(struct event_base*)arg;
    //获得新连接
    int cfd=accept(fd,NULL,NULL);
    //将新连接上树
    readEv=event_new(base,cfd,EV_READ|EV_PERSIST,readcb,base);
    event_add(readEv,NULL);
}
int main(int argc,char *argv[])
{
    //1.搭建服务器固定3步
    //1.1创建套接字
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    //1.2绑定
    struct sockaddr_in serv;
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    serv.sin_port=htons(8888);
    serv.sin_family=AF_INET;
    bind(lfd,(struct sockaddr*)&serv,sizeof(serv));
    //1.3监听
    listen(lfd,128);
    //2.创建根节点
    struct event_base *base=event_base_new();
    //3.设置lfd的监听事件,并且指定回调
    struct event *connEv=event_new(base,lfd,EV_READ|EV_PERSIST,conncb,base);
    //4.将监听事件开始监听---上树
    event_add(connEv,NULL);
    //5.事件循环while
    event_base_dispatch(base);
    //6.扫尾工作
    event_base_free(base);
    return 0;
}
