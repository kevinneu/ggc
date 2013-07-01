/* ----------------------- Standard includes --------------------------------*/
#include <stdlib.h>
#include <sys/time.h>

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
ULONG           ulTimeOut;
BOOL            bTimeoutEnable;

static struct timeval xTimeLast;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    ulTimeOut = usTim1Timerout50us / 20U;
    if( ulTimeOut == 0 )
        ulTimeOut = 1;

    return xMBPortSerialSetTimeout( ulTimeOut );
}

void
xMBPortTimersClose(  )
{
    /* Does not use any hardware resources. */
}

void
vMBPortTimerPoll(  )
{
    ULONG           ulDeltaMS;
    struct timeval  xTimeCur;

    /* Timers are called from the serial layer because we have no high
     * res timer in Win32. */
    if( bTimeoutEnable )
    {
        if( gettimeofday( &xTimeCur, NULL ) != 0 )
        {
            /* gettimeofday failed - retry next time. */
        }
        else
        {
            ulDeltaMS = ( xTimeCur.tv_sec - xTimeLast.tv_sec ) * 1000L +
                ( xTimeCur.tv_usec - xTimeLast.tv_usec ) * 1000L;
            if( ulDeltaMS > ulTimeOut )
            {
                bTimeoutEnable = FALSE;
                ( void )pxMBPortCBTimerExpired(  );
            }
        }
    }
}

void
vMBPortTimersEnable(  )
{
    int             res = gettimeofday( &xTimeLast, NULL );

    assert( res == 0 );
    bTimeoutEnable = TRUE;
}

void
vMBPortTimersDisable(  )
{
    bTimeoutEnable = FALSE;
}
