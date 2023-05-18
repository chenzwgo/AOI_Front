#ifndef MODBUS_COM_H
#define MODBUS_COM_H
#include <QObject>
#include "businesscommon.h"
#include "rtdefine.h"
#include "jmutex.h"
#include <QMutex>
#include "configui/configoper.h"

// modbus
#define ADR_Address  0x01   //地址
#define CMD_Read     0x03   //功能码-读取
#define CMD_One      0x06   //功能码-发送一位
struct StComConfig
{
	QString name;				//端口号
	unsigned int baudRate;	//波特率
	unsigned int parity ;	//校验位
	unsigned int byteSize;	//数据位
	unsigned int stopBits;	//停止位///* StopBits 1,3,2 = 1, 1.5, 2  */

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

	 /*写光源一个数据 参数：设备地址，起始寄存器地址， 写数据  返回：false失败，ture表示成功*/
	 bool writeOneLight(unsigned char rtu, unsigned short int RegAdd,unsigned short var, int iTimeOut=100, bool bRecv=true);
	 /*写光源多个数据 参数：设备地址，起始寄存器地址， 寄存器数量，写数据  返回：false失败，ture表示成功*/
	 bool writeMulLight(unsigned char rtu, unsigned short int RegAdd, short int RegCount, unsigned short var[], int iTimeOut=100, bool bRecv=true);



	/*1.对单个PLC操作*/
	/*读一个或多个模拟量  参数：站号，起始寄存器地址， 读的数量， 返回数据变量指针  返回：false失败，ture表示成功*/
	 bool readMultiRegD(unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, short int *list, int iTimeOut=100, bool bRecv=true);	
	/*读一个或多个开关量  参数：站号，起始寄存器地址， 读的数量， 返回数据变量指针  返回：false失败，ture表示成功*/
	 bool readMultiRegM(unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, bool *list, int iTimeOut=100, bool bRecv=true);
	/*读一个或多个开关量  参数：站号，起始寄存器地址， 读的数量， 返回数据变量指针  返回：false失败，ture表示成功*/
	 bool readMultiRegM_1x(unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, bool *list, int iTimeOut=100, bool bRecv=true);
	
	/*写一个模拟量 参数：站号，起始寄存器地址， 写数据  返回：false失败，ture表示成功*/
	 bool writeSingRegM(unsigned char rtu, unsigned short int RegAdd, bool var, int iTimeOut=100, bool bRecv=true);

	/*2.对全部PLC操作*/
	 bool writeAllRegD(unsigned short int RegAdd, bool var, bool bRecv=true);
	 bool writeAllRegM(unsigned short int RegAdd, short int var, bool bRecv=true);
private:
	
	
	void release();
	unsigned short getCRCChk(BYTE* data,WORD length);//生成CRC校验码
	char convertHexChar(char ch);			 // 10进制转16

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
