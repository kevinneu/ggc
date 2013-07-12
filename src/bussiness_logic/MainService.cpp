#include <ComEntity.h>

#include <utils/Logging.h>
#include <EventLoop.h>

#include <string>

#include <ComServer.h>
#include <boost/shared_ptr.hpp>
#include <ConfigMgr.h>

using namespace dbdky;
using namespace dbdky::port;

int main(int argc, char** argv)
{
    LOG_INFO << "pid = " << getpid();
   
#if 0 
    EventLoop loop;

    ComServer server(&loop, "ggc");

    server.createComEntityAndInsert("/dev/ttyS0", 8, 115200, ComEntity::MB_PAR_NONE);
    server.start();

    loop.loop();
#endif

    ConfigMgr mgr("/home/kevin/workspace/ggc/dat");
    mgr.update();
    return 0;
}
