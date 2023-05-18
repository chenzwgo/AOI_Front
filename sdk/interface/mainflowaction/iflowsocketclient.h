#ifndef _IFLOW_SOCKET_CLIENT_H
#define _IFLOW_SOCKET_CLIENT_H

#include <string>
#include <vector>
using namespace std;

class IFlowSocketClient
{
public:
	virtual ~IFlowSocketClient() {}

    /************************************************************************/
    // @brief socket�Ƿ�����
    // @param
    // @return  ����true��ʾ�����ӣ�����false��ʾδ����
    /************************************************************************/
	virtual bool IsConnect() = 0;

    /************************************************************************/
    // @brief ���÷�����IP �˿�
    // @param serverIP:������Ip; ServerPort:�������˿ڣ� enablePing:�Ƿ���ping����
    // @return  ����true��ʾ���ӳɹ�������false��ʾ����ʧ��
    /************************************************************************/
    virtual void SetServerIp(const string &serverIP,unsigned int serverPort,bool enablePing = false) = 0;

    /************************************************************************/
    // @brief socket����
    // @param serverIP:������Ip; ServerPort:�������˿ڣ� enablePing:�Ƿ���ping����
    // @return  ����true��ʾ���ӳɹ�������false��ʾ����ʧ��
    /************************************************************************/
	virtual bool Connect() = 0;

    /************************************************************************/
    // @brief �Ͽ�����
    // @param
    // @return
    /************************************************************************/
	virtual void Disconnect() = 0;

    /************************************************************************/
    // @brief �����߳̽�����Ϣ��ɸѡ�������½�
    // @param endSign:������(����\r\n); maskInclude:�����ַ����б�; maskExclude:�������ַ����б�
    // @return ����true��ʾ���ͳɹ�������false��ʾ����ʧ��
    /************************************************************************/
    virtual void SetRecvMask(const string &endSign, const vector<string> &maskInclude = vector<string>(), const vector<string> &maskExclude = vector<string>()) = 0;

    /************************************************************************/
    // @brief ����ָ��
    // @param strCmd:�����ַ���
    // @return ����true��ʾ���ͳɹ�������false��ʾ����ʧ��
    /************************************************************************/
	virtual bool SendMsg(const string &strCmd) = 0;

    /************************************************************************/
    // @brief ���ջظ� (�������������ĵ�һ���ظ����ɹ��ʹӶ����Ƴ�֮)
    // @param strRecvMsg:���յĻظ���Ϣ��iTimeOut:��ʱʱ�䣻mask:ɸѡ����
    //        �磺T0* ��ʾT0��ͷ; *T0*��ʾ����T0; *T0��ʾT0��β
    // @return ����true��ʾ���ճɹ�������false��ʾ����ʧ��
    /************************************************************************/
    virtual bool GetRecv(string& strRecvMsg, const string &mask="") = 0;
    
    /************************************************************************/
    // @brief ������ݶ���
    // @param  mask:ɸѡ����,Ϊ�ձ�ʾ�������
    // @return
    /************************************************************************/
    virtual void ClearRecvList(const string &mask = "") = 0;

    /************************************************************************/
    // @brief �ͷ�socketClient
    // @param
    // @return
    /************************************************************************/
    virtual void ReleaseSocketClient() = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param 
    // @return ���ش�������
    /************************************************************************/
	virtual string GetLastError() = 0;
};
#endif // _IFLOW_SOCKET_CLIENT_H
