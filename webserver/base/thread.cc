#include "thread.h"
#include <memory>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <stdint.h>
#include <assert.h>

#include <iostream>
using namespace std;

namespace currentthread
{
    __thread int t_cacheTid=0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength=6;
    __thread const char* t_threadName="default";
}

pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void currentthread::cacheTid()
{
    if(t_cacheTid==0)
    {
        t_cacheTid=gettid();
        t_tidStringLength =snprintf(t_tidString,sizeof(t_tidString),"%5d",t_cacheTid);
    }
}

struct ThreadData
{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    pid_t* tid_;
    string name_;
    CountDownLatch* latch_;

    ThreadData (ThreadFunc &func, string& name,pid_t* tid,CountDownLatch* latch)
    :func_(func),name_(name),tid_(tid),latch_(latch)
    {}

    void runInThread()
    {
        *tid_=currentthread::tid();
        tid_=NULL;
        latch_->countDown();
        latch_=NULL;

        currentthread::t_threadName=name_.empty()?"Thread":name_.c_str();
        prctl(PR_SET_NAME,currentthread::t_threadName);
        func_();//执行函数
        currentthread::t_threadName="Finished";
    }
};

void *startThread(void*obj)
{
    ThreadData *data=static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

Thread::Thread(const ThreadFunc &func,const string &n)
		:started_(false),
 		 joined_(false),
		 pthreadId_(0),
		 tid_(0),
		 func_(func),
		 name_(n),
		 latch_(1)
{
		setDefaultName();
}

Thread::~Thread()
{
		if(!started_&&!joined_)
				pthread_detach(pthreadId_);
}

void Thread::setDefaultName()
{
		if(name_.empty())
		{
				char buf[32];
				snprintf(buf,sizeof(buf),"Thread");
				name_=buf;
		}
}

void Thread::start()
{
    assert(!started_);
    started_=true;
    ThreadData* data=new ThreadData(func_,name_,&tid_,&latch_);
    if(pthread_create(&pthreadId_,NULL,&startThread,data))
    {
        started_=false;
        delete data;
    }
    else
    {
        latch_.wait();//等待struct ThreadData的函数被调用,即开始执行内层真正的功能函数
        assert(tid_>0);
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_=true;
    return pthread_join(pthreadId_,NULL);
}
