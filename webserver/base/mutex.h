#include <pthread.h>

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
						assert(holder_==0);
						pthread_mutex_destroy(&mutex_);
				}

				bool isLockedByThisThread()
				{
						return(holder_==CurrentThread::tid());
				}

				void Lock()
				{
						pthread_mutex_lock(&mutex_);
						holder_=CurrentThread:;tid();
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
}

