#pragma once
#include "mutex.h"
#include "condition.h"
#include "noncopyable.h"

class CountDownLatch:noncopyable
{
		public:
				explicit CountDownLatch(int count)
				:count_(count),mutex_(),condition_(mutex_)
				{
		
				}

				int getCount()
				{
						MutexLockGuard mg(mutex_);
						return count_;
				}

				void wait()
				{
						MutexLockGuard mg(mutex_);
						while(count_>0)//为什么要用循环?
						{
								condition_.wait();//原因 condition_.wait函数会原子的unlockmutex并进入等待,不会与下面的countDown死锁
						}
				}

				void countDown()
				{
						MutexLockGuard mg(mutex_);
						count_--;
						if(count_==0)
						{
								condition_.notifyAll();
						}
				}

		private:
				int count_;
				MutexLock mutex_;
				Condition condition_;
};
