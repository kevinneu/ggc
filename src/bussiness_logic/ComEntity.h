#ifndef __COMENTITY_H_
#define __COMENTITY_H_

#include <port/Buffer.h>
#include <utils/Mutex.h>
#include <utils/StringPiece.h>
#include <utils/Types.h>
#include <utils/Logging.h>

#include <port/Callbacks.h>


#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace dbdky
{
    namespace port
    {
        class Channel;
        class EventLoop;
   
        class ComEntity : boost::noncopyable,
            public boost::enable_shared_from_this<ComEntity>
        {
        public:
            typedef enum
            {
                MB_PAR_NONE,
                MB_PAR_ODD,
                MB_PAR_EVEN
            } eMBParity;

            typedef enum 
            {
                kIdle = 0,
                kOpened,
                kError
            } ComState;

        public:
            ComEntity(EventLoop* loop,
                        const string& portName, 
			unsigned char dataBits,
			unsigned long baudRate,
			eMBParity parity);

            virtual ~ComEntity();

            EventLoop* getLoop() const { return loop_; }
            const string& getPortName() const { return comutil_.portName_;}

            void start();
            void stop();

            void sendInLoop(const StringPiece& message);
            void sendInLoop(const void* message, size_t len);
            
            void shutdownInLoop();
            void setState(ComState s) { state_ = s; }

            void send(const void* message, size_t len);
            void send(const StringPiece& message);
            void send(Buffer* message);
            void shutdown();
            void comDestroyed(); 

            void setMessageCallback(const MessageCallback& cb)
            {
                messageCallback_ = cb;
            }

            void setWriteCompleteCallback(const WriteCompleteCallback& cb)
            {
                writeCompleteCallback_ = cb;
            }
           
            void comEstablished();

            void handleRead(Timestamp);
            void handleWrite();
            void handleError();
            void handleClose();

	 private:
	     class ComUtil
             {
             friend class ComEntity;
             public:
                 ComUtil(const string& portName,
			unsigned long baudRate,
			unsigned char dataBits,
			eMBParity eParity)
			: portName_(portName),
			  baudRate_(baudRate),
			  dataBits_(dataBits),
			  eParity_(eParity),
                          comFd_(-1),
			  bRxEnabled_(false),
                          bTxEnabled_(false)
			{
                            LOG_INFO << "ComEntity::ComUtil()";
			    if (!serialInit())
                            {
                                comFd_ = -1;
                            }
			}
                 ~ComUtil(){}
             private:
                 bool serialInit();
		 bool serialEnable(bool bEnableRx, bool bEnableTx);
		 void serialClose();
                 bool serialRead(unsigned char* pucBuffer, unsigned short usNBytes, unsigned short* usNBytesRead);
                 bool serialWrite(unsigned char* pucBuffer, unsigned short usNBytes);

             private:
                 string portName_;
                 unsigned long baudRate_;
                 unsigned char dataBits_;
                 eMBParity eParity_;
                 int comFd_;

                 bool bRxEnabled_;
                 bool bTxEnabled_;
             } comutil_;
   
         private:
            EventLoop* loop_;
            ComState state_;
            boost::scoped_ptr<Channel> channel_;
            MessageCallback messageCallback_;
            WriteCompleteCallback writeCompleteCallback_;
            CloseCallback closeCallback_;
            Buffer inputBuffer_;
            Buffer outputBuffer_;

        };
    }
}

#endif
