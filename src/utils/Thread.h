#ifndef __THREAD_H_
#define __THREAD_H_

#include <utils/Atomic.h>
#include <utils/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

namespace dbdky
{
    class Thread : boost::noncopyable
    {
    public:
        typedef boost::function<void ()> ThreadFunc;
   
        explicit Thread(const ThreadFunc&, const string& name = string());
        ~Thread();

        void start();
        int join();

        bool started() const { return started_; }
        pid_t tid() const { return *tid_; }
        const string& name() const { return name_; }
        static int numCreated() { return numCreated_.get(); }

    private:
        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        boost::shared_ptr<pid_t> tid_;
        ThreadFunc func_;
        string name_;

        static AtomicInt32 numCreated_;
    };
}


#endif
