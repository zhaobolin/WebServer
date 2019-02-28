#include <stdio.h>
#include <poll.h>
#include <pthread.h>
#include <assert.h>
#include "base/thread.h"
#include "base/noncopyable.h"
#include "base/currentthread.h"
class EventLoop;
__thread EventLoop* t_loopInThisThread=0;//指向当前进程的loop的指针

class EventLoop:noncopyable
{
		public:
				EventLoop()
				:looping_(false),
				 threadId_(currentthread::tid())
				{
						if(t_loopInThisThread)
						{
								printf("当前线程已有循环\n");
						}
						else
						{
								t_loopInThisThread=this;
						}
				}

				void loop()
				{
						assert(!looping_);
						if (!isInLoopingThread())
						{
								printf("当前线程并不是loop被创建的线程\n");
						}
						looping_=true;
						//循环
						printf("loop开始:tid=%d\n",threadId_);
						::poll(NULL,0,5*1000);
						printf("loop结束:tid=%d\n",threadId_);
						looping_=false;
				}

				~EventLoop()
				{
						assert(!looping_);
						t_loopInThisThread=0;
				}

				bool isInLoopingThread()//判断当前循环的线程
				{
						return threadId_==currentthread::tid();
				}

		private:
				bool looping_;
				pid_t threadId_;
};


