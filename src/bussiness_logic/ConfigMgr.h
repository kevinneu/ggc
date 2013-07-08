#ifndef __CONFIGMGR_H_
#define __CONFIGMGR_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <map>
#include <list>

#include <bl_callbacks.h>
#include <utils/Timestamp.h>


namespace dbdky
{
namespace bussiness_logic
{
    class ComEntity;
    class MonitorUnit;
    
    class ConfigMgr : boost::noncopyable
    {
    public:
        typedef std::list<ComEntityPtr> ComEntityList;
        typedef std::map<string, MonitorUnitPtr> MonitorUnitMap;

        ConfigMgr(const string& configPath);
        virtual ~ConfigMgr();

        void update(Timestamp time = Timestamp::now());
       
        void setConfigUpdateCallback(const ConfigUpdateCallback& cb)
        {
            configUpdateCallback_ = cb;
        }
 
    private:
        void onUpdate(Timestamp);

    private:
        std::string globalConfigPath_;
        ConfigUpdateCallback configUpdateCallback_; 

        ComEntityList comEntityList_;
        MonitorUnitMap monitorUnitMap_;
    };
}
}


#endif
