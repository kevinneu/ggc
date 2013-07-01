/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Variables ----------------------------------------*/
static eMBEventType eQueuedEvent;
static BOOL     xEventInQueue;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    xEventInQueue = FALSE;
    return TRUE;
}

BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    xEventInQueue = TRUE;
    eQueuedEvent = eEvent;
    return TRUE;
}

BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    BOOL            xEventHappened = FALSE;

    if( xEventInQueue )
    {
        *eEvent = eQueuedEvent;
        xEventInQueue = FALSE;
        xEventHappened = TRUE;
    }
    else
    {
        /* Poll the serial device. The serial device timeouts if no
         * characters have been received within for t3.5 during an
         * active transmission or if nothing happens within a specified
         * amount of time. Both timeouts are configured from the timer
         * init functions.
         */
        ( void )xMBPortSerialPoll(  );

        /* Check if any of the timers have expired. */
        vMBPortTimerPoll(  );

    }
    return xEventHappened;
}
