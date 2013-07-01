#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

#include "mb.h"
#include "mbport.h"

bool bSetSignal(int iSignalNr, void(*pSigHandler)(int))
{
    bool bResult;
    struct sigaction xNewSig, xOldSig;
    
    xNewSig.sa_handler = pSigHandler;
    sigemptyset(&xNewSig.sa_mask);
    xNewSig.sa_flags = 0;
    if (sigaction(iSignalNr, &xNewSig, &xOldSig) != 0)
    {
        bResult = false;
    }
    else
    {
        bResult = true;
    }

    return bResult;
}

void vSigShutdown(int xSigNr)
{
    switch (xSigNr)
    {
        case SIGQUIT:
        case SIGINT:
        case SIGTERM:
	{
            break;
	}
    }
}


int main(int argc, char** argv)
{
    int iExitCode;
    UCHAR cCh;
  
    const UCHAR ucSlaveID[] = {0xAA, 0xBB, 0xCC};
 
    return iExitCode;
}
