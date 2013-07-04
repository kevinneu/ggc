#ifndef __MEASUREPOINT_H_
#define __MEASUREPOINT_H_

#include <string>
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <bussiness_logic/bl_callbacks.h>

namespace dbdky
{
namespace bussiness_logic
{
    class Timestamp;
    class EventLoop;

    class MeasureParam : boost::noncopyable
    {
    public:
        MeasureParam(std::string name = "", std::string value = "")
            : name_(name),
              value_(value)
        {
        }

        virtual ~MeasureParam() {}

        void setName(const std::string& name)
        {
            name_ = name;
        }

        void setValue(const std::string& value)
        {
            value_ = value;
        }

        std::string getName() const
        {
            return name_; 
        }

        std::string getValue() const
        {
            return value_;
        }

    private:
        std::string name_;
        std::string value_;
    };

    class MeasurePoint : boost::noncopyable
    {
    public:
    typedef std::map<string, MeasureParamPtr> MeasureParamMap;
        
        MeasurePoint();
        virtual ~MeasurePoint();

        void init();
        
    private:
        string iedname_;
        string deviceid_;
        string id_;
        string checktime_;
        MeasurePointUpdateCallback meaurePointUpdateCallback_;
        MeasureParamMap params_;
        
    };
}
}


#endif
