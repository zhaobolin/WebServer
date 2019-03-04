#include "channel.h"
#include "eventloop.h"
#include "epoll.h"
#include <queue>
#include <iostream>
#include <cstdlib>
using namespace std;


Channel::Channel(EventLoop* loop)
		:loop_(loop),
		 events_(0),
		 lastevents_(0),
{
		
}

Channel::Channel(EventLoop* loop,int fd)
		:loop_(loop),
		 fd_(fd),
		 events_(0),
		 lastevents(0)
{

}

Channel::~Channel()
{

}

void Channel::handleRead()
{
		if(readHandler_)
		{
				readHandler_()
		}
}

void Channel::handleWrite()
{
		if(writeHandler_)
		{
				writeHandler_();
		}
}
void Channel::handleConn()
{
		if(connHandler_)
		{
				connHanlder_();
		}
}
