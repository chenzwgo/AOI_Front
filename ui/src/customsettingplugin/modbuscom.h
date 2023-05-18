#ifndef MODBUS_COM_H
#define MODBUS_COM_H
#include <QObject>
#include "businesscommon.h"
#include "rtdefine.h"
#include "jmutex.h"
#include <QMutex>
#include "configui/configoper.h"

// modbus
#define ADR_Address  0x01   //��ַ
#define CMD_Read     0x03   //������-��ȡ
#define CMD_One      0x06   //������-����һλ
struct StComConfig
{
	QString name;				//�˿ں�
	unsigned int baudRate;	//������
	unsigned int parity ;	//У��λ
	unsigned int byteSize;	//����λ
	unsigned int stopBits;	//ֹͣλ///* StopBits 1,3,2 = 1, 1.5, 2  */

	StComConfig()
	{
		name        = "";
		baudRate    = 0;
		parity      = 0;
		byteSize    = 0;
		stopBits    = 0;
	}
};
class ICom;
class CModbusCom : public QObject
{
	Q_OBJECT
public:
    explicit CModbusCom(QObject *parent = 0);
    ~CModbusCom();
	static CModbusCom* GetInstance();
	static void ReleaseInstance();

	 bool init(QString strCom, int baudRate, int parity, int byteSize, int stopBits);

	 bool send(const char* cmd, int ilen);
	 QString getSendData();
	 bool send(const unsigned char* cmd, int ilen);

	 int recvData(char* recv, int ilen,int timeout = 100);

	 int recvData(unsigned char* recv, int ilen,int timeout=100);

	 bool isOpen();

	 bool open();

	 bool close();

	 bool clear();

	 /*д��Դһ������ �������豸��ַ����ʼ�Ĵ�����ַ�� д����  ���أ�falseʧ�ܣ�ture��ʾ�ɹ�*/
	 bool writeOneLight(unsigned char rtu, unsigned short int RegAdd,unsigned short var, int iTimeOut=100, bool bRecv=true);
	 /*д��Դ������� �������豸��ַ����ʼ�Ĵ�����ַ�� �Ĵ���������д����  ���أ�falseʧ�ܣ�ture��ʾ�ɹ�*/
	 bool writeMulLight(unsigned char rtu, unsigned short int RegAdd, short int RegCount, unsigned short var[], int iTimeOut=100, bool bRecv=true);



	/*1.�Ե���PLC����*/
	/*��һ������ģ����  ������վ�ţ���ʼ�Ĵ�����ַ�� ���������� �������ݱ���ָ��  ���أ�falseʧ�ܣ�ture��ʾ�ɹ�*/
	 bool readMultiRegD(unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, short int *list, int iTimeOut=100, bool bRecv=true);	
	/*��һ������������  ������վ�ţ���ʼ�Ĵ�����ַ�� ���������� �������ݱ���ָ��  ���أ�falseʧ�ܣ�ture��ʾ�ɹ�*/
	 bool readMultiRegM(unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, bool *list, int iTimeOut=100, bool bRecv=true);
	/*��һ������������  ������վ�ţ���ʼ�Ĵ�����ַ�� ���������� �������ݱ���ָ��  ���أ�falseʧ�ܣ�ture��ʾ�ɹ�*/
	 bool readMultiRegM_1x(unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, bool *list, int iTimeOut=100, bool bRecv=true);
	
	/*дһ��ģ���� ������վ�ţ���ʼ�Ĵ�����ַ�� д����  ���أ�falseʧ�ܣ�ture��ʾ�ɹ�*/
	 bool writeSingRegM(unsigned char rtu, unsigned short int RegAdd, bool var, int iTimeOut=100, bool bRecv=true);

	/*2.��ȫ��PLC����*/
	 bool writeAllRegD(unsigned short int RegAdd, bool var, bool bRecv=true);
	 bool writeAllRegM(unsigned short int RegAdd, short int var, bool bRecv=true);
private:
	
	
	void release();
	unsigned short getCRCChk(BYTE* data,WORD length);//����CRCУ����
	char convertHexChar(char ch);			 // 10����ת16

private:
	bool	m_bInit;
	StComConfig m_stConfig;
	//CComDrive* m_pComDrive;
	static CModbusCom* m_pSelfInstance;
	QString m_strLogData;
	
	ICom*  m_pModbusCom;
	QMutex m_Locker;
};


class CModbusComMgr
{
public:
    enum eModbusComMgr{UP_LIGHT_COM=1 ,
                      DOWN_LIGHT_COM};
    static CModbusComMgr* GetInstance();
    void ReleaseInstance();
    CModbusCom* getCModbusCom(eModbusComMgr index,QString &strCom) ;
    CModbusComMgr();
    ~CModbusComMgr();
private:
    ComConfig getComCfg(eModbusComMgr index);
    map<string ,CModbusCom*> m_map;
    static CModbusComMgr* m_pSelfInstance;
};
#endif // MODBUS_COM_H
