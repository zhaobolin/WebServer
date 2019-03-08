#include "epoll.h"
#include <sys/epoll.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <deque>
#include <queue>
#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

const int EVENTSNUM=4096;
const int EPOLLWAIT_TIME=10000;

typedef shared_ptr<Channel> SP_Channel;

Epoll::Epoll()
		:epollfd_(epoll_create1(EPOLL_CLOEXEC)),
		events_(EVENTSNUM)
{
		assert(epollfd_>0);
}

Epoll::~Epoll()
{

}

void Epoll::epoll_add(SP_Channel request,int timeout)
{
		int fd=request->getFd();
		//printf("epoll注册文件描述符%d\n",fd);
		if(timeout>0)
		{
			add_timer(request,timeout);
			printf("addtimer\n");
			//	fd2http_[fd] =request->getHolder();
		}
		struct epoll_event event;
		event.data.fd=fd;//文件描述符
		event.events=request->getEvents();//获取事件类型
		//printf("监听事件类型%d\n",event.events);
		request->EqualAndUpdateLastEvents();//判断事件是否与上次一致/更新最后的事件

		fd2chan_[fd]=request;
		if(epoll_ctl(epollfd_,EPOLL_CTL_ADD,fd,&event)<0)
		{
				printf("epoll add error\n");
				fd2chan_[fd].reset();//添加失败则释放Channel指针
		}
}

void Epoll::epoll_mod(SP_Channel request, int timeout)
{
	int fd=request->getFd();
	if(timeout>0)
		add_timer(request,timeout);
	if(!request->EqualAndUpdateLastEvents())
	{
		struct epoll_event event;
		event.data.fd=fd;
		event.events=request->getEvents();
		if(epoll_ctl(epollfd_,EPOLL_CTL_MOD,fd,&event))
		{
				fd2chan_[fd].reset();
		}
	}
}

void Epoll::epoll_del(SP_Channel request)
{
		int fd=request->getFd();
		struct epoll_event event;
		event.data.fd=fd;
		event.events=request->getEvents();
		if(epoll_ctl(epollfd_,EPOLL_CTL_DEL,fd,&event))
		{
				printf("epoll delete error\n");
		}
		fd2chan_[fd].reset();//释放channel
		//fd2http[fd].reset();//释放channel对应的数据
}

std::vector<SP_Channel> Epoll::poll()
{
		while(true)
		{
				int event_count=epoll_wait(epollfd_,&*events_.begin(),events_.size(),EPOLLWAIT_TIME);
				//printf("epoll_wait返回%d\n",event_count);
				if(event_count<0)
				{
						printf("epoll error");
				}
				std::vector<SP_Channel> req_data=getEventsRequest(event_count);//分发处理事件
				if(req_data.size()>0)
				{
						return req_data;
				}
		}
}

void Epoll::handleExpired()//处理到期事件
{
		timerManager_.getExpried();
}

std::vector<SP_Channel> Epoll::getEventsRequest(int eventnum)
{
		std::vector<SP_Channel> req_data;
		for(int i=0;i<eventnum;i++)
		{
				int fd=events_[i].data.fd;//获取文件描述符
				SP_Channel cur_req=fd2chan_[fd];//获取文件描述符对应的Channel指针
				if(cur_req)
				{
						cur_req->setRevents(events_[i].events);//revents为目前活动的事件
						cur_req->setEvents(0);//关心的事件
						req_data.push_back(cur_req);
				}
				else
				{
						printf("cur_req is invalid");
				}
		}
		return req_data;
}

void Epoll::add_timer(SP_Channel request_data,int timeout)
{
		/*
		TimerCallback t=request_data->handleEvents;
		if(t)
		{
				timerManager_.addTimer(t,timeout,0);
		}
		else
				printf("timer add error");
				*/
}


















































