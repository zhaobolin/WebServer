#include "base/thread.h"
#include "eventloop.h"
#include <functional>
#include <stdio.h>
#include <string>
void threadFunc()
{
		printf("threadFunc\n");
		printf("线程tid的值为:%d\n线程名为:%s\n",currentthread::tid(),currentthread::name());
		EventLoop loop;
		loop.loop();
}

int main()
{
		printf("主线程tid的值为:%d\n线程名为:%s\n",currentthread::tid(),currentthread::name());
		Thread* thread=new Thread(&threadFunc,"threadFunc");
		thread->start();
		EventLoop loop;
		loop.loop();
}
