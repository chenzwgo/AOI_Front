#ifndef _SERIALPORTREADER_H
#define _SERIALPORTREADER_H

#include <Windows.h>
#include <stdio.h> 
#include <string>
#include <vector>

using namespace std;
/* 
* 计时器
*/
class  CPerfTimer
{

public:
	CPerfTimer(void);
	~CPerfTimer(void);
public:
	// 开始计时
	inline void Start();

	// 重新开始计时
	inline void ReStart();

	// 获取运行时间(ms)
	inline int GetTimeSpan();
private:
	LARGE_INTEGER m_liFreq;
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liEnd;
};

class CSerialPortControl
{
public:
	CSerialPortControl();
	~CSerialPortControl(void);
public:
	/* @brief 获取所有串口
	*  @return: 所有串口列表
	*/
	static std::vector<std::string > GetAllPort();


	/* @brief 打开串口
	*  @param[in] portNo: 串口号
	*  @param[in] baud: 波特率
	*  @return: 是否打开
	*/
     bool OpenPort(const char* port,UINT baud=CBR_9600);

	/* @brief 是否打开串口
	*  @return: 是否打开
	*/
	 bool IsOpen();

	/* @brief 是否打开串口
	*  @return: 是否打开
	*/
	 void ClosePort();

	 /* @brief 写入数据到串口(写入后根据读取值判断是否设置争取)
	 *  @param[in] pData: 写入数据
	 *  @param[in] length: 写入数据长度
	 *  @param[in] nTimeOut: 超时时间
	 *  @return: 写入是否成功
	 */
	 bool WriteDataToPort(char* pData,int length,int nTimeOut);

	/* @brief 写入数据到串口(写入后直接退出)
	 *  @param[in] pData: 写入数据
	 *  @param[in] length: 写入数据长度
	 *  @return: 写入是否成功
	 */
	bool WriteDataToPort(char* pData,int length);

	/* @brief 读取数据从串口(根据返回值并判断是否正确)
	 *  @param[Out] pData: 读取数据
	 *  @param[in] length: 数组长度
	 *  @param[Out] nReadLen: 读取数据长度
	 *  @param[in] nTimeOut: 超时时间
	 *  @return: 写入是否成功
	 */
     bool ReadDataFromPort(char* pData , int length , int* nReadLen,int nTimeOut);

private:
	void ReadFromPort(std::string& strReceive,int nTimeOut);
private:
	// 串口句柄
	bool m_bOpenPort;
	HANDLE m_hCom;
	CRITICAL_SECTION m_cs;
	CPerfTimer m_perfTimer;
};

#endif _SERIALPORTREADER_H