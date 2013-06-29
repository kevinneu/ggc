#ifndef __COUNTDOWNLATCH_H_
#define __COUNTDOWNLATCH_H_

#include <utils/Condition.h>
#include <utils/Mutex.h>

#include <boost/noncopyable.hpp>

namespace dbdky
{
    class CountDownLatch : boost::noncopyable
    {
    public:
        explicit CountDownLatch(int count);
        void wait();
        void countDown();
        int getCount() const;
    private:
        mutable MutexLock mutex_;
        Condition condition_;
        int count_;
    };
}


#endif
