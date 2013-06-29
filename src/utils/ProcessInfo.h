#ifndef __PROCESSINFO_H_
#define __PROCESSINFO_H_

#include <utils/Types.h>
#include <utils/Timestamp.h>
#include <vector>

namespace dbdky
{
    namespace ProcessInfo
    {
        pid_t pid();
        string pidString();
        uid_t uid();
        string username();
        uid_t euid();
        Timestamp startTime();

        string hostname();

        string procStatus();

        int openedFiles();
        int maxOpenFiles();

        int numThreads();
        std::vector<pid_t> threads();
    }
}


#endif
