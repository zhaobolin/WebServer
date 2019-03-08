#pragma once
#include "channel.h"
#include <vector>
#include <sys/epoll.h>
#include "timer.h"
class Epoll
{
		public:
				Epoll();
				~Epoll();
				//添加/修改/删除请求
				void epoll_add(SP_Channel request,int timeout);
				void epoll_mod(SP_Channel request,int timeout);
				void epoll_del(SP_Channel request);

				std::vector<std::shared_ptr<Channel>> poll();

				std::vector<std::shared_ptr<Channel>> getEventsRequest(int events_num);//根据数量遍历event_返回要操作的Channel指针的vector结构
				void add_timer(std::shared_ptr<Channel> request_data,int timeout);//添加定时器事件
				int getEpollFd()
				{
						return epollfd_;
				}
				void handleExpired();//处理到期事件
		private:
				static const int MAXFDS=100000;//最大连接数
				int epollfd_;
				std::vector<epoll_event> events_;//存放epoll_wait的就绪事件
				std::shared_ptr<Channel> fd2chan_[MAXFDS];//使用fd作为下标存放Channel
				//std::shared_ptr<HttpData> fd2http_[MAXFDS];//使用fd作为下标存放httpdata
				TimerManager timerManager_;
};
