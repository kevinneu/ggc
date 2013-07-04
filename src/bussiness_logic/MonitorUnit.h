#ifndef __MONITORUNIT_H_
#define __MONITORUNIT_H_

#include <boost/noncopyable.hpp>
#include <string>

namespace dbdky
{
namespace bussiness_logic
{
    class MonitorUnit : boost::noncopyable
    {
    public:
    private:
        std::string name_;
        std::string desc_;
        std::string interval_;
        std::string protocol_;
        std::string port_;
        std::string vendor_;
        std::string cycleid_;
        std::string mac_;
        std::string ytime_;
    };
}
}


#endif
