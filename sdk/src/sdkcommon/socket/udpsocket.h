#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include "dllexport.h"
#include "sdkcommon/socket/iudpsocket.h"
#include <string>
using namespace  std;
 
class  udpsocket :public IUdpSocket
{
public:
    udpsocket();
    ~udpsocket();

    bool Init(const char* pIP, unsigned int iport = 0);
    int SendMsg(const char* pDesIP,unsigned int iDesPort,const char* pBuf,unsigned int ilen);
    int RecvMsg(const char* pDesIP,unsigned int iDesPort,const char* pBuf,unsigned int ilen);
    void getIPandPort(string& strIp,unsigned int & iPort);
private:
    bool Uninit();
    bool m_binit;
    int m_isock;
    string m_strIP;
    unsigned int m_iPort;
};



#endif // UDPSOCKET_H
