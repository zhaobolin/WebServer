#pragma once
#include <string>
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <queue>
#include "noncopyable.h"

class EventLoop;
class Channel:noncopyable
{
		public:
				typedef std::function <void()> Callback;
				Channel(EventLoop* loop);
				Channel(EventLoop* loop,int fd);
				~Channel();
				/*void setHolder(std::shared_ptr<HttpData>holder)
				{
						holder_=holder;
				}
				*/
				/*std::shared_ptr<HttpData> getHolder()
				{
						return holder_;
				}
				*/
				void setReadHandler(const Callback& cb)
				{
						printf("设置读处理函数\n");
						readHandler_=cb;
				}
				void setWriteHandler(const Callback& cb)
				{
						printf("设置写处理函数\n"); 
						writeHandler_=cb;
				}
				void setErrorHandler(const Callback& cb)
				{
						printf("设置错误处理函数\n"); 
						errorHandler_=cb;
				}
				void setConnHandler(const Callback& cb)
				{
						printf("设置连接处理函数\n"); 
						connHandler_=cb;
				}
				void handleEvents()
				{
						printf("调用channel的处理函数\n");
						events_=0;
						if((revents_&EPOLLHUP)&&!(revents_ & EPOLLIN))
						{
								events_=0;
								return;
						}
						if(revents_&EPOLLERR)
						{
								if(errorHandler_)
										errorHandler_();
								events_=0;
						}
						if(revents_&(EPOLLIN|EPOLLPRI|EPOLLRDHUP))
						{
								handleRead();
						}
						if(revents_&EPOLLOUT)
						{
								handleWrite();
						}
						handleConn();
				}
				void handleRead();
				void handleWrite();
				void handleError(int fd,int errornum,std::string short_msg);
				void handleConn();
				int getFd()
				{
						return fd_;
				}
				void setFd(int fd)
				{
						fd_=fd;
				}
				int getEvents()
				{
						return events_;
				}
				EventLoop* ownerLoop()
				{
						return loop_;
				}
				void setEvents(int ev)
				{
						events_=ev;
				}
				void setRevents(int ev)
				{
						revents_=ev;
				}
				bool EqualAndUpdateLastEvents()
				{
						bool ret=(lastevents_==events_);
						lastevents_=events_;
						return ret;
				}
				int getLastEvents()
				{
						return lastevents_;
				}
		private:
				 int fd_;
				int events_;
				int revents_;
				int lastevents_;
				EventLoop* loop_;

				Callback readHandler_;
				Callback writeHandler_;
				Callback errorHandler_;
				Callback connHandler_;
				
				//std::weak_ptr<HttpData> holder_;
};
typedef std::shared_ptr<Channel> SP_Channel;
