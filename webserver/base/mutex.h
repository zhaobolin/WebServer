#pragma once
#include <pthread.h>
#include <assert.h>
#include "noncopyable.h"
#include "currentthread.h"
//#include "condition.h"
class MutexLock:noncopyable
{
		public:
				MutexLock()
						:holder_(0)
				{
						pthread_mutex_init(&mutex_,NULL);
				}

				~MutexLock()
				{
						//pthread_mutex_lock(&mutex);
						assert(holder_==0);
						pthread_mutex_destroy(&mutex_);
				}

				bool isLockedByThisThread()
				{
						return(holder_==currentthread::tid());
				}

				void lock()
				{
						pthread_mutex_lock(&mutex_);
						holder_=currentthread::tid();
				}

				void unLock()
				{
						holder_=0;
						pthread_mutex_unlock(&mutex_);
				}

				pthread_mutex_t* getPthreadMutex()
				{
						return &mutex_;
				}

		private:
				pthread_mutex_t mutex_;
				pid_t holder_;
		private:
				//友元类不受访问权限影响
				friend class Condition;
};

class MutexLockGuard:noncopyable
{
		public:
				explicit MutexLockGuard(MutexLock &mutex)
						:mutex_(mutex)
				{
						mutex_.lock();
				}
				~MutexLockGuard()
				{
						mutex_.unLock();
				}
		private:
				MutexLock& mutex_;
};














