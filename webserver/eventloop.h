#pragma once
#include <stdio.h>
#include "epoll.h"
#include <pthread.h>
#include <assert.h>
#include <memory>
#include "mutex.h"
#include "thread.h"
#include "noncopyable.h"
#include "currentthread.h"
#include "channel.h"
#include "epoll.h"
#include <assert.h>

class EventLoop;
//__thread EventLoop* t_loopInThisThread=0;//指向当前进程的loop的指针

class EventLoop:noncopyable
{
		public:
				typedef std::function<void()> Functor;
				EventLoop();
				~EventLoop();
				void loop();
			/*	{
						assert(!looping_);
						if (!isInLoopingThread())
						{
								printf("当前线程并不是loop被创建的线程\n");
						}
						looping_=true;
						quit_=false;
						//循环
						printf("loop开始:tid=%d\n",threadId_);
						//::poll(NULL,0,5*1000);
						while(!quit_)
						{
								pwakeupChannel.clear();
								poller=poll(,&pwakeupChannel_)
						}
						printf("loop结束:tid=%d\n",threadId_);
						looping_=false;
				}
		*/
				bool isInLoopThread()//判断当前循环的线程
				{
						return threadId_==currentthread::tid();
				}
				void runInLoop(Functor&& cb);
				void queueInLoop(Functor&&  cb);
				void assertInLoopThread()
				{
						assert(isInLoopThread());
				}
				void shutdown(std::shared_ptr<Channel> channel)
				{
						poller_->epoll_del(channel);
				}
				void updatePoller(std::shared_ptr<Channel> channel,int timeout=0)
				{
						poller_->epoll_mod(channel,timeout);
				}
				void addToPoller(std::shared_ptr<Channel> channel,int timeout=0)
				{
						poller_->epoll_add(channel,timeout);
				}
				void quit();
		private:
				bool looping_;
				pid_t threadId_;
				//新加入内容
				std::shared_ptr<Epoll> poller_;//持有指向poller的指针
				int wakeupFd_;//用于唤醒epoll来执行用户回调函数的文件描述符
				bool quit_;
				bool eventHandling_;//loop中是否正在处理就绪事件
				mutable MutexLock mutex_;
				std::vector<Functor> pendingFunctors_;//待执行的用户回调函数
				bool callingPendingFunctors_;
				std::shared_ptr<Channel> pwakeupChannel_;//活跃的channel成员

				void wakeup();
				void handleRead();
				void doPendingFunctors();
				void handleConn();
};


