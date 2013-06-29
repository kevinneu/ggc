#include <ComServer.h>

#include <utils/Logging.h>
#include <EventLoop.h>
#include <EventLoopThreadPool.h>

#include <boost/bind.hpp>

#include <stdio.h>

using namespace dbdky;
using namespace dbdky::port;

ComServer::ComServer(EventLoop* loop, const string& nameArg)
  : loop_(loop),
    name_(nameArg),
    threadPool_(new EventLoopThreadPool(loop)),
    connectionCallback_(defaultConnectionCallback),
    messageCallback_(defaultMessageCallback),
    started_(false),
    nextEntityId_(1)
{
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


