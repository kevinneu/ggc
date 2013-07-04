#include <MeasureParam.h>

using namespace dbdky;
using namespace dbdky::bussiness_logic;
using namespace dbdky::port;

MeasureParam::MeasureParam(string name, string value)
    : name_(name),
    : value_(value)
{
}

MeasureParam::~MeasureParam()
{
}

void MeasureParam::onUpdate(Timestamp time,
			string name,
			string value)
{
    if (name != name_)
    {
        return;
    }
    //TODO:
}

MeasureParam:
