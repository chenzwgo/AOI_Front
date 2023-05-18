
#include<string.h>
#include <assert.h>
#include "robotcrl.h"
#include "./sdkcommon/socket/isocketclientmanager.h"
#include "../sdk/include/proxymanager.h"
#include "filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "trace.h"


#define Robot_Bvale_Second  134106
#define Robot_Avale_Second  -115959

//最后4位数据 #define Robot_vale_First    325480
#define Robot_vale_1_1   0xF7
#define Robot_vale_1_2   0x68
#define Robot_vale_1_3   0x00
#define Robot_vale_1_4   0x04

//最后4位数据 bB Robot_vale_2    134106
#define Robot_Bvale_2_1   0x0B
#define Robot_Bvale_2_2   0xDA
#define Robot_Bvale_2_3   0x00
#define Robot_Bvale_2_4   0x02

//最后4位数据 A Robot_vale_2    -115959
#define Robot_Avale_2_1   0x3B
#define Robot_Avale_2_2   0x09
#define Robot_Avale_2_3   0xFF
#define Robot_Avale_2_4   0xFE

#define Robot_Z            0
#define Robot_Flag         1


#define Robot_BLine_Ok_addr_1  0x121C
#define Robot_BLine_Ok_addr_2  0x121E
#define Robot_BLine_Ok_addr_3  0x1220

#define Robot_BLine_NG_addr_1  0x1222
#define Robot_BLine_NG_addr_2  0x1224
#define Robot_BLine_NG_addr_3  0x1226

#define Robot_BLine_RI_addr_1  0x1228
#define Robot_BLine_RI_addr_2  0x122A
#define Robot_BLine_RI_addr_3  0x122C

#define Robot_BLine_Mark_addr  0x125A
///////////////////////////////////////////////
#define Robot_ALine_Ok_addr_1  0x151C
#define Robot_ALine_Ok_addr_2  0x151E
#define Robot_ALine_Ok_addr_3  0x1520
			  
#define Robot_ALine_NG_addr_1  0x1522
#define Robot_ALine_NG_addr_2  0x1524
#define Robot_ALine_NG_addr_3  0x1526
			  
#define Robot_ALine_RI_addr_1  0x1528
#define Robot_ALine_RI_addr_2  0x152A
#define Robot_ALine_RI_addr_3  0x152C
			  
#define Robot_ALine_Mark_addr  0x155A


RobotCrl*RobotCrl::m_pSelfInstance = NULL;
RobotCrl::RobotCrl()
         :m_pISocketClient(NULL),
	      m_pthread(NULL),
	      m_bRun(false)
{
	m_aoiIp = "127.0.0.1";
	m_aoiport = 2000;
	m_robotip = "127.0.0.1";
	m_robotport= 1000;
	m_pthread = new JThreadTP<RobotCrl, void *, void>;
	
	getCfgParam();
	ISocketClientManager* pISocketClientManager = (ISocketClientManager *)GetSDKInstance("sdkcommon", ISocketClientManager::IID_SOCKETCLIENTMANAGER);
	if (NULL != pISocketClientManager)
	{
		m_pISocketClient = pISocketClientManager->CreateSocketClient(m_aoiIp.c_str(), m_aoiport);
		if (NULL != m_pISocketClient)
		{
			m_pISocketClient->Connect(m_robotip.c_str(), m_robotport);
		}
	}
}

RobotCrl::~RobotCrl()
{
	StopRobotThread();
	ISocketClientManager* pISocketClientManager = (ISocketClientManager *)GetSDKInstance("sdkcommon", ISocketClientManager::IID_SOCKETCLIENTMANAGER);
	if (NULL != pISocketClientManager)
	{     
		if (m_pISocketClient)
		{
			pISocketClientManager->ReleaseSocketClient(m_pISocketClient);
		}
	}
}
void RobotCrl::StartThreadConnetRobot()
{
	return;
	if (m_pthread)
	{
		m_bRun = true;
		m_pthread->StartThread(this, &RobotCrl::ThreadClientRobot, this);
	}
}
//停止
void RobotCrl::StopRobotThread()
{
	m_bRun = false;
	if (m_pthread)
	{
		do
		{
			Sleep(10);
		} while (m_pthread->IsRunning());
	}
}
void RobotCrl::ThreadClientRobot(void* param)
{
	if (NULL != m_pISocketClient)
	{
		m_pISocketClient->Connect(m_robotip.c_str(), m_robotport);
	}
	unsigned short int crc16 = 0;
	unsigned char buff[256] = { 0 };

	buff[0] = 0x00;
	buff[1] = 0x00;
	buff[2] = 0x00;
	buff[3] = 0x00;
	buff[4] = 0x00;//默认
	buff[5] = 0x0B;
	buff[6] = 0x02; //站号
	buff[7] = 0x10; //站号
					//
	buff[8] = 0x1F;			// 8
	buff[9] = 0xFF;			// 9
					////地址变化
	buff[10] = 0x00;//默认
	buff[11] = 0x02;//默认
	buff[12] = 0x04;//默认

	buff[13] = 0x00;//默认
	buff[14] = 0x01;//默认
	buff[15] = 0x00;//默认
	buff[16] = 0x00;//默认

	//crc16 = getCRCChk(buff, 17);
	//buff[17] = (unsigned char)(crc16 >> 8);
	//buff[18] = (unsigned char)crc16;
	
	do 
	{
		if (!m_bRun) {
			return;
		}
		ClientSendMsg((char *)buff, 17);
		Sleep(500);
	} while (m_bRun);
}
void RobotCrl::getCfgParam()
{
	IFileVariable* pFileVariable = getCurrentRobotVariable();
	if (pFileVariable)
	{
		m_aoiIp = pFileVariable->getValue(ROBOT_SET_AOI_IP,"127.0.0.1").toCString();
		m_aoiport = pFileVariable->getValue(ROBOT_SET_AOI_PORT, 2000).toInt();
		m_robotip = pFileVariable->getValue(ROBOT_SET_ROBOT_IP,"127.0.0.1").toCString();
		m_robotport = pFileVariable->getValue(ROBOT_SET_ROBOT_PORT, 1000).toInt();
	}
}
RobotCrl* RobotCrl::GetInstance()
{
	if (NULL == m_pSelfInstance)
	{
		m_pSelfInstance = new RobotCrl();
	}
	return m_pSelfInstance;
}
void RobotCrl::ReleaseInstance()
{
	if (NULL != m_pSelfInstance)
	{
		delete m_pSelfInstance;
		m_pSelfInstance = NULL;
	}
}
bool RobotCrl::ReConnect()
{
	if (m_pISocketClient)
	{
		return m_pISocketClient->ReConnect();
		//m_pISocketClient->Connect(m_robotip.c_str(), m_robotport);
	}
	return false;
}
bool RobotCrl::DisConnect()
{
	if (m_pISocketClient)
	{
		m_pISocketClient->DisConnect();
	}
	return false;
}
int  RobotCrl::ClientRecvMsg(char *pBuffer, unsigned int ibufferLength, const char *szMark)
{
	if (m_pISocketClient)
	{
		return m_pISocketClient->ClientRecvMsg(pBuffer,   ibufferLength,   szMark);
	}
	return 0;
}
int  RobotCrl::ClientSendMsg(const char *pData, unsigned int iDataLength)
{
	if (m_pISocketClient)
	{
		return m_pISocketClient->ClientSendMsg(pData, iDataLength);
	}
	return 0;
}

int RobotCrl::SendRobotModubsData(bool bALine, enResToRobot eRes, enSendToRobotIndex Index)
{
	unsigned short int crc16 = 0;
	unsigned char buff[256] = { 0 };

	buff[0] = 0x00;
	buff[1] = 0x00;
	buff[2] = 0x00;
	buff[3] = 0x00;
	buff[4] = 0x00;//默认
	buff[5] = 0x0B;
	buff[6] = 0x02; //站号
	buff[7] = 0x10; //站号
	//
	//地址变化
	// 8
	// 9
	////地址变化
	buff[10]  = 0x00;//默认
	buff[11] = 0x02;//默认
	buff[12] = 0x04;//默认

	//后面 4 位内容变化
	switch (Index)
	{
	case enResSendTo_0:
		if (enRes_OK ==eRes)
		{
			if (bALine){
				buff[8] = (unsigned char)(Robot_ALine_Ok_addr_1 >> 8);
				buff[9] = (unsigned char)Robot_ALine_Ok_addr_1;
			}
			else{
				buff[8] = (unsigned char)(Robot_BLine_Ok_addr_1 >> 8);
				buff[9] = (unsigned char)Robot_BLine_Ok_addr_1;
			}
		}
		else if (enRes_RI == eRes)
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_RI_addr_1 >> 8);
				buff[9] = (unsigned char)Robot_ALine_RI_addr_1;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_RI_addr_1 >> 8);
				buff[9] = (unsigned char)Robot_BLine_RI_addr_1;
			}
		}
		else
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_NG_addr_1 >> 8);
				buff[9] = (unsigned char)Robot_ALine_NG_addr_1;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_NG_addr_1 >> 8);
				buff[9] = (unsigned char)Robot_BLine_NG_addr_1;
			}
		}
		//
		buff[13] = Robot_vale_1_1;
		buff[14] = Robot_vale_1_2;
		buff[15] = Robot_vale_1_3;
		buff[16] = Robot_vale_1_4;
		break;
	case enResSendTo_1:
		if (enRes_OK == eRes)
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_Ok_addr_2 >> 8);
				buff[9] = (unsigned char)Robot_ALine_Ok_addr_2;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_Ok_addr_2 >> 8);
				buff[9] = (unsigned char)Robot_BLine_Ok_addr_2;
			}
		}
		else if (enRes_RI == eRes)
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_RI_addr_2 >> 8);
				buff[9] = (unsigned char)Robot_ALine_RI_addr_2;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_RI_addr_2 >> 8);
				buff[9] = (unsigned char)Robot_BLine_RI_addr_2;
			}
		}
		else
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_NG_addr_2 >> 8);
				buff[9] = (unsigned char)Robot_ALine_NG_addr_2;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_NG_addr_2 >> 8);
				buff[9] = (unsigned char)Robot_BLine_NG_addr_2;
			}
		}
		if (bALine)
		{
			buff[13] = Robot_Avale_2_1;
			buff[14] = Robot_Avale_2_2;
			buff[15] = Robot_Avale_2_3;
			buff[16] = Robot_Avale_2_4;
			break;
		}
		buff[13] = Robot_Bvale_2_1;
		buff[14] = Robot_Bvale_2_2;
		buff[15] = Robot_Bvale_2_3;
		buff[16] = Robot_Bvale_2_4;
		break;
	case enResSendTo_2:
		if (enRes_OK == eRes)
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_Ok_addr_3 >> 8);
				buff[9] = (unsigned char)Robot_ALine_Ok_addr_3;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_Ok_addr_3 >> 8);
				buff[9] = (unsigned char)Robot_BLine_Ok_addr_3;
			}
		}
		else if (enRes_RI == eRes)
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_RI_addr_3 >> 8);
				buff[9] = (unsigned char)Robot_ALine_RI_addr_3;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_RI_addr_3 >> 8);
				buff[9] = (unsigned char)Robot_BLine_RI_addr_3;
			}
		}
		else
		{
			if (bALine) {
				buff[8] = (unsigned char)(Robot_ALine_NG_addr_3 >> 8);
				buff[9] = (unsigned char)Robot_ALine_NG_addr_3;
			}
			else {
				buff[8] = (unsigned char)(Robot_BLine_NG_addr_3 >> 8);
				buff[9] = (unsigned char)Robot_BLine_NG_addr_3;
			}
		}
		buff[13] = 0x00;
		buff[14] = 0x00;
		buff[15] = 0x00;
		buff[16] = 0x00;
		break;
	case enResSendTo_3:
		//
		if (bALine) {
			buff[8] = 0x15;
			buff[9] = 0x5A;
		}
		else {
			buff[8] = 0x12;
			buff[9] = 0x5A;
		}
		//
		buff[13] = 0x00;
		buff[14] = 0x01;
		buff[15] = 0x00;
		buff[16] = 0x00;
		break;
	default:
		break;
	}

	//crc16 = getCRCChk(buff, 17);
	//buff[17] = (unsigned char)(crc16 >> 8);
	//buff[18] = (unsigned char)crc16;
	return ClientSendMsg((char *)buff, 17);
}

int RobotCrl::ClientSendResMsgToRobot(enResToRobot eRes, bool bALine)
{
	
	for (int i = enResSendTo_0; i < 4;i++)
	{
		SendRobotModubsData(bALine, eRes,(enSendToRobotIndex)i);
		Sleep(30);
	}
	return 0;
	//return checkRobotRes(enCheckSend, eRes, bALine);
}
bool RobotCrl::IsClientOnline()
{
	if (m_pISocketClient)
	{
		return m_pISocketClient->IsClientOnline();
	}
	return false;;
}
unsigned short RobotCrl::getCRCChk(BYTE* data, WORD length)
{
#pragma region	 高位字节的CRC 值
	const unsigned char m_auchCRCHi[] =
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
	const unsigned char m_auchCRCLo[] =
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

	BYTE uchCRCHi = 0xFF;	//CRC高位字节初始化
	BYTE uchCRCLo = 0xFF;	//CRC低位自己初始化
	WORD uIndex = 0;		//CRC查询表索引

							/* 完成整个报文缓冲区*/
	while (length--)
	{
		uIndex = uchCRCHi^*data++;	/* 计算CRC */
		uchCRCHi = uchCRCLo^m_auchCRCHi[uIndex];
		uchCRCLo = m_auchCRCLo[uIndex];
	}
	return(uchCRCHi << 8 | uchCRCLo);
}
int RobotCrl::checkRobotRes(enCheckRobotResMark iMark, enResToRobot eRes, bool bALine)
{

	unsigned char buff[32] = { 0 };

	buff[0] = 0x00;
	buff[1] = 0x00;
	buff[2] = 0x00;
	buff[3] = 0x00;
	buff[4] = 0x00;//默认
	buff[5] = 0x06;
	buff[6] = 0x02; 
	buff[7] = 0x03; 

	if (enCheckSend == iMark)
	{
		buff[8] = 0x31;
	}
	else if (enCheckRobotMove == iMark)
	{
		buff[8] = 0x32;
	}
	buff[9] = bALine?0x00:0x02;
	buff[10] = 0x00;
	buff[11] = 0x02;

	//send
	m_mutex.Lock();
	if (NULL != m_pISocketClient)
	{
		m_pISocketClient->ClearClientRecvBuffer();
	}
	ClientSendMsg((char*)buff, 12);
	Sleep(10);
	//OK 1 NG 2 RI 3
	int iRes = ClientRecvMsg((char*)buff,13);
	m_mutex.Unlock();
	if (iRes >= 13)
	{
		switch (eRes)
		{
		case enRes_OK:
			if (buff[10] == 0x01)
			{
				return 0;
			}
			break;
		case enRes_RI:
			if (buff[10] == 0x03)
			{
				return 0;
			}
			break;
		case enRes_NG:
			if (buff[10] == 0x02)
			{
				return 0;
			}
			break;
		case enRes_SIZE:
			break;
		default:
			break;
		}

	}
	//
	return -1;
}
int RobotCrl::ClientCheckRobotRes(enResToRobot eRes, bool bALine)
{
	return checkRobotRes(enCheckRobotMove,eRes, bALine);
}
bool RobotCrl::send_msg_to_plc(unsigned short itime)
{
	unsigned char buff[32] = { 0 };
	buff[0] = 0xF5;
	buff[1] = 0x01;
	buff[2] = itime;
	buff[3] = 0x00;

	m_mutex.Lock();
	if (NULL != m_pISocketClient)
	{
		if (!IsClientOnline())
		{
			ReConnect();
		}
		m_pISocketClient->ClearClientRecvBuffer();
	}
	ClientSendMsg((char*)buff, 4);
	Sleep(10);
	//OK 1 NG 2 RI 3
	int iRes = ClientRecvMsg((char*)buff, 13);
	m_mutex.Unlock();
	if (iRes>=2)
	{
		if ('O'==buff[0]&& 'K' == buff[1])
		{
			return true;
		}
		
	}
	return false;
}
bool  RobotCrl::send_sn_and_res_to_xialiaoji(unsigned long sn, unsigned long res)
{
	unsigned char buff[32] = { 0 };
	buff[0] = 0x02;
	result_t t_data;
	t_data.result = res;
	t_data.sn = sn;
	memcpy(&buff[1] , (unsigned char *)&t_data , sizeof(result_t));

	if (NULL != m_pISocketClient)
	{
		if (!IsClientOnline())
		{
			ReConnect();
		}
		m_pISocketClient->ClearClientRecvBuffer();
	}
	int ires = ClientSendMsg((char*)buff, 9);
	if (ires>1)
	{
		if (0x02 == buff[0] )
		{
			return true;
		}
	}

	return false;
}
bool  RobotCrl::send_sn_to_xialiaoji(unsigned long sn)
{
	unsigned char buff[32] = { 0 };
	buff[0] = 0x03;
	memcpy(&buff[1], (unsigned char *)&sn, 4);

	if (NULL != m_pISocketClient)
	{
		if (!IsClientOnline())
		{
			ReConnect();
		}
		m_pISocketClient->ClearClientRecvBuffer();
	}
	int ires = ClientSendMsg((char*)buff, 5);
	if (ires > 1)
	{
		if (0x03 == buff[0])
		{
			return true;
		}
	}

	return false;
}
bool  RobotCrl::get_xialiaoji_sn(unsigned long &sn)
{
	unsigned char buff[32] = { 0 };
	buff[0] = 0x05;
	memcpy(&buff[1], (unsigned char *)&sn, 4);

	if (NULL != m_pISocketClient)
	{
		if (!IsClientOnline())
		{
			ReConnect();
		}
		m_pISocketClient->ClearClientRecvBuffer();
	}
	int ires = ClientSendMsg((char*)buff, 5);
	if (ires > 1)
	{
		sn = buff[0];
	}

	return false;
}
