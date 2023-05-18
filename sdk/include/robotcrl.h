#ifndef  __ROBOTCRL_H__
#define  __ROBOTCRL_H__
#include <stdint.h>
#include <time.h>
#include<list>
#include "g12globalenv.h"
#include "g12commonexport.h"
#include "sdkcommon/socket/isocketclientmanager.h"
#include "sdkcommon/socket/isocketclient.h"
#include "rtdefine.h"
#include "jthreadt.h"
#include "jmutex.h"
#include <map>
#include <string>
using namespace std;

class IFileVariable;
class ISocketClient;
enum enResToRobot
{
	enRes_OK=0,
	enRes_RI,
	enRes_NG,
	enRes_SIZE,
};
enum enSendToRobotIndex
{
	enResSendTo_0 = 0,
	enResSendTo_1,
	enResSendTo_2,
	enResSendTo_3,
};
enum enCheckRobotResMark
{
	enCheckSend = 0,
	enCheckRobotMove,
};

typedef struct ST_result
{
	unsigned long result;
    unsigned long sn;
}result_t;


class RESOURCE_LIBRARY_EXPORT RobotCrl
{
   public:
	   static RobotCrl* GetInstance();
	   void   ReleaseInstance();
	   //启动线程一直发送数据到机器人/以防掉线/因为机器人再不接收数据0.5秒后自动断开
	   void StartThreadConnetRobot();
	   //停止
	   void StopRobotThread();
	   //
	   bool ReConnect();
	   bool DisConnect();
	   int ClientRecvMsg(char *pBuffer, unsigned int ibufferLength, const char *szMark = NULL);
	   int ClientSendMsg(const char *pData, unsigned int iDataLength);
	   int ClientSendResMsgToRobot(enResToRobot eRes, bool bALine = true);
	   int ClientCheckRobotRes(enResToRobot eRes, bool bALine = true);
	   bool IsClientOnline();
	   bool send_msg_to_plc(unsigned short itime);
	   //0 ok,1 ri ,2 ng
	   bool send_sn_and_res_to_xialiaoji(unsigned long sn, unsigned long res);
	   bool send_sn_to_xialiaoji(unsigned long sn);
	   bool get_xialiaoji_sn(unsigned long &sn);
private:
	RobotCrl();
	~RobotCrl();
	void getCfgParam();
	int SendRobotModubsData(bool bALine, enResToRobot eRes, enSendToRobotIndex Index);
	void ThreadClientRobot(void* param);
	unsigned short getCRCChk(BYTE* data, WORD length);
	int checkRobotRes(enCheckRobotResMark iMark,enResToRobot eRes, bool bALine = true);
	

	static RobotCrl* m_pSelfInstance;
	ISocketClient * m_pISocketClient;
	string m_aoiIp ;
	int    m_aoiport ;
	string m_robotip ;
	int    m_robotport ;
	JThreadTP<RobotCrl, void*, void>* m_pthread;// 
	bool m_bRun;
	JMutex m_mutex;

};
	











#endif
