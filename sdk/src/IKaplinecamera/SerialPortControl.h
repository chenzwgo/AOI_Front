#ifndef _SERIALPORTREADER_H
#define _SERIALPORTREADER_H

#include <Windows.h>
#include <stdio.h> 
#include <string>
#include <vector>

using namespace std;
/* 
* ��ʱ��
*/
class  CPerfTimer
{

public:
	CPerfTimer(void);
	~CPerfTimer(void);
public:
	// ��ʼ��ʱ
	inline void Start();

	// ���¿�ʼ��ʱ
	inline void ReStart();

	// ��ȡ����ʱ��(ms)
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
	/* @brief ��ȡ���д���
	*  @return: ���д����б�
	*/
	static std::vector<std::string > GetAllPort();


	/* @brief �򿪴���
	*  @param[in] portNo: ���ں�
	*  @param[in] baud: ������
	*  @return: �Ƿ��
	*/
     bool OpenPort(const char* port,UINT baud=CBR_9600);

	/* @brief �Ƿ�򿪴���
	*  @return: �Ƿ��
	*/
	 bool IsOpen();

	/* @brief �Ƿ�򿪴���
	*  @return: �Ƿ��
	*/
	 void ClosePort();

	 /* @brief д�����ݵ�����(д�����ݶ�ȡֵ�ж��Ƿ�������ȡ)
	 *  @param[in] pData: д������
	 *  @param[in] length: д�����ݳ���
	 *  @param[in] nTimeOut: ��ʱʱ��
	 *  @return: д���Ƿ�ɹ�
	 */
	 bool WriteDataToPort(char* pData,int length,int nTimeOut);

	/* @brief д�����ݵ�����(д���ֱ���˳�)
	 *  @param[in] pData: д������
	 *  @param[in] length: д�����ݳ���
	 *  @return: д���Ƿ�ɹ�
	 */
	bool WriteDataToPort(char* pData,int length);

	/* @brief ��ȡ���ݴӴ���(���ݷ���ֵ���ж��Ƿ���ȷ)
	 *  @param[Out] pData: ��ȡ����
	 *  @param[in] length: ���鳤��
	 *  @param[Out] nReadLen: ��ȡ���ݳ���
	 *  @param[in] nTimeOut: ��ʱʱ��
	 *  @return: д���Ƿ�ɹ�
	 */
     bool ReadDataFromPort(char* pData , int length , int* nReadLen,int nTimeOut);

private:
	void ReadFromPort(std::string& strReceive,int nTimeOut);
private:
	// ���ھ��
	bool m_bOpenPort;
	HANDLE m_hCom;
	CRITICAL_SECTION m_cs;
	CPerfTimer m_perfTimer;
};

#endif _SERIALPORTREADER_H