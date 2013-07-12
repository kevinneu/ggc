#include <ConfigMgr.h>

#include <utils/FileUtil.h>
#include <utils/Logging.h>

#include <bl_callbacks.h>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include <tinyxml.h>
#include <ComEntity.h>

using namespace dbdky;
using namespace dbdky::port;

const string configFileName = "GlobalConf.xml";

ConfigMgr::ConfigMgr(const string& configPath)
    : globalConfigPath_(configPath),
      configUpdateCallback_(boost::bind(&ConfigMgr::onUpdate, this, _1))
{
    LOG_INFO << "configPath: " << configPath;
}

ConfigMgr::~ConfigMgr()
{
}

void ConfigMgr::update(Timestamp time)
{
    LOG_INFO << "Enter";
    if (configUpdateCallback_)
    {
        configUpdateCallback_(time);
    }
}

void ConfigMgr::onUpdate(Timestamp time)
{
    //TODO:
    // 1. cleanup the old entities.
    // 2. Create new.

    parseConfigFile();
}

void ConfigMgr::parseConfigFile()
{
    string content;
    string globalConfigFile = "";
    int err;
    bool res;

    int64_t size = 0;
    
    if (globalConfigPath_ == "")
    {
        LOG_WARN << "globalConfigPath is not set";
        return;
    }

    globalConfigFile += globalConfigPath_;
    globalConfigFile += "/";
    globalConfigFile += configFileName;

#if 0
    err = FileUtil::readFile(globalConfigFile, 102400, &content, &size);
    if (err != 0)
    {
        LOG_ERROR << "Open File: " << globalConfigFile << "error";
        return;
    }
    
    LOG_INFO << "Read config file: " << globalConfigFile << "size: " << content.size();
    
    LOG_INFO << "Content: " << content;

#endif

    TiXmlDocument docSrc(globalConfigFile.c_str());
    res = docSrc.LoadFile(globalConfigFile.c_str());

    if (!res)
    {
        LOG_ERROR << "load xml string error: " << content;
        return;
    }
 
    TiXmlElement* root = docSrc.RootElement();
    TiXmlNode* node = 0;

    for (node = root->FirstChild("Config-Item");
            node;
            node = node->NextSibling("Config-Item"))
    {
        TiXmlElement* configItemElement = node->ToElement();
        if (NULL == configItemElement)
        {
            LOG_ERROR << "Empty node found";
            break;
        }

        if ( (NULL != configItemElement->Attribute("name"))
            && (NULL != configItemElement->Attribute("desc"))
            && (NULL != configItemElement->Attribute("ref")))
        {
            string configType(configItemElement->Attribute("name"));
            string configRef(configItemElement->Attribute("ref"));

            if ("ComConfig" == configType)
            {
                parseComConfig(configRef);
            }
            else if ("DBConfig" == configType)
            {
                parseDBConfig(configRef);
            }
            else if ("MonitorUnitConfig" == configType)
            {
                parseMonitorUnitConfig(configRef);
            }
            else
            {
                LOG_ERROR << "Unknown config type";
                continue;
            }
        }
        else
        {
            LOG_ERROR << "Bad config-item";
            continue;
        }
    }
}

void ConfigMgr::parseComConfig(const string& refFileName)
{
    LOG_INFO << "refFileName: " << refFileName;
    
    string comConfigFileName = "";
    bool res;
    int64_t size = 0;
    
    if (globalConfigPath_ == "")
    {
        LOG_WARN << "globalConfigPath is not set";
        return;
    }

    comConfigFileName += globalConfigPath_;
    comConfigFileName += "/";
    comConfigFileName += refFileName;

    TiXmlDocument docSrc(comConfigFileName.c_str());
    res = docSrc.LoadFile(comConfigFileName.c_str());

    if (!res)
    {
        LOG_ERROR << "Parse xml file error: " << comConfigFileName;
        return;
    }

    TiXmlElement* root = docSrc.RootElement();
    TiXmlNode* node = 0;
    TiXmlNode* subNode = 0;

    string portName("");
    string baudRate("");
    string dataBits("");
    string stopBits("");
    string parity("");

    for (node = root->FirstChild("ComConfig");
            node;
            node->NextSibling("ComConfig"))
    {
        TiXmlElement* comConfigElement = node->ToElement();
        string portName("");
        string dataBits("");
        string baudRate("");
        string parity("");


        for (subNode = node->FirstChild("Item");
                subNode;
                subNode->NextSibling("Item"))
        {
            TiXmlElement* itemElement = subNode->ToElement();
            if ((NULL != itemElement->Attribute("name"))
                && (NULL != itemElement->Attribute("value"))) 
            {
                {
                    string itemName(itemElement->Attribute("name"));
                    string itemValue(itemElement->Attribute("value"));

                    if ("PortName" == itemName)
                    {
                        portName = itemValue;
                    }
                    else if ("BaudRate" == itemName)
                    {
                        baudRate = itemValue;
                    }
                    else if ("DataBits" == itemName)
                    {
                        dataBits = itemValue;
                    }
                    else if ("StopBits" == itemName)
                    {
                        stopBits = itemValue;
                    }
                    else if ("Parity" == itemName)
                    {
                        parity = itemValue;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            else
            {
                LOG_WARN << "Invalid config item";
                continue;
            }
        }        


        if (("" != portName)
	    && ("" != baudRate)
	    && ("" != dataBits)
	    && ("" != stopBits)
	    && ("" != parity))
        {
            ComEntity::eMBParity tParity;
            switch (atoi(parity.c_str()))
            {
                case 0:
                {
                    tParity = port::ComEntity::MB_PAR_NONE;
                    break;
                }
                case 1:
                {
                    tParity = port::ComEntity::MB_PAR_ODD;
                    break;
                }
                case 2:
                {
                    tParity = port::ComEntity::MB_PAR_EVEN;
                    break;
                }
                default:
                {
                    tParity = port::ComEntity::MB_PAR_NONE;
                }
            }
     
            ComEntityPtr comEntity(new ComEntity(NULL, portName, 
		static_cast<unsigned char>(atoi(dataBits.c_str())), 
		atol(baudRate.c_str()), 
		tParity)
		);
            comEntityMap_[portName] = comEntity;
        }
        else
        {
            LOG_ERROR << "Insufficient config information for COM. will ignore";
            continue;
        }
    }
}

void ConfigMgr::parseDBConfig(const string& refFileName)
{
    LOG_INFO << "refFileName: " << refFileName;

    string DBConfigFile = "";
    int err;
    bool res;

    int64_t size = 0;
    
    if (globalConfigPath_ == "")
    {
        LOG_WARN << "globalConfigPath is not set";
        return;
    }

    DBConfigFile += globalConfigPath_;
    DBConfigFile += "/";
    DBConfigFile += refFileName;

}

void ConfigMgr::parseMonitorUnitConfig(const string& refFileName)
{
    LOG_INFO << "refFileName: " << refFileName;

    string monitorUnitConfigFile = "";
    bool res;
    int64_t size = 0;
    
    if (globalConfigPath_ == "")
    {
        LOG_WARN << "globalConfigPath is not set";
        return;
    }

    monitorUnitConfigFile += globalConfigPath_;
    monitorUnitConfigFile += "/";
    monitorUnitConfigFile += refFileName;
}

