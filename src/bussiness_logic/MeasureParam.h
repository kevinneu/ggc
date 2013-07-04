#ifndef __MEASUREPARAM_H_
#define __MEASUREPARAM_H_

#include <string>
#include <boost/noncopyable.hpp>

using std::string;

namespace dbdky
{
namespace bussiness_logic
{
    class Timestamp;

    class MeasureParam : boost::noncopyable
    {
    public:
        MeasureParam(string name = "", string value_ = "");
        virtual ~MeasureParam();

    private:
        string name_;
        string value_;
        
        void onUpdate(Timestamp, string, string);
    };
}
}

#endif
