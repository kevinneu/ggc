#ifndef __EVENTLOOPTHREAD_H_
#define __EVENTLOOPTHREAD_H_

#include <utils/Condition.h>
#include <utils/Mutex.h>
#include <utils/Thread.h>

#include <boost/noncopyable.hpp>

namespace dbdky
{
    namespace port
    {
        class EventLoop;
        class EventLoopThread : boost::noncopyable
        {
        public:
            typedef boost::function<void(EventLoop*)> ThreadInitCallback;
            EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
            ~EventLoopThread();
            EventLoop* startLoop();

        private:
            void threadFunc();
  
            EventLoop* loop_;
            bool exiting_;
            Thread thread_;
            MutexLock mutex_;
            Condition cond_;
            ThreadInitCallback callback_;
        };
    }
}

#endif
