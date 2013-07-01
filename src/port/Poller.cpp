#include <Poller.h>

using namespace dbdky;
using namespace dbdky::port;

Poller::Poller(EventLoop* loop)
  : ownerLoop_(loop)
{
}

Poller::~Poller()
{
}
