#include "comcontrol.h"
#include <Windows.h>

#define MAX_DATA_SIZE_B 4096
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
    if (m_bOpen)
    {
        return true;
    }
    if (m_strComName == "" || m_strComName.length() == 0)
    {
        return false;
    }

    HANDLE hComHandle = NULL;
    //*_com_handle = INVALID_HANDLE_VALUE;
	string strComName = m_strComName;
    if (m_strComName.length() > 4)
    {
        strComName = "\\\\.\\" + m_strComName;
    }

    hComHandle = CreateFileA(
        m_strComName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL ,
        NULL
        );
    if( INVALID_HANDLE_VALUE == hComHandle )
    {
        return false;
    }

    //设置读写缓冲区大小
    if (!::SetupComm(hComHandle, MAX_DATA_SIZE_B, MAX_DATA_SIZE_B))
    {
        return false;
    }
    //设置超时
    COMMTIMEOUTS CTO;
    if (!::GetCommTimeouts(hComHandle, &CTO))
    {
        return false;
    }

    CTO.ReadIntervalTimeout = MAXDWORD;
    CTO.ReadTotalTimeoutConstant = 0;
    CTO.ReadTotalTimeoutMultiplier = 0;
    CTO.WriteTotalTimeoutMultiplier = 10;
    CTO.WriteTotalTimeoutConstant = 1000;
    if (!::SetCommTimeouts(hComHandle, &CTO))
    {
        return false;
    }

    if ( !::SetCommMask(hComHandle, EV_RXCHAR))
    {
        return false;
    }
    // 清空缓冲区
    if (!::PurgeComm(hComHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
    {
        return false;
    }
    m_bOpen = true;
    m_fd = (long)hComHandle;
    return true;
}

bool ComControl::CloseCom()
{
    if (!m_bOpen)
    {
        return true;
    }

    HANDLE hComHandle = NULL;
    hComHandle = (HANDLE)m_fd;
    if ( INVALID_HANDLE_VALUE != hComHandle )
    {
        ::PurgeComm(hComHandle, PURGE_TXABORT | PURGE_TXCLEAR);
        CloseHandle(hComHandle );
    }
    m_bOpen = false;
    m_fd = -1;
    return true;
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
    if(!IsOpen() || NULL == pmsg)
    {
        return 0;
    }

    DWORD    error;
    DWORD dwDataLen = 0;

    HANDLE hComHandle = NULL;
    hComHandle = (HANDLE)m_fd;

    if( INVALID_HANDLE_VALUE == hComHandle )
    {
        return false;
    }

    //清除错误
    if(ClearCommError(hComHandle, &error, NULL) && error > 0)
    {
        PurgeComm(hComHandle, PURGE_TXABORT | PURGE_TXCLEAR);
    }

    if(!WriteFile(hComHandle, pmsg, msglen, &dwDataLen, NULL))
    {
        DWORD derror = GetLastError();
        HandleError();
        return 0;
    }

    return (int)dwDataLen;
}

int ComControl::ReadCom(void *buffer, unsigned int buffersize)
{
    if(!IsOpen() || NULL == buffer)
    {
        return -1;
    }

    HANDLE hComHandle = NULL;
    hComHandle = (HANDLE)m_fd;

    if( INVALID_HANDLE_VALUE == hComHandle )
    {
        return false;
    }
    COMSTAT  stat;
    DWORD error;
    DWORD dwDataLen = 0;

    if(ClearCommError(hComHandle, &error, &stat) && error > 0)    //清除错误
    {
        PurgeComm(hComHandle, PURGE_RXABORT | PURGE_RXCLEAR); /*清除输入缓冲区*/
        return 0;
    }

    if(!ReadFile(hComHandle, buffer, buffersize,&dwDataLen, NULL))
    {
        DWORD derror = GetLastError();
        HandleError();
        return 0;
    }
    return (int)dwDataLen;
}

bool ComControl::IsOpen()
{
    return m_bOpen;
}

bool ComControl::FlushCom()
{

    if(!IsOpen())
    {
        return false;
    }

    HANDLE hComHandle = NULL;
    hComHandle = (HANDLE)m_fd;

    if( INVALID_HANDLE_VALUE == hComHandle )
    {
        return false;
    }

    if (!::PurgeComm(hComHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
    {
        return false;
    }
    return true;
}


/**
EVENPARITY            Even
MARKPARITY            Mark
NOPARITY              No parity
ODDPARITY             Odd
SPACEPARITY           Space


ONESTOPBIT 1 stop bit
ONE5STOPBITS 1.5 stop bits
TWOSTOPBITS 2 stop bits

*/
bool ComControl::SetComState(unsigned long BaudRate, unsigned long Parity, unsigned long ByteSize, unsigned long StopBits)
{
    if(!IsOpen())
    {
        return false;
    }

    HANDLE hComHandle = NULL;
    hComHandle = (HANDLE)m_fd;

    if( INVALID_HANDLE_VALUE == hComHandle )
    {
        return false;
    }

    DCB dcb;
    if( !::GetCommState(hComHandle, &dcb))
    {
        return false;
    }
    
    dcb.BaudRate     = BaudRate;        // 波特率
    dcb.Parity       = Parity;          // 校验偶
    dcb.ByteSize     = ByteSize;        // 数据位字节
	switch(StopBits)
	{
		case 1: dcb.StopBits = ONESTOPBIT; break;
		case 2: dcb.StopBits = TWOSTOPBITS; break;
		case 3: dcb.StopBits = ONE5STOPBITS;break;
	}
    dcb.fDsrSensitivity = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fOutX        = FALSE;
    dcb.fInX         = FALSE;    
    dcb.fParity      = (Parity == NOPARITY ? 0 : 1);

    if( !::SetCommState(hComHandle, &dcb))
    {
		DWORD dstatus = GetLastError();
        return false;
    }
    return true;
}

bool ComControl::GetComState(unsigned long &BaudRate, unsigned long &Parity, unsigned long &ByteSize, unsigned long &StopBits)
{
    if(!IsOpen())
    {
        return false;
    }

    HANDLE hComHandle = NULL;
    hComHandle = (HANDLE)m_fd;

    if( INVALID_HANDLE_VALUE == hComHandle )
    {
        return false;
    }

    DCB dcb;
    if( !::GetCommState(hComHandle, &dcb))
    {
        return false;
    }
    BaudRate = dcb.BaudRate  ;        // 波特率
    Parity =dcb.Parity ;              // 校验偶
    ByteSize =dcb.ByteSize  ;         // 数据位字节
    StopBits =dcb.StopBits ;          // 停止位
    return true;
}

void ComControl::HandleError()
{
    CloseCom();
}

long ComControl::GetHandle()
{
    return m_fd;
}

bool ComControl::SetComState( void * totalState )
{
    HANDLE hComHandle = (HANDLE)m_fd;
    DCB * pDcb = (DCB *)totalState;

    if( INVALID_HANDLE_VALUE == hComHandle || false == m_bOpen || NULL == totalState )
    {
        return false;
    }

    return ( SetCommState(hComHandle, pDcb) != FALSE );
}

bool ComControl::GetComState( void *totalState )
{
    HANDLE hComHandle = (HANDLE)m_fd;
    DCB * pDcb = (DCB *)totalState;

    if( INVALID_HANDLE_VALUE == hComHandle || false == m_bOpen || NULL == totalState )
    {
        return false;
    }

    return ( GetCommState(hComHandle, pDcb) != FALSE );
}

bool ComControl::SetTimeOut( unsigned long ReadIntervalTimeout20us, unsigned long ReadTotalTimeoutMultiplier20us, unsigned long ReadTotalTimeoutConstant20us,
    unsigned long WriteTotalTimeoutMultiplier20us, unsigned long WriteTotalTimeoutConstant20us )
{
    HANDLE hComHandle = (HANDLE)m_fd;
    COMMTIMEOUTS cto;

    if( INVALID_HANDLE_VALUE == hComHandle || false == m_bOpen )
    {
        return false;
    }

    cto.ReadIntervalTimeout         = ReadIntervalTimeout20us / 50;
    cto.ReadTotalTimeoutConstant    = ReadTotalTimeoutConstant20us / 50;
    cto.ReadTotalTimeoutMultiplier  = ReadTotalTimeoutMultiplier20us / 50;
    cto.WriteTotalTimeoutConstant   = WriteTotalTimeoutConstant20us / 50;
    cto.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier20us / 50;


    return ( SetCommTimeouts( hComHandle, &cto ) != FALSE );
}

bool ComControl::GetTimeOut( unsigned long &ReadIntervalTimeout20us, unsigned long &ReadTotalTimeoutMultiplier20us, unsigned long &ReadTotalTimeoutConstant20us,
    unsigned long &WriteTotalTimeoutMultiplier20us, unsigned long &WriteTotalTimeoutConstant20us )
{
    HANDLE hComHandle = (HANDLE)m_fd;
    COMMTIMEOUTS cto;

    if( INVALID_HANDLE_VALUE == hComHandle || false == m_bOpen )
    {
        return false;
    }

    if ( GetCommTimeouts( hComHandle, &cto ) == TRUE )
    {
        ReadIntervalTimeout20us         = cto.ReadIntervalTimeout * 50;
        ReadTotalTimeoutConstant20us    = cto.ReadTotalTimeoutConstant * 50;
        ReadTotalTimeoutMultiplier20us  = cto.ReadTotalTimeoutMultiplier * 50;
        WriteTotalTimeoutConstant20us   = cto.WriteTotalTimeoutConstant * 50;
        WriteTotalTimeoutMultiplier20us = cto.WriteTotalTimeoutMultiplier * 50;
        
        return true;
    }

    return false;
}