#pragma once
#include <pthread.h>
#include "mutex.h"
class Condition:zbase::noncopyable
{
		public:
				explicit Condition(MutexLock& mutex)
				:mutex_(mutex)
				{
						pthread_cond_init(&cond_,NULL)
				}
				~Condition()
				{
						pthread_cond_destroy(&cond_);
				}
				void wait()
				{
						pthread_cond_wait(%cond_,mutex_getPthreadMutex());
						//这个函数检测到条件并未就绪,就将调用线程加入等待队列中,然后将mutex释放
						//即:mutex是为了保证同时只有一个线程正在加入等待队列中
				}
				void notify()
				{
						pthread_cond_signal(&cond_);
				}
				void notifyAll()
				{
						pthread_cond_broadcast(&cond_);
				}
		private:
				MutexLock& mutex_;
				pthread_cond_t cond_;

};
