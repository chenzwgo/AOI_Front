#include "modbuscom.h"
#include "rtdefine.h"
#include <Windows.h> 
#include "trace.h"
#include "sdkcommon/com/icommanager.h"
#include "sdkcommon/com/icom.h"
#include "proxymanager.h"
#include "trace.h"
#include "QString"
#define COMMUNICATION_TIME Sleep(10);


CModbusCom* CModbusCom::m_pSelfInstance = NULL;

unsigned short ModBusCRC (unsigned char *ptr,unsigned char size) 
{
	unsigned short a,b,tmp,CRC16,V;  

	CRC16=0xffff;    //CRC寄存器初始值

	for (a=0;a<size;a++) //N个字节
	{    
		CRC16=*ptr^CRC16;  
		for (b=0;b<8;b++)            //8位数据
		{  
			tmp=CRC16 & 0x0001;  
			CRC16 =CRC16 >>1; //右移一位
			if (tmp)  
			CRC16=CRC16^0xa001; //异或多项式
		} 
		*ptr++;
	}  

	V = ((CRC16 & 0x00FF) << 8) | ((CRC16 & 0xFF00) >> 8) ;//高低字节转换

	return V; 
}


CModbusCom::CModbusCom(QObject *parent)
	: QObject(parent),
     m_bInit(false),
	 m_pModbusCom(NULL)
{
	m_strLogData = "";
}
CModbusCom* CModbusCom::GetInstance()
{
	if (NULL == m_pSelfInstance)
	{
		m_pSelfInstance = new CModbusCom();
	}
	return m_pSelfInstance;
}

void CModbusCom::ReleaseInstance()
{
	if (NULL != m_pSelfInstance)
	{
		delete m_pSelfInstance;
		m_pSelfInstance = NULL;
	}
}
bool CModbusCom::init(QString strCom, int baudRate, int parity, int byteSize, int stopBits)
{
	QMutexLocker locker(&m_Locker);
	RETURN_V_IF(m_bInit&&(strCom == m_stConfig.name), true);//初始化过相同的COM口直接返回
	RETURN_V_IF(strCom.isEmpty(), false);

	//strcpy_s(m_stConfig.name, 63, name);
	m_stConfig.name = strCom;
	m_stConfig.baudRate = baudRate;
	m_stConfig.parity = parity;
	m_stConfig.byteSize = byteSize;
	m_stConfig.stopBits = stopBits;

	IComManager* pComManager = (IComManager *)GetSDKInstance("sdkcommon",IComManager::IID_COMMANAGER);
	RETURN_V_IF(NULL == pComManager, m_bInit);
    m_pModbusCom = pComManager->CreateCom(m_stConfig.name.toLatin1());
	RETURN_V_IF (NULL == m_pModbusCom, m_bInit);
    m_pModbusCom->SetComName(m_stConfig.name.toLatin1());
	RETURN_V_IF(!m_pModbusCom->OpenCom(), m_bInit);
	if(!m_pModbusCom->SetComState(m_stConfig.baudRate, m_stConfig.parity, m_stConfig.byteSize, m_stConfig.stopBits))
	{
		m_pModbusCom->CloseCom();
		return m_bInit;
	}
	m_bInit = true;
	return true;
}

CModbusCom::~CModbusCom()
{
	release();
}

void CModbusCom::release()
{
	QMutexLocker locker(&m_Locker);

	IComManager* pComManager = (IComManager *) GetSDKInstance("sdkcommon",IComManager::IID_COMMANAGER);
	RETURN_IF(NULL == m_pModbusCom || NULL == m_pModbusCom);
	m_pModbusCom->CloseCom();
	pComManager->ReleaseCom(m_pModbusCom);
	m_pModbusCom = NULL;
	m_bInit = false;

}

bool CModbusCom::send( const char* cmd, int ilen )
{
	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(NULL == cmd, false);
	RETURN_V_IF(m_pModbusCom == NULL, false);
	int writeSize = 0;
	int Sendlen = 0;

	if (!m_pModbusCom->IsOpen()){
		RETURN_V_IF(!m_pModbusCom->OpenCom(), false);
		Sleep(20);
	}
	RETURN_V_IF(!m_pModbusCom->FlushCom(), false);
	while(Sendlen != ilen){
		writeSize = m_pModbusCom->WriteCom((void *)(cmd + Sendlen),ilen - Sendlen);
		if (writeSize >0 )
			Sendlen += writeSize;
		else
			break;
	}
	return Sendlen != ilen ? false : true;
}
 QString CModbusCom:: getSendData()
 {
	 return m_strLogData;
 }
bool CModbusCom::send( const unsigned char* cmd, int ilen )
{
	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(NULL == cmd, false);
	RETURN_V_IF(m_pModbusCom == NULL, false);
	int writeSize = 0;
	int Sendlen = 0;

	if (!m_pModbusCom->IsOpen()){
		RETURN_V_IF(!m_pModbusCom->OpenCom(), false);
		Sleep(20);
	}
	RETURN_V_IF(!m_pModbusCom->FlushCom(), false);
	while(Sendlen != ilen){
		writeSize = m_pModbusCom->WriteCom((void *)(cmd + Sendlen),ilen - Sendlen);
		if (writeSize >0 )
			Sendlen += writeSize;
		else
			break;
	}
	return Sendlen != ilen ? false : true;;
}
int CModbusCom::recvData( char* recv, int ilen ,int timeout)
{
	RETURN_V_IF(NULL == recv, -1);
	RETURN_V_IF(m_pModbusCom == NULL, -1);
	int readSize = 0;
	int Readlen = 0;
	int trysec = 0;
	int itimeout = timeout/10;

	if (!m_pModbusCom->IsOpen()){
		RETURN_V_IF(!m_pModbusCom->OpenCom(), -1);
		Sleep(20);
	}

	while(Readlen != ilen)
	{
		readSize = m_pModbusCom->ReadCom((void *)(recv + Readlen),ilen - Readlen);
		if (readSize >0 )
			Readlen += readSize;
		else if(readSize == 0)
		{
			if(trysec < itimeout)
			{
				Sleep(10);
				trysec ++;
				continue;
			}
			else
				break;
		}
		else
			return -1;

		Sleep(10);
	}
	return Readlen;
}
int CModbusCom::recvData( unsigned char* recv, int ilen ,int timeout)
{
	RETURN_V_IF(!m_bInit, -1);
	RETURN_V_IF(NULL == recv, -1);
	RETURN_V_IF(m_pModbusCom == NULL, -1);
	int readSize = 0;
	int Readlen = 0;
	int trysec = 0;
	int itimeout = timeout/10;

	if (!m_pModbusCom->IsOpen()){
		RETURN_V_IF(!m_pModbusCom->OpenCom(), -1);
		Sleep(20);
	}

	while(Readlen != ilen)
	{
		readSize = m_pModbusCom->ReadCom((void *)(recv + Readlen),ilen - Readlen);
		if (readSize >0 )
			Readlen += readSize;
		else if(readSize == 0)
		{
			if(trysec < itimeout)
			{
				Sleep(10);
				trysec ++;
				continue;
			}
			else
				break;
		}
		else
			return -1;

		Sleep(10);
	}
	return Readlen;
}
bool CModbusCom::isOpen()
{
	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(m_pModbusCom == NULL, false);
	return m_pModbusCom->IsOpen();
}

bool CModbusCom::open()
{
	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(m_pModbusCom == NULL, false);
	return m_pModbusCom->OpenCom();
}

bool CModbusCom::close()
{
	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(m_pModbusCom == NULL, false);
	return m_pModbusCom->CloseCom();
}

bool CModbusCom::clear()
{
	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(m_pModbusCom == NULL, false);
	return m_pModbusCom->FlushCom();
}

bool CModbusCom::readMultiRegD( unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, short int *list, int iTimeOut/*=100*/, bool bRecv/*=true*/ )
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME
	
	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned char buff[256] = {0};
	unsigned char recv[256] = {0};
	unsigned short int crc16 = 0;
	unsigned short int crctmp = 0;
	short int vartmp = 0;

	buff[0] = rtu;
	buff[1] = 0x03;
	buff[2] = (unsigned char)(RegAdd >> 8);
	buff[3] = (unsigned char)RegAdd;
	buff[4] = (unsigned char)(RegCount >> 8);
	buff[5] = (unsigned char)RegCount;
	crc16 = getCRCChk(buff, 6);
	buff[6]= (unsigned char)(crc16 >> 8);
	buff[7]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send(buff,8);
	RETURN_V_IF(!bRes, bRes);
	RETURN_V_IF(!bRecv, bRes);

	int recvlen = 3+(RegCount *2)+ 2;
	int bytelen = recvData(recv, recvlen, iTimeOut);
	RETURN_V_IF(bytelen<=0, false);

	//返回长度有效,解析接收缓冲区
	RETURN_V_IF(!(bytelen== recvlen && recv[0]== rtu && recv[1]== 0x03), false);
	crc16= getCRCChk(recv, 3+(RegCount*2));
	crctmp= recv[bytelen-2];
	crctmp= crctmp << 8 | recv[bytelen-1];
	RETURN_V_IF(crc16 != crctmp, false);

	for(int i=0; i< RegCount; i++){
		vartmp= recv[3+(2*i)];
		vartmp= vartmp << 8;
		vartmp= vartmp | recv[3+((2*i)+1)];

		list[i]= vartmp;
	}

	return true;
}

bool CModbusCom::readMultiRegM( unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, bool *list, int iTimeOut/*=100*/, bool bRecv/*=true*/ )
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME

	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned char buff[256] = {0};
	unsigned char recv[256] = {0};
	unsigned short int crc16 = 0;
	unsigned short int crctmp = 0;

	buff[0] = rtu;
	buff[1] = 0x01;
	buff[2] = (unsigned char)(RegAdd >> 8);
	buff[3] = (unsigned char)RegAdd;
	buff[4] = (unsigned char)(RegCount >> 8);
	buff[5] = (unsigned char)RegCount;
	crc16 = getCRCChk(buff, 6);
	buff[6]= (unsigned char)(crc16 >> 8);
	buff[7]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send(buff,8);
	RETURN_V_IF(!bRes, bRes);
	RETURN_V_IF(!bRecv, bRes);

	//读数据
	int recvlen = 3+((RegCount+7)/8)+ 2;
	int bytelen = recvData(recv, recvlen, iTimeOut);
	RETURN_V_IF(bytelen<=0, false);

	//返回长度有效,解析接收缓冲区
	RETURN_V_IF(!(bytelen== recvlen && recv[0]== rtu && recv[1]== 0x01), false);
	crc16= getCRCChk(recv, 3+((RegCount +7)/8));
	crctmp= recv[bytelen-2];
	crctmp= crctmp << 8 | recv[bytelen-1];
	RETURN_V_IF(crc16!= crctmp, false);

	unsigned char row=0, col=0;						
	for(int i=0; i<RegCount; i++){						
		row= i / 8;							
		col= i % 8;							
		list[i]= recv[3 + row] >> col & 0x01;
	}

	return true;
}

bool CModbusCom::readMultiRegM_1x( unsigned char rtu, unsigned short int RegAdd, unsigned short int RegCount, bool *list, int iTimeOut/*=100*/, bool bRecv/*=true*/ )
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME

	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned short int crc16 = 0;
	unsigned short int crctmp = 0;
	unsigned char buff[256] = {0};
	unsigned char recv[256] = {0};

	buff[0]= rtu;
	buff[1]= 0x02;
	buff[2]= (unsigned char)(RegAdd >> 8);
	buff[3]= (unsigned char)RegAdd;
	buff[4]= (unsigned char)(RegCount >> 8);
	buff[5]= (unsigned char)RegCount;
	crc16= getCRCChk(buff, 6);
	buff[6]= (unsigned char)(crc16 >> 8);
	buff[7]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send((char *)buff, 8);
	RETURN_V_IF(!bRes, bRes);
	RETURN_V_IF(!bRecv, bRes);

	//读数据
	int recvlen = 3+((RegCount+7)/8)+ 2;
	int bytelen = recvData(recv, recvlen, iTimeOut);
	RETURN_V_IF(bytelen<=0, false);
	RETURN_V_IF(!(bytelen== recvlen && recv[0]== rtu && recv[1]== 0x02), false);

	//返回长度有效,解析接收缓冲区
	crc16= getCRCChk(recv, 3+((RegCount +7)/8));
	crctmp= recv[bytelen-2];
	crctmp= crctmp << 8 | recv[bytelen-1];
	RETURN_V_IF(crc16!= crctmp, false);

	unsigned char row=0, col=0;						
	for(int i=0; i<RegCount; i++){						
		row= i / 8;							
		col= i % 8;							
		list[i]= recv[3 + row] >> col & 0x01;
	}

	return true;
}
bool CModbusCom::writeOneLight( unsigned char rtu, unsigned short int RegAdd, unsigned short  var, int iTimeOut/*=100*/, bool bRecv/*=true*/ )
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME

	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned short int crc16 = 0;
	unsigned char buff[256] = {0};
	unsigned char recv[256] = {0};

	buff[0]= rtu;
	buff[1]= 0x06;
	buff[2]= (unsigned char)(RegAdd >> 8);
	buff[3]= (unsigned char)RegAdd;
	buff[4]= (unsigned char)(var >> 8);
	buff[5]= (unsigned char)var;
	crc16= getCRCChk(buff, 6);
	buff[6]= (unsigned char)(crc16 >> 8);
	buff[7]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send((char *)buff, 8);
	RETURN_V_IF(!bRes, bRes);
	RETURN_V_IF(!bRecv, bRes);

	int bytelen = recvData(recv, 8, iTimeOut);
	RETURN_V_IF(bytelen<=0, false);
	RETURN_V_IF(!(bytelen== 8 && recv[0]== rtu && recv[1]== buff[1]), false);

	return true;	
}

bool CModbusCom::writeMulLight(unsigned char rtu, unsigned short int RegAdd, short int RegCount, unsigned short var[], int iTimeOut/*=100*/, bool bRecv/*=true*/)
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME

	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(RegCount <= 1, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned short int crc16 = 0;
	unsigned char buff[256] = {0};
	unsigned char recv[256] = {0};

	buff[0] = rtu;								// 起始地址
	buff[1] = 0x10;								// 功能码
	buff[2] = (unsigned char)(RegAdd >> 8);		// 起始地址寄存器
	buff[3] = (unsigned char)RegAdd;			
	buff[4] = (unsigned char)(RegCount >> 8);	// 寄存器数量
	buff[5] = (unsigned char)RegCount;
	buff[6] = RegCount * 2;						// 总的byte位数

	int num = 0;
	for (int i=0; i<RegCount; i++)
	{
		buff[6 + ++num]= (unsigned char)(var[i] >> 8);
		buff[6 + ++num]= (unsigned char)var[i];
	}
	
	crc16= getCRCChk(buff, 7 + RegCount * 2);
	buff[6 + ++num]= (unsigned char)(crc16 >> 8);
	buff[6 + ++num]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send((char *)buff, 7 + RegCount*2 + 2 +1);//经测试需要加多一位的数据长度下发，对方才能正确 发到光源控制器
	RETURN_V_IF(!bRes, bRes);
	RETURN_V_IF(!bRecv, bRes);

	int bytelen = recvData(recv, 8, iTimeOut);
	RETURN_V_IF(bytelen<=0, false);
	RETURN_V_IF(!(bytelen == 8 && recv[0]== rtu && recv[1]== buff[1]), false);

	return true;	
}

bool CModbusCom::writeSingRegM( unsigned char rtu, unsigned short int RegAdd, bool var, int iTimeOut/*=100*/, bool bRecv/*=true*/ )
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME

	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned short int crc16 = 0;
	unsigned char buff[256] = {0};
	unsigned char recv[256] = {0};

	buff[0]= rtu;
	buff[1]= 0x05;
	buff[2]= (unsigned char)(RegAdd >> 8);
	buff[3]= (unsigned char)RegAdd;
	if(var){
		buff[4]= 0xff;
		buff[5]= 0x00;
	}
	else{
		buff[4]= 0x00;
		buff[5]= 0x00;
	}

	crc16= getCRCChk(buff, 6);
	buff[6]= (unsigned char)(crc16 >> 8);
	buff[7]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send((char *)buff, 8);
	RETURN_V_IF(!bRes, false);
	RETURN_V_IF(!bRecv, bRes);

	int bytelen = recvData(recv, 8, iTimeOut);
	RETURN_V_IF(bytelen<=0, false);
	RETURN_V_IF(!(bytelen== 8 && recv[0]== rtu && recv[1]== 0x05), false);

	return true;
}

bool CModbusCom::writeAllRegD( unsigned short int RegAdd, bool var, bool bRecv/*=true*/ )
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME

	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned short int crc16 = 0;
	unsigned char buff[256] = {0};

	buff[0]= 0xff;
	buff[1]= 0x06;
	buff[2]= (unsigned char)(RegAdd >> 8);
	buff[3]= (unsigned char)RegAdd;
	buff[4]= (unsigned char)(var >> 8);
	buff[5]= (unsigned char)var;
	crc16= getCRCChk(buff, 6);
	buff[6]= (unsigned char)(crc16 >> 8);
	buff[7]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send((char *)buff, 8);
	RETURN_V_IF(!bRes, false);

	return true;
}

bool CModbusCom::writeAllRegM( unsigned short int RegAdd, short int var, bool bRecv/*=true*/ )
{
	QMutexLocker locker(&m_Locker);
	COMMUNICATION_TIME

	RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(!isOpen(), false);

	unsigned short int crc16 = 0;
	unsigned char buff[256] = {0};

	buff[0]= 0xff;
	buff[1]= 0x05;
	buff[2]= (unsigned char)(RegAdd >> 8);
	buff[3]= (unsigned char)RegAdd;
	if(var){
		buff[4]= 0xff;
		buff[5]= 0x00;
	}
	else{
		buff[4]= 0x00;
		buff[5]= 0x00;
	}

	crc16= getCRCChk(buff, 6);
	buff[6]= (unsigned char)(crc16 >> 8);
	buff[7]= (unsigned char)crc16;

	//发送数据
	m_pModbusCom->FlushCom();
	bool bRes = send((char *)buff, 8);
	RETURN_V_IF(!bRes, false);

	return true;
}

unsigned short CModbusCom::getCRCChk( BYTE* data,WORD length )
{
#pragma region	 高位字节的CRC 值
	const unsigned char m_auchCRCHi[]=
	{
		0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
		0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
		0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
		0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
		0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
		0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
		0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
		0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
		0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
		0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
		0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
		0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
		0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
		0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
		0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
		0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
		0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
		0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
		0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
		0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
		0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
		0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
		0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
		0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
		0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
		0x80,0x41,0x00,0xC1,0x81,0x40 
	};
#pragma endregion

#pragma region	 低位字节的CRC 值
	const unsigned char m_auchCRCLo[]=
	{
		0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,
		0x07,0xC7,0x05,0xC5,0xC4,0x04,0xCC,0x0C,0x0D,0xCD,
		0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,
		0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
		0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,
		0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
		0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
		0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
		0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
		0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,
		0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,
		0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
		0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,
		0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
		0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
		0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
		0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,
		0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
		0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,
		0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
		0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
		0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,
		0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B,
		0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
		0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
		0x43,0x83,0x41,0x81,0x80,0x40 
	};
#pragma endregion

	BYTE uchCRCHi=0xFF ;	//CRC高位字节初始化
	BYTE uchCRCLo=0xFF ;	//CRC低位自己初始化
	WORD uIndex = 0;		//CRC查询表索引

	/* 完成整个报文缓冲区*/
	while(length--)
	{
		uIndex=uchCRCHi^*data++;	/* 计算CRC */
		uchCRCHi=uchCRCLo^m_auchCRCHi[uIndex];
		uchCRCLo=m_auchCRCLo[uIndex];
	}
	return(uchCRCHi<<8|uchCRCLo);
}

char CModbusCom::convertHexChar( char ch )
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	else if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	else if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
	else return (-1);
}
CModbusComMgr* CModbusComMgr::m_pSelfInstance = NULL;

CModbusComMgr::CModbusComMgr()
{

}
CModbusComMgr* CModbusComMgr::GetInstance()
{
    if (NULL == m_pSelfInstance)
    {
        m_pSelfInstance = new CModbusComMgr;
    }
    return m_pSelfInstance;
}

void CModbusComMgr::ReleaseInstance()
{
      map<string ,CModbusCom*>::iterator it = m_map.begin();
      while(it != m_map.end())
      {
          delete it->second;
          it++;
      }
      m_map.clear();
}

CModbusCom* CModbusComMgr::getCModbusCom( eModbusComMgr index ,QString &strCom)
{
    CModbusCom * pCModbusCom = NULL;
    QString str = QString ::number(index);

    ComConfig	m_stLightCom = getComCfg(index);
    strCom = QString::fromStdString(m_stLightCom.name);

    if(m_map.find(str.toStdString()) == m_map.end())
    {
        pCModbusCom  = new CModbusCom();
        bool res = false;
        res = pCModbusCom->init(strCom,m_stLightCom.baudRate,m_stLightCom.parity,m_stLightCom.byteSize,m_stLightCom.stopBits);
        if (!res)
        {
            return NULL;
        }
       m_map.insert(pair<string ,CModbusCom*>(strCom.toStdString(),pCModbusCom));
    }
    m_map[strCom.toStdString()]->open();
    return m_map[strCom.toStdString()];
}

ComConfig CModbusComMgr::getComCfg( eModbusComMgr index )
{
    if (UP_LIGHT_COM == index )
    {
         return *CONFIG_INSTANCE->GetLightCom();
    }
    return *CONFIG_INSTANCE->GetLightDownCom();
}


