#include <Poller.h>
#include <PollPoller.h>
#include <EPollPoller.h>

#include <stdlib.h>

using namespace dbdky::port;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    if (::getenv("DBDKY_USE_POLL"))
    {
        return new PollPoller(loop);
    }
    else
    {
        return new EPollPoller(loop);
    }
}
