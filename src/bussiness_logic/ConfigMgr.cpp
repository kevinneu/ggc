#include <ConfigMgr.h>

using namespace dbdky;
using namespace dbdky::bussiness_logic;

void ConfigMgr::update(Timestamp time)
{
    if (configUpdateCallback_)
    {
        configUpdateCallback_(time);
    }
}

void ConfigMgr::onUpdate(Timestamp time)
{
}


