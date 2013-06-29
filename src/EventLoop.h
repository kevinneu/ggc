#ifndef __EVENTLOOP_H_
#define __EVENTLOOP_H_

#include <vector>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <utils/Mutex.h>
#include <utils/Thread.h>
#include <utils/Timestamp.h>
#include <Callbacks.h>
#include <TimerId.h>

namespace dbdky
{
    namespace port
    {
        class Channel;
        class Poller;
        class TimerQueue;

        class EventLoop : boost::noncopyable
        {
        public:
            typedef boost::function<void()> Functor;

            EventLoop();
            ~EventLoop();

            void loop();
            void quit();

            Timestamp pollReturnTime() const
            {
                return pollReturnTime_;
            }

            int64_t iteration() const
            {
                return iteration_;
            }

            void runInLoop(const Functor& cb);
            void queueInLoop(const Functor& cb);
            TimerId runAt(const Timestamp& time, const TimerCallback& cb);
            TimerId runAfter(double delay, const TimerCallback& cb);
            TimerId runEvery(double interval, const TimerCallback& cb);
            void cancel(TimerId timerId);

            void wakeup();
            void updateChannel(Channel* channel);
            void removeChannel(Channel* channel);
           
            void assertInLoopThread()
            {
                if (!isInLoopThread())
                {
                    abortNotInLoopThread();
                }
            }

            bool isInLoopThread() const
            {
                return threadId_ == CurrentThread::tid();
            }

            bool eventHandling() const
            {
                return eventHandling_;
            }

            static EventLoop* getEventLoopOfCurrentThread();

        private:
            void abortNotInLoopThread();
            void handleRead();
            void doPendingFunctors();
            void printActiveChannels() const;
  
            typedef std::vector<Channel*> ChannelList;

            bool looping_;
            bool quit_;
            bool eventHandling_;
            bool callingPendingFunctors_;
            int64_t iteration_;
            const pid_t threadId_;
            Timestamp pollReturnTime_;
            boost::scoped_ptr<Poller> poller_;
            boost::scoped_ptr<TimerQueue> timerQueue_;
            int wakeupFd_;
            boost::scoped_ptr<Channel> wakeupChannel_;
            ChannelList activeChannels_;
            Channel* currentActiveChannel_;
            MutexLock mutex_;
            std::vector<Functor> pendingFunctors_;
        };
    }
}


#endif
