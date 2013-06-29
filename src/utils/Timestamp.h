#ifndef __TIMESTAMP_H_
#define __TIMESTAMP_H_

#include <utils/copyable.h>
#include <utils/Types.h>

#include <boost/operators.hpp>

namespace dbdky
{
    class Timestamp : public dbdky::copyable,
                      public boost::less_than_comparable<Timestamp>
    {
    public:
        Timestamp()
            : microSecondsSinceEpoch_(0)
        {
        }

        explicit Timestamp(int64_t microSecondsSinceEpoch);
        void swap(Timestamp& that)
        {
            std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
        }

        string toString() const;
        string toFormattedString() const;
        
        bool valid() const { return microSecondsSinceEpoch_ > 0; }

        int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
       
        time_t secondsSinceEpoch() const
        {
            return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
        }

        static Timestamp now();
        static Timestamp invalid();

        static const int kMicroSecondsPerSecond = 1000 * 1000;

    private:
        int64_t microSecondsSinceEpoch_;
    };

    inline bool operator<(Timestamp lhs, Timestamp rhs)
    {
        return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
    }

    inline bool operator==(Timestamp lhs, Timestamp rhs)
    {
        return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
    }

    inline double timeDifference(Timestamp high, Timestamp low)
    {
        int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
        
        return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
    }

    inline Timestamp addTime(Timestamp timestamp, double seconds)
    {
        int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  
        return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
    }
}

#endif
