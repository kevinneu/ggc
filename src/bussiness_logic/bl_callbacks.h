#ifndef __BL_CALLBACKS_H_
#define __BL_CALLBACKS_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <utils/Timestamp.h>

namespace dbdky
{
namespace bussiness_logic
{
    class MeasurePoint;
    class MeasureParam;
    class MonitorUnit;

    typedef boost::shared_ptr<MeasurePoint> MeasurePointPtr;
    typedef boost::shared_ptr<MeasureParam> MeasureParamPtr;
    typedef boost::shared_ptr<MonitorUnit> MeasureUnitPtr;

    typedef boost::function<void(Timestamp, string, string)> MeasurePointUpdateCallback;

    typedef boost::function<void(Timestamp, const MeasureUnitPtr&)> MeasureUnitOnIntervalCallback;
}
}
#endif
