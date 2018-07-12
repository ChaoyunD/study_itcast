#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/in.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

const char MESSAGE[]="HELLO WORLD!\n";
const int PORT=8888;

void listener_cb(struct evconnlistener *,evutil_socket_t,struct sockaddr*,int socklen,void *);
void conn_writecb(struct bufferevent *,void *);
void conn_eventcb(struct bufferevent *,short,void *);
void signal_cb(evutil_socket_t ,short,void *);

int main(int argc,char *argv[])
{
    struct event_base *base;//根节点
    struct evconnlistener *listener;//链接侦听器
    struct event *signal_event;//信号事件

    struct sockaddr_in serv;
    base=event_base_new();//创建根节点
    if(!base)
    {
        fprintf(stderr,"Could not initialize libevent!\n");
        return 1;
    }
    memset(&serv,0x00,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    serv.sin_port=htons(PORT);
    //创建链接侦听器 socket bind listen accept 得到新连接将新描述符传递给回调
    listener=evconnlistener_new_bind(base,listener_cb,(void *)base,        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,(struct sockaddr*)&serv,sizeof(serv));
    if(!listener)
    {
        fprintf(stderr,"Could not create a listener!\n");
        return 1;
    }
    //创建一个信号事件
    signal_event=evsignal_new(base,SIGINT,signal_cb,(void *)base);
    if(!signal_event||event_add(signal_event,NULL)<0)
    {
        fprintf(stderr,"Could not create/add a signal event!\n");
        return 1;
    }
    //事件循环
    event_base_dispatch(base);
    //扫尾工作
    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    printf("done!\n");
    return 0;
}
void listener_cb(struct evconnlistener *listener,evutil_socket_t fd,struct sockaddr* sa,int socklen,void *user_data)
{
    struct event_base *base=user_data;
    struct bufferevent *bev;
    //创建一个bufferevent
    bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    if(!bev)
    {
        fprintf(stderr,"Error constructing bufferevent!");
        event_base_loopbreak(base);
    }
    //设置回调
    bufferevent_setcb(bev,NULL,conn_writecb,conn_eventcb,NULL);
    //使写生效
    bufferevent_enable(bev,EV_WRITE);
    //使读生效
    bufferevent_disable(bev,EV_READ);
    //向bufferevent的写缓冲区写数据
    bufferevent_write(bev,MESSAGE,strlen(MESSAGE));
}
void conn_writecb(struct bufferevent *bev,void *user_data)
{
    struct evbuffer *output=bufferevent_get_output(bev);
    if(evbuffer_get_length(output)==0)
    {
        printf("flushed answer\n");
        bufferevent_free(bev);//释放bufferevent会自动关闭socket
    }
}
void conn_eventcb(struct bufferevent *bev,short events,void *user_data)
{
    //BEV_EVENT_EOF 对端关闭
    if(events & BEV_EVENT_EOF)
        printf("Connection closed.\n");
    else if(events & BEV_EVENT_ERROR)
    {
        printf("Got an error on the connection:%s\n",strerror(errno));
    }
    bufferevent_free(bev);
}
void signal_cb(evutil_socket_t sig,short events,void *user_data)
{
    struct event_base *base=user_data;
    struct timeval delay={2,0};
    printf("Caught an interrupt signal;exiting cleanly in two seconds.\n");
    event_base_loopexit(base,&delay);
}
