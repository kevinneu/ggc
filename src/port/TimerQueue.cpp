#define __STDC_LIMIT_MACROS
#include <TimerQueue.h>

#include <utils/Logging.h>
#include <EventLoop.h>
#include <Timer.h>
#include <TimerId.h>

#include <boost/bind.hpp>
#include <sys/timerfd.h>

namespace dbdky
{
    namespace port
    {
        namespace detail
        {
            int createTimerfd()
            {
                int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
				TFD_NONBLOCK | TFD_CLOEXEC);

                if (timerfd < 0)
                {
                    LOG_SYSFATAL << "Failed in timerfd_create";
                }

                return timerfd;
            }

            struct timespec howMuchTimeFromNow(Timestamp when)
            {
                int64_t microseconds = when.microSecondsSinceEpoch() 
				- Timestamp::now().microSecondsSinceEpoch();

                if (microseconds < 100)
                {
		    microseconds = 100;
                }

                struct timespec ts;
                ts.tv_sec = static_cast<time_t>(
			microseconds / Timestamp::kMicroSecondsPerSecond);
		ts.tv_nsec = static_cast<long>(
			(microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);

		return ts;
            }

            void readTimerfd(int timerfd, Timestamp now)
            {
                uint64_t howmany;
                ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
                LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
                //if (n != sizeof howmany)
                //{
                //    LOG_ERROR << "TimerQueue::handleRead() reads" << n << " bytes instead of 8";
                //}
            }

            void resetTimerfd(int timerfd, Timestamp expiration)
            {
                struct itimerspec newValue;
                struct itimerspec oldValue;
                bzero(&newValue, sizeof newValue);
                bzero(&oldValue, sizeof oldValue);
                newValue.it_value = howMuchTimeFromNow(expiration);
                int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
                if (ret)
                {
                    LOG_SYSERR << "timerfd_settime()";
                }
            }
        }
    }
}

using namespace dbdky;
using namespace dbdky::port;
using namespace dbdky::port::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop, timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
    timerfdChannel_.setReadCallback(
	boost::bind(&TimerQueue::handleRead, this));
    timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(timerfd_);
    for (TimerList::iterator it = timers_.begin();
		it != timers_.end(); it++)
    {
        delete it->second;
    }
}

TimerId TimerQueue::addTimer(const TimerCallback& cb,
		Timestamp when,
		double interval)
{
    Timer* timer = new Timer(cb, when, interval);
    loop_->runInLoop(
	boost::bind(&TimerQueue::addTimerInLoop, this, timer));
    return TimerId(timer, timer->sequence());
}

void TimerQueue::cancel(TimerId timerId)
{
    loop_->runInLoop(
	boost::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged = insert(timer);
    if (earliestChanged)
    {
        resetTimerfd(timerfd_, timer->expiration());
    }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
    loop_->assertInLoopThread();
    //assert(timers_.size() == activeTimers_.size());
    ActiveTimer timer(timerId.timer_, timerId.sequence_);
    ActiveTimerSet::iterator it = activeTimers_.find(timer);
    if (it != activeTimers_.end())
    {
        size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
        assert(n == 1);
        (void)n;
        delete it->first;
        activeTimers_.erase(it);
    }
    else if (callingExpiredTimers_)
    {
        cancelingTimers_.insert(timer);
    }

    //assert(timers_.size() == activeTimers_.size());
}

void TimerQueue::handleRead()
{
    loop_->assertInLoopThread();
    Timestamp now(Timestamp::now());
    readTimerfd(timerfd_, now);

    std::vector<Entry> expired = getExpired(now);

    callingExpiredTimers_ = true;
    cancelingTimers_.clear();
    for (std::vector<Entry>::iterator it = expired.begin();
	it != expired.end(); it++)
    {
        it->second->run();
    }

    callingExpiredTimers_ = false;

    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    //assert(timers_.size() == activeTimers_.size());
    std::vector<Entry> expired;
    //Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    Entry sentry(now, reinterpret_cast<Timer*>(UINT32_MAX));
    TimerList::iterator end = timers_.lower_bound(sentry);
    assert(end == timers_.end() || now < end->first);
    std::copy(timers_.begin(), end, back_inserter(expired));
    timers_.erase(timers_.begin(), end);
 
    for (std::vector<Entry>::iterator it = expired.begin(); it != expired.end(); it++)
    {
        ActiveTimer timer(it->second, it->second->sequence());
        size_t n = activeTimers_.erase(timer);
        //assert(n == 1);
        (void)n;
    }

    //assert(timers_.size() == activeTimers_.size());
    return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
    Timestamp nextExpire;

    for (std::vector<Entry>::const_iterator it = expired.begin();
        it != expired.end(); it++)
    {
        ActiveTimer timer(it->second, it->second->sequence());
        if (it->second->repeat()
		&& cancelingTimers_.find(timer) == cancelingTimers_.end())
        {
            it->second->restart(now);
            insert(it->second);
        }
        else
        {
            delete it->second;
        }
    }

    if (!timers_.empty())
    {
        nextExpire = timers_.begin()->second->expiration();
    }
    
    if (nextExpire.valid())
    {
        resetTimerfd(timerfd_, nextExpire);
    }
}

bool TimerQueue::insert(Timer* timer)
{
    loop_->assertInLoopThread();
    //assert(timers_.size() == activeTimers_.size());
    bool earliestChanged = false;
    Timestamp when = timer->expiration();
    TimerList::iterator it = timers_.begin();
    if (it == timers_.end() || when < it->first)
    {
        earliestChanged = true;
    }

    {
        std::pair<TimerList::iterator, bool> result = timers_.insert(Entry(when, timer));
        assert(result.second);
        (void)result;
    }

    //assert(timers_.size() == activeTimers_.size());
    return earliestChanged;
}
