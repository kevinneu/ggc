#include <ComEntity.h>

#include <utils/Logging.h>
#include <EventLoop.h>

#include <string>

using namespace dbdky;
using namespace dbdky::port;

int main(int argc, char** argv)
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    //ComEntity(&loop, "/dev/ttyS0", 8, 9600, ComEntity::MB_PAR_NONE);


    //ComServer.start();
    loop.loop();
    return 0;
}
