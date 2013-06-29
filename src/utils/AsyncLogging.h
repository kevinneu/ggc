#ifndef __ASYNCLOGGING_H_
#define __ASYNCLOGGING_H_

#include <utils/BlockingQueue.h>
#include <utils/BoundedBlockingQueue.h>
#include <utils/CountDownLatch.h>
#include <utils/Mutex.h>
#include <utils/Thread.h>

#include <utils/LogStream.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace dbdky
{
    class AsyncLogging : boost::noncopyable
    {
    public:
        AsyncLogging(const string& basename,
		size_t rollSize,
		int flushInterval = 3);
 
   	~AsyncLogging()
        {
            if (running_)
            {
                stop();
            }
        }

        void append(const char* logline, int len);
        void start()
        {
            running_ = true;
            thread_.start();
            latch_.wait();
        }

        void stop()
        {
            running_ = false;
            cond_.notify();
            thread_.join();
        }

    private:
        AsyncLogging(const AsyncLogging&);
        void operator=(const AsyncLogging&);

        void threadFunc();

        typedef dbdky::detail::FixedBuffer<dbdky::detail::kLargeBuffer> Buffer;
        typedef boost::ptr_vector<Buffer> BufferVector;
        typedef BufferVector::auto_type BufferPtr;

        const int flushInterval_;
        bool running_;
        string basename_;
        size_t rollSize_;
        dbdky::Thread thread_;
        dbdky::CountDownLatch latch_;
        dbdky::MutexLock mutex_;
        dbdky::Condition cond_;
        BufferPtr currentBuffer_;
        BufferPtr nextBuffer_;
        BufferVector buffers_;
    };
}

#endif
