#ifndef __TIMERQUEUE_H_
#define __TIMERQUEUE_H_

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include <utils/Mutex.h>
#include <utils/Timestamp.h>
#include <Callbacks.h>
#include <Channel.h>

namespace dbdky
{
    namespace port
    {
        class EventLoop;
        class Timer;
        class TimerId;

        class TimerQueue : boost::noncopyable
        {
        public:
            TimerQueue(EventLoop* loop);
            ~TimerQueue();

            TimerId addTimer(const TimerCallback& cb,
                         Timestamp when,
                         double interval);

            void cancel(TimerId timerId);

        private:
            // FIXME: use unique_ptr<Timer> instead of raw pointers.
            typedef std::pair<Timestamp, Timer*> Entry;
            typedef std::set<Entry> TimerList;
            typedef std::pair<Timer*, int64_t> ActiveTimer;
            typedef std::set<ActiveTimer> ActiveTimerSet;

            void addTimerInLoop(Timer* timer);
            void cancelInLoop(TimerId timeId);
            void handleRead();
            std::vector<Entry> getExpired(Timestamp now);
            void reset(const std::vector<Entry>& expired, Timestamp now);

            bool insert(Timer* timer);

            EventLoop* loop_;
            const int timerfd_;
            Channel timerfdChannel_;
            TimerList timers_;

            ActiveTimerSet activeTimers_;
            bool callingExpiredTimers_;
            ActiveTimerSet cancelingTimers_;
        };
    }
}


#endif
