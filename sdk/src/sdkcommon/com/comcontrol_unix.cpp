#include "comcontrol.h"
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>

#include <iostream>
using namespace std;

ComControl::ComControl()
{
    m_strComName = "";
    m_fd = -1;
    m_bOpen = false;
}

ComControl::~ComControl()
{
    CloseCom();
    m_strComName = "";
    m_fd = -1;
    m_bOpen = false;
}

bool ComControl::OpenCom()
{
    if(m_bOpen)
    {
        return true;
    }

    m_fd = open(m_strComName.c_str(),O_RDWR);
    if(m_fd == -1)
    {
        return false;
    }

    int n = fcntl(m_fd, F_GETFL, 0);
    fcntl(m_fd, F_SETFL, n & ~O_NDELAY);

    m_bOpen = true;
    return true;
}

bool ComControl::CloseCom()
{
    if(!m_bOpen)
    {
        return true;
    }
    close(m_fd);
    m_fd = -1;
    m_bOpen = false;
}

void ComControl::SetComName(const char *pName)
{
    if (pName == NULL || m_bOpen)
    {
        return;
    }
    m_strComName = pName;
}

int ComControl::WriteCom(void *pmsg, unsigned int msglen)
{
    if(!IsOpen() || NULL == pmsg || m_fd == -1)
    {
        return 0;
    }

   int ilength =  write(m_fd, pmsg, msglen);
   if(ilength  == -1)
   {
       return -1;
   }
   return ilength;
}

int ComControl::ReadCom(void *buffer, unsigned int buffersize)
{
    if(!IsOpen() || NULL == buffer || m_fd == -1)
    {
        return -1;
    }

    int ilength =  read(m_fd, buffer, (size_t)buffersize);
    if(ilength  == -1)
    {
        return -1;
    }
    return ilength;
}

bool ComControl::IsOpen()
{
    return m_bOpen;
}

bool ComControl::FlushCom()
{
    if(!IsOpen()  || m_fd == -1)
    {
        return false;
    }
    if(tcflush(m_fd, TCIOFLUSH) == -1)
    {
        m_bError = true;
        return false;
    }
    return true;
}


bool ComControl::SetComState(unsigned long BaudRate, unsigned long Parity, unsigned long ByteSize, unsigned long StopBits)
{
    struct termios newtio;
    if(!IsOpen()  || m_fd == -1)
    {
        return false;
    }

    if (tcgetattr(m_fd, &newtio)!=0)
    {
        return false;
    }

    speed_t _baud=0;
    switch (BaudRate)
    {
    #ifdef B0
    case      0: _baud=B0;     break;
    #endif
    #ifdef B50
    case     50: _baud=B50;    break;
    #endif
    #ifdef B75
    case     75: _baud=B75;    break;
    #endif
    #ifdef B110
    case    110: _baud=B110;   break;
    #endif
    #ifdef B134
    case    134: _baud=B134;   break;
    #endif
    #ifdef B150
    case    150: _baud=B150;   break;
    #endif
    #ifdef B200
    case    200: _baud=B200;   break;
    #endif
    #ifdef B300
    case    300: _baud=B300;   break;
    #endif
    #ifdef B600
    case    600: _baud=B600;   break;
    #endif
    #ifdef B1200
    case   1200: _baud=B1200;  break;
    #endif
    #ifdef B1800
    case   1800: _baud=B1800;  break;
    #endif
    #ifdef B2400
    case   2400: _baud=B2400;  break;
    #endif
    #ifdef B4800
    case   4800: _baud=B4800;  break;
    #endif
    #ifdef B7200
    case   7200: _baud=B7200;  break;
    #endif
    #ifdef B9600
    case   9600: _baud=B9600;  break;
    #endif
    #ifdef B14400
    case  14400: _baud=B14400; break;
    #endif
    #ifdef B19200
    case  19200: _baud=B19200; break;
    #endif
    #ifdef B28800
    case  28800: _baud=B28800; break;
    #endif
    #ifdef B38400
    case  38400: _baud=B38400; break;
    #endif
    #ifdef B57600
    case  57600: _baud=B57600; break;
    #endif
    #ifdef B76800
    case  76800: _baud=B76800; break;
    #endif
    #ifdef B115200
    case 115200: _baud=B115200; break;
    #endif
    #ifdef B128000
    case 128000: _baud=B128000; break;
    #endif
    #ifdef B230400
    case 230400: _baud=B230400; break;
    #endif
    #ifdef B460800
    case 460800: _baud=B460800; break;
    #endif
    #ifdef B576000
    case 576000: _baud=B576000; break;
    #endif
    #ifdef B921600
    case 921600: _baud=B921600; break;
    #endif
    default:
     break;
    }
    cfsetospeed(&newtio, (speed_t)_baud);
    cfsetispeed(&newtio, (speed_t)_baud);

    /* We generate mark and space parity ourself. */
    if (ByteSize == 7 && (Parity==3 || Parity == 4))
    {
       ByteSize = 8;
    }
    switch (ByteSize)
    {
        case 5:
           newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS5;
           break;
        case 6:
           newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS6;
           break;
        case 7:
           newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS7;
           break;
        case 8:
        default:
           newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS8;
           break;
    }
    newtio.c_cflag |= CLOCAL | CREAD;

    //parity
    newtio.c_cflag &= ~(PARENB | PARODD);
    if (Parity ==0)
    {
        newtio.c_cflag &= ~(PARENB | PARODD);
    }
    else if (Parity== 1)
    {
       newtio.c_cflag |= (PARENB | PARODD);
    }
    else if (Parity == 2)
    {
       newtio.c_cflag |= PARENB;
    }


    newtio.c_cflag &= ~CRTSCTS;

    //stopbits
    if (StopBits==2)
    {
       newtio.c_cflag |= CSTOPB;
    }
    else
    {
       newtio.c_cflag &= ~CSTOPB;
    }

    newtio.c_iflag=IGNBRK;

    //software handshake
    bool softwareHandshake = false;
    if (softwareHandshake)
    {
       newtio.c_iflag |= IXON | IXOFF;
    }
    else
    {
       newtio.c_iflag &= ~(IXON|IXOFF|IXANY);
    }

    newtio.c_lflag=0;
    newtio.c_oflag=0;

    newtio.c_cc[VTIME]=1;
    newtio.c_cc[VMIN]=60;


    if (tcsetattr(m_fd, TCSANOW, &newtio)!=0)
    {
        return false;
    }

    int mcs=0;
    ioctl(m_fd, TIOCMGET, &mcs);
    mcs |= TIOCM_RTS;
    ioctl(m_fd, TIOCMSET, &mcs);

    if (tcgetattr(m_fd, &newtio)!=0)
    {
        return false;
    }

    //hardware handshake
    bool hardwareHandshake = false;
    if (hardwareHandshake)
    {
       newtio.c_cflag |= CRTSCTS;
    }
    else
    {
       newtio.c_cflag &= ~CRTSCTS;
    }

    if (tcsetattr(m_fd, TCSANOW, &newtio)!=0)
    {
        return false;
    }

    return true;
}

bool ComControl::GetComState(unsigned long &BaudRate, unsigned long &Parity, unsigned long &ByteSize, unsigned long &StopBits)
{
    struct termios newtio;
    if(!IsOpen()  || m_fd == -1)
    {
        return false;
    }

    if (tcgetattr(m_fd, &newtio)!=0)
    {
        return false;
    }
    /* ByteSize Number of bits/byte, 4-8        */
    /* Parity  0-4=None,Odd,Even,Mark,Space    */
    /* StopBits 0,1,2 = 1, 1.5, 2               */
    BaudRate = newtio.c_ispeed;
    if((newtio.c_cflag & ~CSIZE) & CS5)
    {
            ByteSize = 5;
    }
    else     if((newtio.c_cflag & ~CSIZE) & CS6)
    {
            ByteSize = 6;
    }
    else     if((newtio.c_cflag & ~CSIZE) & CS7)
    {
            ByteSize = 7;
    }
    else     if((newtio.c_cflag & ~CSIZE) & CS8)
    {
            ByteSize = 8;
    }

    if(newtio.c_cflag & (0|PARENB))
    {
        Parity = 0;
    }
    else if(newtio.c_cflag &(0| PARENB | PARODD))
    {
        Parity = 1;
    }
    else if(newtio.c_cflag &(0| PARENB ))
    {

        Parity =2;
    }

    if(newtio.c_cflag &(0| CSTOPB ))
    {

        StopBits = 2;
    }

    return true;
}


long ComControl::GetHandle( void )
{
    return m_fd;
}
bool ComControl::SetComState( void *totalState )
{
    return true;
}
bool ComControl::GetComState( void *totalState )
{
    return true;
}

// 超时的时间都是基于20微秒的
bool ComControl::SetTimeOut( unsigned long ReadIntervalTimeout20us, unsigned long ReadTotalTimeoutMultiplier20us, unsigned long ReadTotalTimeoutConstant20us,
    unsigned long WriteTotalTimeoutMultiplier20us, unsigned long WriteTotalTimeoutConstant20us )
{
    return true;
}
bool ComControl::GetTimeOut( unsigned long &ReadIntervalTimeout20us, unsigned long &ReadTotalTimeoutMultiplier20us, unsigned long &ReadTotalTimeoutConstant20us,
    unsigned long &WriteTotalTimeoutMultiplier20us, unsigned long &WriteTotalTimeoutConstant20us )
{
    return true;
}
