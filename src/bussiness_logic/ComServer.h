#ifndef __COMSERVER_H_
#define __COMSERVER_H_

#include <utils/Types.h>
#include <ComEntity.h>

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <RS485Codec.h>

namespace dbdky
{
    namespace port
    {
        class EventLoop;
        class EventLoopThreadPool;

        class ComServer : boost::noncopyable
        {
        public:
            typedef boost::function<void(EventLoop*)> ThreadInitCallback;
            ComServer(EventLoop* loop, const string& nameArg);
            ~ComServer();

            const string& name() const { return name_; }
            void setThreadNum(int numThreads);
            void setThreadInitCallback(const ThreadInitCallback& cb)
            {
                threadInitCallback_ = cb;
            }

            void start();

            void setMessageCallback(const MessageCallback& cb)
            {
                messageCallback_ = cb;
            }

            void setWriteCompleteCallback(const WriteCompleteCallback& cb)
            {
                writeCompleteCallback_ = cb;
            }

            void onMessage(const ComEntityPtr& com , Buffer* buf, Timestamp time)
            {
            }

            void onStringMessage(const ComEntityPtr&, const string& message, Timestamp)
            {
/*
                for (ComEntityList::iterator itr = comEntities_.begin();
                    itr != comEntities_.end();
                    itr++)
                {
                    codec_.send(get_pointer(*itr), message);
                }
*/
            }

            void createComEntityAndInsert(const string& name, unsigned char dataBits, unsigned long baudRate, ComEntity::eMBParity parity);

        private:
            typedef std::map<string, ComEntityPtr> ComEntityMap;
            
            EventLoop* loop_;
            const string name_;
            boost::scoped_ptr<EventLoopThreadPool> threadPool_;
            MessageCallback messageCallback_;
            WriteCompleteCallback writeCompleteCallback_;
            ThreadInitCallback threadInitCallback_;
            bool started_;
            int nextEntityId_;
            ComEntityMap comEntities_;
            RS485Codec codec_;
        };
    }
}

#endif
