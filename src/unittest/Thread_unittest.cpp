#include <utils/Thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <boost/bind.hpp>

static void td_func()
{
    while (1)
    {
        printf("Thread.\n");
        sleep(1);
    }
}


int main(int argc, char** args)
{
    dbdky::Thread td(boost::bind(&td_func));
    td.start();
    return 0;
}
