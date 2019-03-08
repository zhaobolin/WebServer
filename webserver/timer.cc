#include "timer.h"
#include <sys/time.h>
#include <unistd.h>
#include <queue>
TimerNode::TimerNode(TimerCallback cb,int timeout,double interval)
    :deleted_(false),
    interval_(interval),
    repeat_(interval>0.0),
    cb_(cb)
{
    struct timeval now;
    gettimeofday(&now,NULL);
    expiredTime_=(((now.tv_sec%10000)*1000)+(now.tv_usec/1000))+timeout;
    //numcreated_++;
}

TimerNode::~TimerNode()
{
}

TimerNode::TimerNode(TimerNode& tn)
:cb_(tn.cb_)
{
   
}
void TimerNode::update(int timeout)//更新等待时间
{
    struct timeval now;
    gettimeofday(&now,NULL);
    expiredTime_=(((now.tv_sec%10000)*1000)+(now.tv_usec/1000))+timeout;
}

bool TimerNode::isValid()
{
    struct timeval now;
    gettimeofday(&now,NULL);
    size_t temp=(((now.tv_sec%10000)*1000)+(now.tv_usec/1000));
    if(temp<expiredTime_)//未到期
    {
        return true;
    }
    else//到期了
    {
        this->setDeleted();
        return false;
    }
    
}


TimerManager::TimerManager()
{

}
TimerManager::~TimerManager()
{

}

void TimerManager::addTimer(TimerCallback cb,int timeout,double interval)
{
    SPTimerNode new_node(new TimerNode(cb,timeout,interval));
    timerNodeQueue.push(new_node);
}

std::vector<SPTimerNode> TimerManager::getExpried()
{
    std::vector<SPTimerNode> res;
    while(!timerNodeQueue.empty())
    {
        SPTimerNode now=timerNodeQueue.top();
        if(now->isDeleted())
        {
            res.push_back(now);
            timerNodeQueue.pop();
        }
        else if(now->isValid()==false)
        {
            res.push_back(now);
            timerNodeQueue.pop();
        }
        else
        {
            break;
        }
    }
    return res;
}
