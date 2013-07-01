#ifndef __EVENTLOOPTHREADPOOL_H_
#define __EVENTLOOPTHREADPOOL_H_

#include <utils/Condition.h>
#include <utils/Mutex.h>

#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace dbdky
{
    namespace port
    {
        class EventLoop;
        class EventLoopThread;
  
        class EventLoopThreadPool : boost::noncopyable
        {
        public:
            typedef boost::function<void(EventLoop*)> ThreadInitCallback;

            EventLoopThreadPool(EventLoop* baseLoop);
            ~EventLoopThreadPool();
            void setThreadNum(int numThreads) { numThreads_ = numThreads; }
            void start(const ThreadInitCallback& cb = ThreadInitCallback());
            EventLoop* getNextLoop();

        private:
            EventLoop* baseLoop_;
            bool started_;
            int numThreads_;
            int next_;
            boost::ptr_vector<EventLoopThread> threads_;
            std::vector<EventLoop*> loops_;
        };
    }
}


#endif
