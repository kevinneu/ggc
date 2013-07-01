#ifndef _MB_ASCII_H
#define _MB_ASCII_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#if MB_ASCII_ENABLED > 0
eMBErrorCode    eMBASCIIInit( UCHAR slaveAddress, UCHAR ucPort,
                              ULONG ulBaudRate, eMBParity eParity );
void            eMBASCIIStart( void );
void            eMBASCIIStop( void );

eMBErrorCode    eMBASCIIReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame,
                                 USHORT * pusLength );
eMBErrorCode    eMBASCIISend( UCHAR slaveAddress, const UCHAR * pucFrame,
                              USHORT usLength );
BOOL            xMBASCIIReceiveFSM( void );
BOOL            xMBASCIITransmitFSM( void );
BOOL            xMBASCIITimerT1SExpired( void );
#endif

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
