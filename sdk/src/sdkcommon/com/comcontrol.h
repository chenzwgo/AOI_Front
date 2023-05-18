#ifndef COMCONTROL_H
#define COMCONTROL_H
#include "sdkcommon/com/icom.h"
#include <string>
using namespace std;

class ComControl: public ICom
{
public:
    ComControl();
    ~ComControl();

    virtual bool OpenCom( void ) ;
    virtual bool CloseCom( void );
    virtual void SetComName(const char* pName);
    virtual int WriteCom( void* pmsg ,unsigned int msglen ) ;
    virtual int ReadCom( void* buffer ,unsigned int buffersize );
    virtual bool IsOpen( void );
    virtual bool FlushCom( void );
    virtual long GetHandle( void );

    virtual bool SetComState(unsigned long BaudRate,unsigned long Parity,unsigned long ByteSize,unsigned long StopBits);
    virtual bool GetComState(unsigned long &BaudRate,unsigned long &Parity,unsigned long &ByteSize,unsigned long &StopBits);

    virtual bool SetComState( void *totalState );
    virtual bool GetComState( void *totalState );

    // 超时的时间都是基于20微秒的
    virtual bool SetTimeOut( unsigned long ReadIntervalTimeout20us, unsigned long ReadTotalTimeoutMultiplier20us, unsigned long ReadTotalTimeoutConstant20us,
        unsigned long WriteTotalTimeoutMultiplier20us, unsigned long WriteTotalTimeoutConstant20us );
    virtual bool GetTimeOut( unsigned long &ReadIntervalTimeout20us, unsigned long &ReadTotalTimeoutMultiplier20us, unsigned long &ReadTotalTimeoutConstant20us,
        unsigned long &WriteTotalTimeoutMultiplier20us, unsigned long &WriteTotalTimeoutConstant20us );

private:
    void HandleError();
    bool m_bOpen;
    bool m_bError;
    long m_fd;
    string m_strComName;
};

#endif // COMCONTROL_H
