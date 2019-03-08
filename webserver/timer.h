#pragma once
#include <stdio.h>
#include <memory>
#include <queue>
#include <functional>
#include <vector>
typedef std::function<void()> TimerCallback;

class TimerNode
{
    public:
        
        TimerNode(TimerCallback cb,int timeout,double interval);
        ~TimerNode();
        TimerNode(TimerNode &tn);
        void update(int timeout);
        //void run(){cb_();}
        bool isValid();//检查是否到期
        void setDeleted(){deleted_=true;}
        bool isDeleted(){return deleted_;}
        size_t getExpTime(){return expiredTime_;}

        static int numCreated(){return numcreated_;}
    private:
        bool deleted_;
        size_t expiredTime_;
        double interval_;//超过时间间隔，对于一次性定时器，值为零
        bool repeat_;//是否重复
        TimerCallback cb_;

        static int numcreated_;
};
struct TimerCmp//比较函数，用于排序
{
    bool operator()(std::shared_ptr<TimerNode>&a,std::shared_ptr<TimerNode>&b)//仿函数，a>b的时候，a在b后面
    {
        return a->getExpTime()>b->getExpTime();       
    }
};
typedef std::shared_ptr<TimerNode> SPTimerNode;

class TimerManager//时间队列，待处理的延时事件在这里
{
    public:
        TimerManager();
        ~TimerManager();
        void addTimer(TimerCallback cb,int timeout,double interval);
        std::vector<SPTimerNode> getExpried();
    private:
        std::priority_queue<SPTimerNode,std::deque<SPTimerNode>,TimerCmp> timerNodeQueue;

};

/*class Timer
{
    public:
     Timer(const Callback &cb,Timestamp when,double interval)
       :callback_(cb),
        expiration_(when),
        interval_(interval),
        repeat_(interval>0.0),
        sequence_(s_numCreated_.incrementAndGet())
     {}
    private:
        typedef std::function<void> Callback;
        Callback callback_;//定时器回调函数
        Timestamp expiration_;
        const double interval_;//超过时间间隔，对于一次性定时器，值为零
        const bool repeat_;//是否重复
        const int sequence_;//定时器序号
        static int s_numCreated_;
};
class TimerId
{
    public:
        TimerId():
            timer_(NULL),
            sequence_(0)
        {

        }
        TimerId(Timer* timer,int seq):
            timer_(timer),
            sequence_(seq)
        {

        }

        friend class TimerQueue;
    private:
        Timer* timer_;
        int sequence_;
};
*/


