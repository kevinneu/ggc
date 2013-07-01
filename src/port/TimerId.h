#ifndef __TIMERID_H_
#define __TIMERID_H_

#include <utils/copyable.h>

namespace dbdky
{
    namespace port
    {
        class Timer;
        
	class TimerId : public dbdky::copyable
	{
	public:
	    TimerId()
		: timer_(NULL),
		 sequence_(0)
	    {
 	    }

	    TimerId(Timer* timer, int64_t seq)
		: timer_(timer),
		 sequence_(seq)
	    {
	    }

   	friend class TimerQueue;
	private:
	    Timer* timer_;
	    int64_t sequence_;
	};
    }
}


#endif
