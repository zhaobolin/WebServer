#include "thread.h"
#include "eventloop.h"
#include <functional>
#include <stdio.h>
#include <string.h>
#include <sys/timerfd.h>
#include <memory>
/*void threadFunc()
{
		printf("threadFunc\n");
		printf("线程tid的值为:%d\n线程名为:%s\n",currentthread::tid(),currentthread::name());
		EventLoop loop;
		loop.loop();
}
*/

EventLoop* g_loop;

void timeout()
{
		printf("timeout\n");
		g_loop->quit();
}

int main()
{
	EventLoop loop;
	g_loop=&loop;

	int timefd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
  //printf("%d\n",timefd);
	std::shared_ptr<Channel> channel(new Channel(&loop,timefd));
	//printf("main_channelsetreadhandler\n");
	channel.get()->setReadHandler(timeout);
  channel.get()->setEvents(EPOLLIN);
	struct itimerspec howlong;
	bzero(&howlong,sizeof howlong);
	howlong.it_value.tv_sec=3;
	int d=timerfd_settime(timefd,0/*TFD_TIMER_ABSTIME*/,&howlong,NULL);
	//printf("函数返回值%d\n",d);
	loop.addToPoller(channel,0);
	loop.loop();
	::close(timefd);
	/*printf("主线程tid的值为:%d\n线程名为:%s\n",currentthread::tid(),currentthread::name());
	Thread* thread=new Thread(&threadFunc,"threadFunc");
	thread->start();
	EventLoop loop;
	loop.loop();
	*/
}
