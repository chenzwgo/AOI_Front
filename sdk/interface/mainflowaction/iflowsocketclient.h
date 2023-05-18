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
    // @brief socket是否连接
    // @param
    // @return  返回true表示已连接，返回false表示未连接
    /************************************************************************/
	virtual bool IsConnect() = 0;

    /************************************************************************/
    // @brief 设置服务器IP 端口
    // @param serverIP:服务器Ip; ServerPort:服务器端口； enablePing:是否开启ping功能
    // @return  返回true表示连接成功，返回false表示连接失败
    /************************************************************************/
    virtual void SetServerIp(const string &serverIP,unsigned int serverPort,bool enablePing = false) = 0;

    /************************************************************************/
    // @brief socket连接
    // @param serverIP:服务器Ip; ServerPort:服务器端口； enablePing:是否开启ping功能
    // @return  返回true表示连接成功，返回false表示连接失败
    /************************************************************************/
	virtual bool Connect() = 0;

    /************************************************************************/
    // @brief 断开连接
    // @param
    // @return
    /************************************************************************/
	virtual void Disconnect() = 0;

    /************************************************************************/
    // @brief 设置线程接收消息的筛选条件，新建
    // @param endSign:结束符(常用\r\n); maskInclude:包含字符串列表; maskExclude:不包含字符串列表
    // @return 返回true表示发送成功，返回false表示发送失败
    /************************************************************************/
    virtual void SetRecvMask(const string &endSign, const vector<string> &maskInclude = vector<string>(), const vector<string> &maskExclude = vector<string>()) = 0;

    /************************************************************************/
    // @brief 发送指令
    // @param strCmd:命令字符串
    // @return 返回true表示发送成功，返回false表示发送失败
    /************************************************************************/
	virtual bool SendMsg(const string &strCmd) = 0;

    /************************************************************************/
    // @brief 接收回复 (查找满足条件的第一条回复，成功就从队列移除之)
    // @param strRecvMsg:接收的回复信息；iTimeOut:超时时间；mask:筛选条件
    //        如：T0* 表示T0开头; *T0*表示包含T0; *T0表示T0结尾
    // @return 返回true表示接收成功，返回false表示接收失败
    /************************************************************************/
    virtual bool GetRecv(string& strRecvMsg, const string &mask="") = 0;
    
    /************************************************************************/
    // @brief 清除数据队列
    // @param  mask:筛选条件,为空表示清除所有
    // @return
    /************************************************************************/
    virtual void ClearRecvList(const string &mask = "") = 0;

    /************************************************************************/
    // @brief 释放socketClient
    // @param
    // @return
    /************************************************************************/
    virtual void ReleaseSocketClient() = 0;

    /************************************************************************/
    // @brief 获取错误描述
    // @param 
    // @return 返回错误描述
    /************************************************************************/
	virtual string GetLastError() = 0;
};
#endif // _IFLOW_SOCKET_CLIENT_H
