#include <ComEntity.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <Channel.h>
#include <EventLoop.h>

#include <boost/bind.hpp>

namespace dbdky
{
    namespace port
    {
        static struct termios xNewTIO;
        static struct termios xOldTIO;
    }
}

using namespace dbdky;
using namespace dbdky::port;

void dbdky::port::defaultConnectionCallback(const ComEntityPtr& conn)
{
    LOG_TRACE << "dbdky::port::defaultConnectionCallback";
}

void dbdky::port::defaultMessageCallback(const ComEntityPtr&,
                                Buffer* buf,
                                Timestamp receiveTime)
{
    buf->retrieveAll();
}

void ComEntity::ComUtil::serialClose()
{
    if (comFd_ != -1)
    {
        ::tcsetattr(comFd_, TCSANOW, &xOldTIO);
        ::close(comFd_);
    }
}

bool ComEntity::ComUtil::serialEnable(bool bEnableRx, bool bEnableTx)
{
    //TODO:
}

bool ComEntity::ComUtil::serialRead(unsigned char* pucBuffer, unsigned short usNBytes, unsigned short* usNBytesRead)
{
    bool bResult = true;
    ssize_t res;
    fd_set rfds;
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 50000;
    
    FD_ZERO(&rfds);
    
    #pragma GCC diagnostic ignored "-Wold-style-cast"
    FD_SET(comFd_, &rfds);

    do
    {
        if (::select(comFd_ + 1, &rfds, NULL, NULL, &tv) == -1)
        {
            if (errno != EINTR)
            {
                bResult = false;
            }
        }
        else if (FD_ISSET(comFd_, &rfds))
        {

            if ((res = ::read(comFd_, pucBuffer, usNBytes)) == -1)
            {
                bResult = false;
            }
            else
            {
                *usNBytesRead = (unsigned short)res;
                break;
            }
        }
        else
        {
            *usNBytesRead = 0;
            break;
        }
    } while(bResult == true);

    return bResult;
}

bool ComEntity::ComUtil::serialWrite(unsigned char* pucBuffer, unsigned short usNBytes)
{
    ssize_t res;
    size_t left = (size_t)usNBytes;
    size_t done = 0;

    while (left > 0)
    {
        if (( res = ::write(comFd_, pucBuffer + done, left)) == -1)
        {
            if (errno != EINTR)
            {
                break;
            }

            continue;
        }

        done += res;
        left -= res;
    }

    return left == 0 ? true : false;
}

bool ComEntity::ComUtil::serialInit()
{
    bool bStatus = true;
    speed_t xNewSpeed;

    if ((comFd_ = ::open(portName_.c_str(), O_RDWR | O_NOCTTY)) < 0)
    {
        LOG_SYSERR << "SER-INIT: Can't open serial"; 
        bStatus = false;
        return bStatus;
    }

    if (::tcgetattr(comFd_, &xOldTIO) != 0)
    {
        LOG_SYSERR << "SER-INIT: Can't get settings from port";
        ::close(comFd_);
        bStatus = false;
        return bStatus;
    }

    bzero(&xNewTIO, sizeof(struct termios));

    xNewTIO.c_iflag |= IGNBRK | INPCK;
    xNewTIO.c_cflag |= CREAD | CLOCAL;
    switch (eParity_)
    {
        case MB_PAR_NONE:
        {
            break;
        }
        case MB_PAR_EVEN:
        {
            xNewTIO.c_cflag |= PARENB;
            break;
        }
        case MB_PAR_ODD:
        {
            xNewTIO.c_cflag |= PARENB | PARODD;
            break;                
        }
        default:
        {
            bStatus = false;
        }
    }

        switch (dataBits_)
        {
            case 8:
            {
                xNewTIO.c_cflag |= CS8;
                break;
            }
            case 7:
            {
                xNewTIO.c_cflag |= CS7;
                break;
            }
            default:
            {
                bStatus = false;
        }       
    }

    switch (baudRate_)
    {
        case 9600:
        {
            xNewSpeed = B9600;
            break;
        }
        case 19200:
        {
            xNewSpeed = B19200;
            break;
        }
        case 38400:
        {
            xNewSpeed = B38400;
            break;
        }
        case 57600:
        {
            xNewSpeed = B57600;
            break;        
        }
        case 115200:
        {
            xNewSpeed = B115200;
            break; 
        }
        default:
        {
            bStatus = false;
        }      
    }

    if (bStatus)
    {
        if (::cfsetispeed(&xNewTIO, xNewSpeed) != 0)
        {
            LOG_SYSERR << "SER-INIT: Can't set baud rate";
        }
        else if (::cfsetospeed(&xNewTIO, xNewSpeed) != 0)
        {
            LOG_SYSERR << "SER-INIT: Can't set baud rate";
        }
        else if (::tcsetattr(comFd_, TCSANOW, &xNewTIO) != 0)
        {
            LOG_SYSERR << "SER-INIT: Can't set settings";
        }
        else
        {
            serialEnable(false, false);
            bStatus = true;
        }
    }

    return bStatus;
}

ComEntity::ComEntity(EventLoop* loop,
        const string& portName,
	uint8_t dataBits,
	unsigned long baudRate,
	eMBParity parity)
  : state_(kIdle),
    loop_(loop),
    comutil_(portName, baudRate, dataBits, parity),
    channel_(new Channel(loop, comutil_.comFd_))
    
{
    LOG_INFO << "ComEntity::ComEntity()";
    if (channel_->fd() >= 0)
    {
        LOG_INFO << "ComEntity::ComEntity(), open serial OK. fd[" << channel_->fd() << "]";
        state_ = kOpened;
        channel_->setReadCallback(
            boost::bind(&ComEntity::handleRead, this, _1));
        channel_->setWriteCallback(
            boost::bind(&ComEntity::handleWrite, this));
       
    }
    else
    {
        LOG_INFO << "ComEntity::ComEntity(), open serial FAIL.";
        state_ = kError;
    }
}

ComEntity::~ComEntity()
{
}

void ComEntity::handleRead(Timestamp receiveTime)
{
    loop_->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    LOG_INFO << "ComEntity::handleRead: read length[" << n << "]";
    if (n > 0)
    {
        if (messageCallback_)
        {
            messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
        }
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedErrno;
        LOG_SYSERR << "ComEntity::handleRead";
        handleError();
    }
}

void ComEntity::handleWrite()
{
}

            
void ComEntity::handleError()
{
}
            
void ComEntity::handleClose()
{
}

void ComEntity::sendInLoop(const StringPiece& message)
{
    sendInLoop(message.data(), message.size());   
}

void ComEntity::sendInLoop(const void* data, size_t len)
{
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool error = false;
    if (state_ == kOpened)
    {
        LOG_WARN << "com not opened yet, give up writing";
        return;
    }

    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
    {
        nwrote = ::write(channel_->fd(), data, len);
        if (nwrote >= 0)
        {
            remaining = len - nwrote;
            if (remaining == 0 && writeCompleteCallback_)
            {
                loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
            }
        }
        else
        {
            nwrote = 0;
            if (errno != EWOULDBLOCK)
            {
                LOG_SYSERR << "ComEntity::sendInLoop";
                if (errno == EPIPE)
                {
                    error = true;
                }
            }
        }
    }   
}

void ComEntity::shutdown()
{
    if (state_ == kOpened)
    {
        setState(kIdle);
        loop_->runInLoop(boost::bind(&ComEntity::shutdownInLoop, this));
    }
}

void ComEntity::shutdownInLoop()
{
    loop_->assertInLoopThread();
    //TODO:
}

void ComEntity::send(const void* data, size_t len)
{
    if (state_ == kOpened)
    {
        if (loop_->isInLoopThread())
        {
            sendInLoop(data, len);
        }
        else
        {
            string message(static_cast<const char*>(data), len);
            loop_->runInLoop(boost::bind(&ComEntity::sendInLoop, this, message));
        }
    }
}

void ComEntity::send(const StringPiece& message)
{
    if (state_ == kOpened)
    {
        if (loop_->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            loop_->runInLoop(boost::bind(&ComEntity::sendInLoop, this, message.as_string()));
        }
    }
}

void ComEntity::send(Buffer* buf)
{
    if (state_ == kOpened)
    {
        if (state_ == kOpened)
        {
            if (loop_->isInLoopThread())
            {
                sendInLoop(buf->peek(), buf->readableBytes());
                buf->retrieveAll();
            }
            else
            {
                loop_->runInLoop(boost::bind(&ComEntity::sendInLoop, this, buf->retrieveAllAsString()));
            }
        }
    }
}

void ComEntity::start()
{
    //TODO:
}

void ComEntity::stop()
{
    //TODO:
}

void ComEntity::comDestroyed()
{
    //TODO:
    
}

void ComEntity::comEstablished()
{
    //TODO:
    LOG_INFO << "ComEntity::comEstablished";
    loop_->assertInLoopThread();
    assert(state_ == kOpened);
    channel_->tie(shared_from_this());
    channel_->enableReading();
}

