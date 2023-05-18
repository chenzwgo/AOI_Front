#include "frontpc.h"
#include <QMutex>
#include "utils.h"
#include "netdevui/isocketproxy.h"
#include "ProductUnion.h"
#include "errcode_def.h"
#include "sendmessage.h"
#include "outputDataSave.h"
static QMutex g_frontpcMutex;

CFrontPCtoBack *CFrontPCtoBack::m_pInstance = NULL;


CFrontPCtoBack::CFrontPCtoBack()
{

}

CFrontPCtoBack::~CFrontPCtoBack()
{

}



int CFrontPCtoBack::SendProductSN( unsigned int proSN )
{
    if (!m_bEnable)
    {
        return 0;
    }
    ISocketproxy* pCSocketproxy = Utils::getPCSocketproxy();
    if(pCSocketproxy == NULL)
    {
        return 2;
    }
    if(SYS_ERR_OK != pCSocketproxy->sendSynPrdunctSN(proSN))
    {
        CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("同步BackPC产品SN失败!")),UILOG_WARN);
        return 3;
    }
    return 0;
}

int CFrontPCtoBack::queryBackPcStatus(int &iStatus)
{
    //if (!m_bEnable)
    //{
    //    return 0;
    //}

    ISocketproxy* pCSocketproxy = Utils::getPCSocketproxy();
    if (pCSocketproxy == NULL)
    {
        return 2;
    }
    if (SYS_ERR_OK != pCSocketproxy->sendQuerySysStatus(iStatus))
    {
        CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("查询BackPC运行状态失败")), UILOG_WARN);
        return 3;
    }
    return 0;
}

int CFrontPCtoBack::queryBackPcProductSN(unsigned int &proSN)
{
    if (!m_bEnable)
    {
        return 0;
    }

    ISocketproxy* pCSocketproxy = Utils::getPCSocketproxy();
    if (pCSocketproxy == NULL)
    {
        return 2;
    }
    if (SYS_ERR_OK != pCSocketproxy->sendQueryProductSN(proSN))
    {
        CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("查询BackPC 产品SN失败")), UILOG_WARN);
        return 3;
    }
    return 0;

}
int CFrontPCtoBack::sendProductInfo(unsigned int proSN, int iProductRes)
{
    if (!m_bEnable)
    {
        return 0;
    }
    ISocketproxy* pCSocketproxy = Utils::getPCSocketproxy();
    if (pCSocketproxy == NULL)
    {
        return -1;
    }
    char bodybuf[256] = { 0 };
    sprintf(bodybuf, "\"productSn\": \"%d\",\"res\": \"%d\"", proSN, iProductRes);

    if (SYS_ERR_OK != pCSocketproxy->sendRequest(PACKET_FUN_SEND_PRODUCT_RESULT, bodybuf))
    {
        CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("发送产品结果失败")));
    }

    return 0;
}



CFrontPCtoBack * CFrontPCtoBack::getInstance()
{
    QMutexLocker locker(&g_frontpcMutex);
    if (NULL == m_pInstance)
    {
        m_pInstance = new CFrontPCtoBack;
    }

    return m_pInstance;
}

void CFrontPCtoBack::releaseInstance()
{
    QMutexLocker locker(&g_frontpcMutex);
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}
void CFrontPCtoBack::pushBackSynResult(unsigned int iSn, int iRest, int iNgOrRiDefectType, string str2Dcode)
{
	QMutexLocker locker(&m_synResultocker);

	stProductDataRes FrontMsg;
	FrontMsg.sn = to_string(iSn);
	FrontMsg.iNgOrRiDefectType = iNgOrRiDefectType;
	FrontMsg.ResultFlag = (enResFlag)iRest;
	FrontMsg.str2Dcode = str2Dcode;

	//
	QString str = "RI";
	if (FrontMsg.ResultFlag == res_OK || FrontMsg.ResultFlag == res_OK2)
	{
		str = "OK";
	}
	else if (FrontMsg.ResultFlag == res_Ng)
	{
		str = "NG";
	}
	QString strLog = QString("ReceiveFront sn:%1;Res:%2;defectType:%3;str2dcod:%4").arg(iSn).arg(iRest).arg(iNgOrRiDefectType).arg(QString::fromStdString(str2Dcode));
	outputDataSave::GetInstance()->saveLog("ReceiveFrontRes", strLog);
	QString msg = QString::fromLocal8Bit("收到A机SN:%1  Res: %2").arg(iSn).arg(str);
	outputDataSave::GetInstance()->saveLog("ReceiveFrontRes", msg);
	//
	m_proResult.insert(iSn, FrontMsg);
	//mQListFrontRes.push_back(FrontMsg);
}
bool CFrontPCtoBack::getForntMsgBySn(int iSn, stProductDataRes &PrMsg)
{
	QMutexLocker locker(&m_synResultocker);
	if (m_proResult.contains(iSn))
	{
		PrMsg = m_proResult.value(iSn);
		QString msg = QString::fromLocal8Bit("取出A机SN:%1  Res: %2  剩余前端结果: %3").arg(QString::fromStdString(PrMsg.sn)).arg(PrMsg.ResultFlag).arg(mQListFrontRes.count());
		CSendMessage::GetInstance()->sendRobotLog(msg);
		m_proResult.remove(iSn);
		return true;
	}
	return false;
}
void CFrontPCtoBack::ClearFrontMsg()
{
	QMutexLocker locker(&m_synResultocker);
	m_proResult.clear();
}
