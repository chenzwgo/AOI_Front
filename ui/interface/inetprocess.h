#ifndef _INETPROCESS_H_
#define _INETPROCESS_H_



class INetProcess
{
public:
    enum
    {
        IID_NETPROCESS = 0x108
    };
	
	INetProcess(){}
    virtual ~INetProcess() { }
    virtual bool Connect(const char* szID) = 0;
    virtual bool DisConnect(const char* szID) = 0;
    virtual bool IsConnect(const char* szID) = 0;
    virtual void ClearNetBuffer(const char* szID) = 0;
    virtual int  ReadNet(const char* szID, char *pBuffer, unsigned int ibufferLength,const char *szMark = NULL) = 0;
    virtual int  WriteNet(const char* szID, const char *pBuffer, unsigned int ibufferLength) = 0;
};

#endif // _IVISIONPROCESS_H_