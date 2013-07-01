#include <ComServer.h>

#include <utils/Logging.h>
#include <port/EventLoop.h>
#include <port/EventLoopThreadPool.h>

#include <boost/bind.hpp>

#include <stdio.h>
#include <utility>


using namespace dbdky;
using namespace dbdky::port;

ComServer::ComServer(EventLoop* loop, const string& nameArg)
  : loop_(loop),
    name_(nameArg),
    threadPool_(new EventLoopThreadPool(loop)),
    messageCallback_(defaultMessageCallback),
    started_(false),
    nextEntityId_(1),
    codec_(boost::bind(&ComServer::onStringMessage, this, _1, _2, _3))
{
    setMessageCallback(
        boost::bind(&RS485Codec::onMessage, &codec_, _1, _2, _3));
}

ComServer::~ComServer()
{
    loop_->assertInLoopThread();
    LOG_TRACE << "ComServer::~ComServer [" << name_ << "] destructing";
    
    for (ComEntityMap::iterator itr(comEntities_.begin());
            itr != comEntities_.end();
            itr++)
    {
        ComEntityPtr com = itr->second;
        itr->second.reset();
        com->getLoop()->runInLoop(
            boost::bind(&ComEntity::comDestroyed, com));
        com.reset();
    }
}

void ComServer::createComEntityAndInsert(const string& entityName, unsigned char dataBits, unsigned long baudRate, ComEntity::eMBParity parity)
{
    loop_->assertInLoopThread();
    LOG_INFO << "Comserver::createComEntityAndInsert: name[" << entityName << "]";

    ComEntityMap::iterator itr = comEntities_.find(entityName);
    if (itr !=comEntities_.end())
    {
        LOG_INFO << "ComServer::insertComEntity: The Entity [ " << entityName << " ] is already contained in ComServer";
        return;
    }

    EventLoop* ioLoop = threadPool_->getNextLoop();
    nextEntityId_++;
    boost::shared_ptr<ComEntity> comEntity0(new ComEntity(ioLoop, "/dev/ttyS0", 8, 9600, ComEntity::MB_PAR_NONE));
    comEntity0->setMessageCallback(messageCallback_);
    comEntity0->setWriteCompleteCallback(writeCompleteCallback_);
    comEntities_[entityName] = comEntity0;
    
    ioLoop->runInLoop(boost::bind(&ComEntity::comEstablished, comEntity0));
}

void ComServer::setThreadNum(int numThreads)
{
    assert(0 <= numThreads);
    threadPool_->setThreadNum(numThreads);
}

void ComServer::start()
{
    if (!started_)
    {
        started_ = true;
        threadPool_->start(threadInitCallback_);
    }
}


