#pragma once
#include <pthread.h>
#include "noncopyable.h"

class MutexLock:zbase::noncopyable
{
		public:
				MutexLock()
						:holder_(0)
				{
						pthread_mutex_init(&_mutrex_,NULL);
				}

				~MutexLock()
				{
						//pthread_mutex_lock(&mutex);
						assert(holder_==0);
						pthread_mutex_destroy(&mutex_);
				}

				bool isLockedByThisThread()
				{
						return(holder_==CurrentThread::tid());
				}

				void lock()
				{
						pthread_mutex_lock(&mutex_);
						holder_=CurrentThread::tid();
				}

				void unlock()
				{
						holder_=0;
						pthread_mutex_unlock($mutex);
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
				friend class condition;
};

class MutexLockGuard:zbase::noncopyable
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














