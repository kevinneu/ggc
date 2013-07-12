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
namespace port
{
    class ComEntity;
    class MonitorUnit;
    
    class ConfigMgr : boost::noncopyable
    {
    public:
        typedef std::map<string, ComEntityPtr> ComEntityMap;
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
        void parseConfigFile();
        void parseComConfig(const string& refFileName);
        void parseDBConfig(const string& refFileName);
        void parseMonitorUnitConfig(const string& refFileName);

    private:
        std::string globalConfigPath_;
        ConfigUpdateCallback configUpdateCallback_; 

        ComEntityMap comEntityMap_;
        MonitorUnitMap monitorUnitMap_;
    };
}
}


#endif
