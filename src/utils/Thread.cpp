#include <utils/Thread.h>
#include <utils/CurrentThread.h>
#include <utils/Exception.h>
#include <utils/Logging.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace dbdky
{
    namespace CurrentThread
    {
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread const char* t_threadName = "unknown";
       
        const bool sameType = boost::is_same<int, pid_t>::value;
        BOOST_STATIC_ASSERT(sameType);
    }

    namespace detail
    {
        pid_t gettid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

        void afterFork()
        {
            dbdky::CurrentThread::t_cachedTid = 0;
            dbdky::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
        }

        class ThreadNameInitializer
        {
        public:
            ThreadNameInitializer()
            {
                dbdky::CurrentThread::t_threadName = "main";
                CurrentThread::tid();
                pthread_atfork(NULL, NULL, &afterFork);
            }
        };

        ThreadNameInitializer init;

        struct ThreadData
        {
            typedef dbdky::Thread::ThreadFunc ThreadFunc;
            ThreadFunc func_;
            string name_;
            boost::weak_ptr<pid_t> wkTid_;

            ThreadData(const ThreadFunc& func,
			const string& name,
			const boost::shared_ptr<pid_t>& tid)
		: func_(func),
		  name_(name),
		  wkTid_(tid)
            {
	    }

            void runInThread()
            {
                pid_t tid = dbdky::CurrentThread::tid();
       
                boost::shared_ptr<pid_t> ptid = wkTid_.lock();
                if (ptid)
                {
                    *ptid = tid;
                    ptid.reset();
                }

                dbdky::CurrentThread::t_threadName = name_.c_str();
                try
                {
                    func_();
                    dbdky::CurrentThread::t_threadName = "finished";
                }
                catch (const Exception& ex)
                {
                    dbdky::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
 		    fprintf(stderr, "reason: %s\n", ex.what());
 		    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
                    abort();
                }
                catch (const std::exception& ex)
                {
  		    dbdky::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
		    fprintf(stderr, "reason: %s\n", ex.what());
                    abort();
                }
                catch (...)
                {
                    dbdky::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
                    throw;
                }
            }
        };

        void* startThread(void* obj)
        {
            ThreadData* data = static_cast<ThreadData*>(obj);
            data->runInThread();
            delete data;
            return NULL;
        }
    }
}

using namespace dbdky;

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = detail::gettid();
        int n = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        assert(n == 6);
        (void)n;
    }
}

bool CurrentThread::isMainThread()
{
    return tid() == ::getpid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const string& n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(new pid_t(0)),
      func_(func),
      name_(n)
{
    numCreated_.increment();
}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        pthread_detach(pthreadId_);
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    detail::ThreadData* data = new detail::ThreadData(func_, name_, tid_);
    if (pthread_create(&pthreadId_, NULL, &detail::startThread, data))
    {
        started_ = false;
        delete data;
        LOG_SYSFATAL << "Failed in pthread_create";
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    
    return pthread_join(pthreadId_, NULL);
}
