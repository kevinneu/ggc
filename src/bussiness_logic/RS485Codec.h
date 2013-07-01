#ifndef __RS485CODEC_H_
#define __RS485CODEC_H_

#include <utils/Logging.h>
#include <port/Buffer.h>
//#include <Endian.h>
#include <ComEntity.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

class RS485Codec : boost::noncopyable
{
public:
    typedef boost::function<void (const dbdky::port::ComEntityPtr&, 
            const dbdky::string& message,
            dbdky::Timestamp)> StringMessageCallback;

    explicit RS485Codec(const StringMessageCallback& cb)
        : messageCallback_(cb)
    {
    }

    void onMessage(const dbdky::port::ComEntityPtr& com,
            dbdky::port::Buffer* buf,
            dbdky::Timestamp receiveTime)
    {
        //TODO:
        //We are here just to test this routine.
      
        LOG_INFO << "onMessage: [" << buf->retrieveAllAsString() << "]";
/*
        while (buf->readableBytes() >= kHeaderLen)
        {
            const void* data = buf->peek();
            int32_t be32 = *static_cast<const int32_t*>(data);
            const int32_t len = dbdky::port::networkToHost32(be32);
            if (len > 65536 || len < 0)
            {
                LOG_ERROR << "Invalid Length" << len;
                com->shutdown();
                break;
            }
            else if (buf->readableBytes() >= len + kHeaderLen)
            {
                buf->retrive(kHeaderLen);
                dbdky::string message(buf->peek(), len);
                messageCallback_(com, message, receiveTime);
                buf->retrieve(len);
            }
            else
            {
                break;
            }
        }
*/
    }

    void send(dbdky::port::ComEntity* com, const dbdky::StringPiece& message)
    {
        dbdky::port::Buffer buf;
        buf.append(message.data(), message.size());
        int32_t len = static_cast<int32_t>(message.size());
        int32_t be32 = dbdky::port::hostToNetwork32(len);
        buf.prepend(&be32, sizeof be32);
        com->send(&buf);
    }

private:
    StringMessageCallback messageCallback_;
    const static size_t kHeaderLen = sizeof(int32_t);
};



#endif
