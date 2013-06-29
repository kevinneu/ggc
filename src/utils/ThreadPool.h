#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include <utils/Condition.h>
#include <utils/Mutex.h>
#include <utils/Thread.h>
#include <utils/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace dbdky
{
    class ThreadPool : boost::noncopyable
    {
    public:
        typedef boost::function<void()> Task;
        explicit ThreadPool(const string& name = string());
        ~ThreadPool();
        void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
        void start(int numThreads);
        void stop();

        void run(const Task& f);

    private:
        bool isFull() const;
        void runInThread();
        Task take();
     
        MutexLock mutex_;
        Condition notEmpty_;
        Condition notFull_;
        string name_;
        boost::ptr_vector<dbdky::Thread> threads_;
        std::deque<Task> queue_;
        size_t maxQueueSize_;
        bool running_;
    };
}

#endif
