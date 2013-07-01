/* ----------------------- Standard includes --------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"

/* ----------------------- Defines ------------------------------------------*/
#define NELEMS( x ) ( sizeof( ( x ) )/sizeof( ( x )[0] ) )

/* ----------------------- Static variables ---------------------------------*/
static FILE    *fLogFile = NULL;
static eMBPortLogLevel eLevelMax = MB_LOG_DEBUG;
static pthread_mutex_t xLock = PTHREAD_MUTEX_INITIALIZER;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortLogLevel( eMBPortLogLevel eNewLevelMax )
{
    eLevelMax = eNewLevelMax;
}

void
vMBPortLogFile( FILE * fNewLogFile )
{
    fLogFile = fNewLogFile;
}

void
vMBPortLog( eMBPortLogLevel eLevel, const CHAR * szModule, const CHAR * szFmt, ... )
{
    CHAR            szBuf[512];
    int             i;
    va_list         args;
    FILE           *fOutput = fLogFile == NULL ? stderr : fLogFile;

    static const char *arszLevel2Str[] = { "ERROR", "WARN", "INFO", "DEBUG" };

    i = snprintf( szBuf, NELEMS( szBuf ), "%s: %s: ", arszLevel2Str[eLevel], szModule );

    if( i != 0 )
    {
        va_start( args, szFmt );
        i += vsnprintf( &szBuf[i], NELEMS( szBuf ) - i, szFmt, args );
        va_end( args );
    }

    if( i != 0 )
    {
        if( eLevel <= eLevelMax )
        {
            fputs( szBuf, fOutput );
        }
    }
}

void
vMBPortEnterCritical( void )
{
    if( pthread_mutex_lock( &xLock ) != 0 )
    {
        vMBPortLog( MB_LOG_ERROR, "OTHER", "Locking primitive failed: %s\n", strerror( errno ) );
    }
}

void
vMBPortExitCritical( void )
{
    if( pthread_mutex_unlock( &xLock ) != 0 )
    {
        vMBPortLog( MB_LOG_ERROR, "OTHER", "Locking primitive failed: %s\n", strerror( errno ) );
    }
}
