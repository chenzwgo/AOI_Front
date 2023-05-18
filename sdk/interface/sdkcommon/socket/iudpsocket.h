#ifndef _IUDP_SOCKET__H_
#define _IUDP_SOCKET__H_

#include <string>
using std::string;

class IUdpSocket
{
public:
	enum
	{
		IID_UDP_SOCKET = 0x2001
	};
	virtual ~IUdpSocket(){}
public:

    virtual bool Init(const char* pIP, unsigned int iport = 0) =0;
    virtual int SendMsg(const char* pDesIP,unsigned int iDesPort,const char* pBuf,unsigned int ilen)=0;
    virtual int RecvMsg(const char* pDesIP,unsigned int iDesPort,const char* pBuf,unsigned int ilen)=0;
    virtual void getIPandPort(string& strIp,unsigned int & iPort)=0;
};


#endif
