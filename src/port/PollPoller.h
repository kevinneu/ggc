#ifndef __POLLPOLLER_H_
#define __POLLPOLLER_H_

#include <Poller.h>

#include <map>
#include <vector>

struct pollfd;

namespace dbdky
{
    namespace port
    {
        class PollPoller : public Poller
        {
        public:
            PollPoller(EventLoop* loop);
            virtual ~PollPoller();

            virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels);
            virtual void updateChannel(Channel* channel);
            virtual void removeChannel(Channel* channel);

        private:
            void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

            typedef std::vector<struct pollfd> PollFdList;
            typedef std::map<int, Channel*> ChannelMap;
          
            PollFdList pollfds_;
            ChannelMap channels_;
        };
    }
}

#endif
