#ifndef __TIMEZONE_H_
#define __TIMEZONE_H_

#include <utils/copyable.h>
#include <boost/shared_ptr.hpp>
#include <time.h>

namespace dbdky
{
    class TimeZone : public dbdky::copyable
    {
    public:
        explicit TimeZone(const char* zonefile);

        bool valid() const { return data_; }
 
        struct tm toLocalTime(time_t secondsSinceEpoch) const;

        time_t fromLocalTime(const struct tm&) const;

        static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);

        static time_t fromUtcTime(const struct tm&);

        static time_t fromUtcTime(int year, int month, int day,
				int hour, int minute, int seconds);

        struct Data;

    private:
        boost::shared_ptr<Data> data_;
    };
}

#endif
