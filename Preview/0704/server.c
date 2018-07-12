#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
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
#define SERV_PORT 6666
#define SERV_IP "127.0.0.1"
int main(void)
{
    int cfd,lfd;//客户端文件描述符和服务器文件描述符
    char buf[BUFSIZ];//写文件缓冲区
    int n;
    socklen_t clie_addr_len;//
    struct sockaddr_in serv_addr,clie_addr;//服务器ip和客户端ip
    lfd=socket(AF_INET,SOCK_STREAM,0);//打开网络通讯端口，ipv4版本，流模式，使用默认协议。
    serv_addr.sin_family=AF_INET;//地址结构类型
    serv_addr.sin_port=htons(SERV_PORT);//端口号编程网络字节序列
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    /*网络地址为INADDR_ANY，
    这个宏表示本地的任意IP地址，因为服务器可能有多个网卡，
    每个网卡也可能绑定多个IP地址，这样设置可以在所有的IP地址上监听，
    直到与某个客户端建立了连接时才确定下来到底用哪个IP地址*/
    bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));//绑定一个固定的网络地址和端口号
    listen(lfd,128);//在建立连接的过程中,允许的最大客户端连接书，默认最大为128个
    clie_addr_len=sizeof(clie_addr);//传出、传入参数，为真正接收到的地址结构体的大小。
    cfd=accept(lfd,(struct sockaddr *)&clie_addr,&clie_addr_len);//接受连接，返回值为客户端的文件描述符。
    printf("client port:%d\n",ntohs(clie_addr.sin_port));
    while(1)
    {
        n=read(cfd,buf,sizeof(buf));
        for(int i=0;i<n;i++)
        {
            buf[i]=toupper(buf[i]);
        }
        write(cfd,buf,n);
    }
    close(lfd);
    close(cfd);
    return 0;
}
