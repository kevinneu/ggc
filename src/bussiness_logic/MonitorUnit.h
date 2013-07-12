#ifndef __MONITORUNIT_H_
#define __MONITORUNIT_H_


#include <boost/noncopyable.hpp>
#include <string>

#include <MeasurePoint.h>

#include <port/TimerId.h>
//#include <utils/Timestamp.h>

namespace dbdky
{
namespace port
{
    class EventLoop;
    class Timestamp;

    class MonitorUnit : boost::noncopyable
    {
    public:
        typedef std::map<string, MeasurePointPtr> MeasurePointMap;

        MonitorUnit(EventLoop* loop, string name,
			string desc,
			string interval,
			string protocol,
			string port,
			string vendor,
			string cycleid,
			string mac,
			string ytime)
	: loop_(loop),
          name_(name),
	  desc_(desc),
	  interval_(interval),
          ts_interval_(atoll(interval_.c_str())),
	  protocol_(protocol),
	  port_(port),
          vendor_(vendor),
          cycleid_(cycleid),
          mac_(mac),
          ytime_(ytime)
        {
            //TODO:

        }

        virtual ~MonitorUnit(){}

        void start();
        void stop();
        bool isRunning() const;
      
        void setName(const string& name)
        {
            name_ = name;
        }

        string getName() const
        {
	    return name_;
	}

        void setDesc(const string& desc)
        {
	    desc_ = desc;
	}
     
        string getDesc() const
        {
	    return desc_;
	}

        void setInterval(const string& newInterval, string& oldInterval)
        {
            oldInterval = interval_;
            updateInterval(newInterval);
        } 

        string getInterval() const
        {
            return interval_;
        }

        void setProtocol(const string& newProtocol, string& oldProtocol)
        {
            oldProtocol =  protocol_;
            updateProtocol(newProtocol);
        }

        string getProtocol() const
        {
            return protocol_;
        }

        void setPort(const string& newPort, string& oldPort)
        {
            oldPort = port_;
            updatePort(newPort);
        }
  
        string getPort() const
        {
	    return port_;
	}

        void setVendor(const string& vendor)
        {
            vendor_ = vendor;
        }

        string getVendor() const
        {
            return vendor_;
        }
  
        void setCycleid(const string& cycleid)
        {
            cycleid_ = cycleid;
        }

        string getCycleid() const
        {
            return cycleid_;
        }

        void setMac(const string& mac)
        {
            mac_ = mac;
        }

        string getMac() const
        {
            return mac_;
        }

        void setYtime(const string& ytime)
        {
            ytime_ = ytime;
        }

        string getYtime() const
        {
            return ytime_;
        }

	void setMonitorUnitOnIntervalCallback(const MonitorUnitOnIntervalCallback& cb)
        {
            monitorUnitOnIntervalCallback_ = cb;
        }

        void setUpdateDBCallback(const UpdateDBCallback& cb)
        {
            updateDBCallback_ = cb;
        }

    private:
        void updateInterval(const string& newInterval);
        void updateProtocol(const string& newProtocol);
        void updatePort(const string& newPort);

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

        EventLoop* loop_;
        dbdky::port::TimerId tInterval_;
        dbdky::Timestamp ts_interval_;
        MonitorUnitOnIntervalCallback monitorUnitOnIntervalCallback_;
        MeasurePointMap measurePoints_;
        UpdateDBCallback updateDBCallback_;
    };
}
}


#endif
