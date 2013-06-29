#ifndef __POLLER_H_
#define __POLLER_H_

#include <vector>
#include <boost/noncopyable.hpp>

#include <utils/Timestamp.h>
#include <EventLoop.h>

namespace dbdky
{
    namespace port
    {
        class Poller : boost::noncopyable
        {
        public:
            typedef std::vector<Channel*> ChannelList;
    
            Poller(EventLoop* loop);
            virtual ~Poller();
   
            virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
            virtual void updateChannel(Channel* channel) = 0;
            virtual void removeChannel(Channel* channel) = 0;
            static Poller* newDefaultPoller(EventLoop* loop);
            void assertInLoopThread()
            {
                ownerLoop_->assertInLoopThread();
            }

        private:
            EventLoop* ownerLoop_;
        };
    }
}


#endif
