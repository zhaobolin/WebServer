#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
ssize_t readn(int fd,void* buffer,size_t n)
{
    size_t nleft=n;//还需要的字节数
    ssize_t nread=0;//read函数读到的字节数
    char *ptr=(char*)buffer;

    while(nleft>0)
    {
        nread=read(fd,ptr,nleft);
        if(-1==nread)
        {
            if(errno==EINTR)//EINTR错误码为中断的系统调用，需要重新执行
            {
                nread=0;
            }
            else if(errno==EAGAIN)//资源临时不可用
            {
                return (n-nread);
            }
            else
            {
                return -1;
            }
        }
        else if(nread==0)
        {
            break;
        }
        nleft-=nread;
        ptr+=nread;
    }
    return n-nleft;
}
ssize_t writen(int fd,void* buffer,size_t n)
{
    int nleft=n;
    int nwrite=0;
    char* ptr=(char*)buffer;
    while(n>0)
    {
        if((nwrite=write(fd,ptr,nleft))<=0)
        {
            if(nwrite<0)
            {
                if(errno==EINTR)
                {
                    nwrite=0;
                    continue;
                }
                else if(errno==EAGAIN)
                {
                    return n-nleft;
                }
                else
                {
                    return -1;
                }
            }
        }
        nleft-=nwrite;
        ptr+=nwrite;
    }
    return n-nleft;
}
int setSocketNonBlocking(int fd)
{
    int flag=fcntl(fd,F_GETFL,0);
    if(flag==-1)
    {
        return -1;
    }

    flag|=O_NONBLOCK;
    if(fcntl(fd,F_SETFL,flag)==-1)
    {
        return -1;
    }
    return 0;
}
int socket_bind_listen(int port)
{
   // 检查port值，取正确区间范围
    if (port < 0 || port > 65535)
        return -1;

    // 创建socket(IPv4 + TCP)，返回监听描述符
    int listen_fd = 0;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;

    // 消除bind时"Address already in use"错误
    int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        return -1;

    //设置服务器IP和Port，和监听描述副绑定
    struct sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        return -1;

    // 开始监听，最大等待队列长为LISTENQ
    if(listen(listen_fd, 2048) == -1)
        return -1;

    // 无效监听描述符
    if(listen_fd == -1)
    {
        close(listen_fd);
        return -1;
    }
return listen_fd; 
}