#include "ProductMark.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "maindefine.h"
#include "filevariable/ifilevariable.h"
#include "signalcheck.h"
#include "sendmessage.h"

#include "sdkcommon\com\icommanager.h"
#include "sdkcommon\com\icom.h"
#include "callsdk.h"
#include "motion\imotion.h"
#include "motion\iioctrlinterface.h"
#include "motion\imotioncfg.h"
#include "g12common.h"
#include "trace.h"
#include "workflow\utils.h"
#include "commonapi.h"

#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"

CProductMark::CProductMark(unsigned long long ullsn)
{
	mMarkFlag.init();
	mMarkFlag.uSn = ullsn;
	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectAliasNameVec = p2->getDefectListInstance(0)->getDefectAliasNameList();
	}
}

CProductMark::~CProductMark()
{
	//
}
void CProductMark::setMsgToPrinter(QStringList strListMsg, enResFlag rstFlag)
{
	mMarkFlag.eFlag = rstFlag;
	mMarkFlag.strPrinterMsg = strListMsg;
	QString msg = "";
	for (int i = 0; i < strListMsg.count();i++)
	{
		msg += strListMsg.at(i);
	}
	QString strPath = getSaveResultFilePath();
	strPath += "/" + getCurrentProducsTimeStrPath() + "/penma_result.log";
	QString strLog = QString("sn = %1, msg = %2, resFlag = %3").arg(getSn()).arg(msg).arg(rstFlag);
	LOG_INFO_F_SIMP(strPath.toStdString().data(), strLog.toLocal8Bit());
}
void CProductMark::setDefectsVecToPrinter(vector<stDefectFeature*>* pDefectStrutVec, enResFlag rstFlag)
{

	LOG_INFO_F_SIMP("penma_result.log", "sn=%d,DefectSize=%d,resFlag=%d", getSn(), pDefectStrutVec->size(), rstFlag);
	mMarkFlag.eFlag = rstFlag;
	int imax = 5;// 最多打5个
	QString strMsg = "";
	for (size_t i = 0; i < pDefectStrutVec->size(); i++) 
	{
		stDefectFeature *pDefectRes = pDefectStrutVec->at(i);
		//根据等级//除去脏污记录打印信息-- 暂用临时数据  比如：区域 1 丝印   字符：1SY
		if (en_Smirch != pDefectRes->iDefectType && 5 <= pDefectRes->iDefectLevel)//临时使用5级，后续需要配置文件
		{
			
            if(pDefectRes->iDefectType >=0 && pDefectRes->iDefectType < m_defectAliasNameVec.size())
            {
				imax--;
                char buf[16]= {0};
                sprintf(buf,"%02d%s",pDefectRes->regionPos, m_defectAliasNameVec[pDefectRes->iDefectType]);
				strMsg += QString::fromLatin1(buf);
                mMarkFlag.strPrinterMsg.append(QString::fromLatin1(buf));
            }

		}
		if (imax<=0)
		{
			break;
		}
	}
	CSendMessage::GetInstance()->Send(strMsg, UILOG_WARN);
	return;
}
stPrMark CProductMark::getMarkFlag()
{
	return mMarkFlag;
}
unsigned long long CProductMark:: getSn()
{
	return mMarkFlag.uSn;
}


CPrMarkUnion *CPrMarkUnion::m_pInstance = NULL;
CPrMarkUnion::CPrMarkUnion(void)
{
    IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
    if (NULL != p2)
    {
        m_defectAliasNameVec = p2->getDefectListInstance(0)->getDefectAliasNameList();
    }
	m_mutexPrMarkList.Init();
	m_listPrMark.clear();
	mMark.init();
}
CPrMarkUnion::~CPrMarkUnion(void)
{
	releaseAllPrMark();
}
CPrMarkUnion * CPrMarkUnion::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CPrMarkUnion();
	}
	return m_pInstance;
}

void CPrMarkUnion::releaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CPrMarkUnion::createPrMark(unsigned long long ullsn)
{
	if (!isEnablePenma())
	{
		return;
	}
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	CProductMark* pProduct = new CProductMark(ullsn);;
    m_listPrMark.push_back(pProduct);
}

CProductMark* CPrMarkUnion::getFirstProductMark()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	CProductMark* pProduct = NULL;
	if (m_listPrMark.size() > 0)
	{
		pProduct = m_listPrMark.at(0);
	}
	return pProduct;
}
unsigned long long CPrMarkUnion::getFirstSn()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	if (m_listPrMark.size() > 0)
	{
		CProductMark* pProduct = m_listPrMark.at(0);
		return pProduct->getSn();
	}
	return 0;
}

void CPrMarkUnion::releaseFirstPrMark()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	if (m_listPrMark.size() > 0)
	{
		CProductMark* pProduct = m_listPrMark.at(0);
		m_listPrMark.pop_front();
		delete pProduct;
		pProduct = NULL;
	}

}

void CPrMarkUnion::releasePrMark(CProductMark* pProMark)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	foreach(CProductMark* pstProduct, m_listPrMark)
	{
		if (pstProduct == pProMark)
		{
			delete pstProduct;
			m_listPrMark.removeOne(pProMark);
			return;
		}
	}
}
void CPrMarkUnion::releaseAllPrMark()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	foreach(CProductMark* pstProduct, m_listPrMark)
	{
		if (pstProduct != NULL)
		{
			delete pstProduct;;
		}
	}
	m_listPrMark.clear();
}

int CPrMarkUnion::getPrMarkCnt()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	return m_listPrMark.size();
}

void CPrMarkUnion::setMsgToPrinter(unsigned long long ullsn, QStringList strListMsg, enResFlag rstFlag)
{
	if (!isEnablePenma())
	{
		return;
	}
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	foreach(CProductMark* pstProduct, m_listPrMark)
	{
		if (pstProduct != NULL && pstProduct->getSn() == ullsn)
		{
			return pstProduct->setMsgToPrinter(strListMsg, rstFlag);
		}
	}
}
void CPrMarkUnion::setUnionDefectsVecToPrinter(unsigned long long ullsn, vector<stDefectFeature*>* pDefectStrutVec, enResFlag rstFlag)
{
	if (!isEnablePenma())
	{
		return;
	}
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	foreach(CProductMark* pstProduct, m_listPrMark)
	{
		if (pstProduct != NULL && pstProduct->getSn() == ullsn)
		{
			return pstProduct->setDefectsVecToPrinter(pDefectStrutVec, rstFlag);
		}
	}
}
stPrMark CPrMarkUnion::getUnionMarkFlag(unsigned long long ullsn)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);
	foreach(CProductMark* pstProduct, m_listPrMark)
	{
		if (pstProduct != NULL && pstProduct->getSn() == ullsn)
		{
			return pstProduct->getMarkFlag();
		}
	}
	return mMark;
}

stPrMark CPrMarkUnion::getFirstPrMarkFlag()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrMarkList);

	if (m_listPrMark.size() > 0)
	{
		return m_listPrMark.at(0)->getMarkFlag();
	}
	return mMark;
}
void CPrMarkUnion::SnToPenmaData(long sn, QStringList& Penmadata)
{ 
	
	if (sn <= 9999)
	{
		QString penmaStr = snToStr(sn);
		Penmadata.append(penmaStr);
	}
	else
	{
		int bSn = sn / 10000;
		int sSn = sn % 10000;
		QString bPenmaStr = snToStr(bSn);
		QString sPenmaStr = snToStr(sSn);
		Penmadata.append(bPenmaStr);
		Penmadata.append(sPenmaStr);
	}
}
void CPrMarkUnion::DefectToPenmaData(stDefectFeature *pDefectRes, string strLine, QStringList& Penmadata)
{
	if (Penmadata.count()>=6)//当前最多打6个 SN 加五个缺陷
	{
		return;
	}
	if (((en_Smirch != pDefectRes->iDefectType && 6 <= pDefectRes->iDefectLevel) || (en_Smirch == pDefectRes->iDefectType && 7 == pDefectRes->iDefectLevel)) )
	{

        if (pDefectRes->iDefectType > 0 && pDefectRes->iDefectType < m_defectAliasNameVec.size())
		{
			char buf[16] = { 0 };
			int regionPos = pDefectRes->regionPos;
			if (regionPos >= 10 || regionPos<1)
			{
				regionPos = 9;
			}
            sprintf(buf, "%s%d%s", m_defectAliasNameVec[pDefectRes->iDefectType], regionPos, strLine.c_str());
			Penmadata.append(QString::fromLatin1(buf));
		}

	}
}

QString CPrMarkUnion::snToStr(long sn)
{

	QString penmaStr;
	if (sn > 9999)
		sn = 9999;
	if (sn >= 1000)
	{
		penmaStr = QString::number(sn);
	}
	else if (sn >= 100)
	{
		penmaStr = "0" + QString::number(sn);
	}
	else if (sn >= 10)
	{
		penmaStr = "00" + QString::number(sn);
	}
	else if (sn >= 1)
	{
		penmaStr = "000" + QString::number(sn);
	}
	else
	{
		penmaStr = "0000" + QString::number(sn);
	}
	return penmaStr;

}
/////////////////////////////////
//////////////////////////////////////
CPrPrinterCtr *CPrPrinterCtr::m_pInstance = NULL;
CPrPrinterCtr::CPrPrinterCtr(void)
{
	m_bRun = false;
	m_itimeMax = 1000;
	m_ResultFlag = res_OK3;
	m_pIo = NULL;
	mMark.init();
	m_pthread = NULL;
	m_pthread = new JThreadTP<CPrPrinterCtr, void*, void>;

	IFileVariable* pFileVariable = getCurrentRobotVariable();
	if (NULL != pFileVariable)
	{
		m_bEnalbe = pFileVariable->getValue(PENMA_ENABLE, true).toBool();
		m_strComName = pFileVariable->getValue(PENMA_COM_NAME).toCString();
		m_idelayTime = pFileVariable->getValue(PENMA_DELAY_TIMES, 1000).toInt();
	}

	IIOCtrlInterface* pIo = NULL;
	iMotion* pImotion = NULL;
    const char* ioName = "do_pen_out";
	pImotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);

	if (pImotion != NULL)
		m_pIo = pImotion->GetIOCtrlInterface();

}
CPrPrinterCtr::~CPrPrinterCtr(void)
{
	m_bRun = false;
	if (NULL != m_pthread)
	{
		do
		{
			Sleep(1);
		} while (m_pthread->IsRunning());
		delete m_pthread;
		m_pthread = NULL;
	}
}
CPrPrinterCtr * CPrPrinterCtr::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CPrPrinterCtr();
	}
	return m_pInstance;
}

void CPrPrinterCtr::releaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CPrPrinterCtr::ThreadPrinterCtr(void* param)
{
	//喷码代码
	IFileVariable* pFileVariable = getCurrentRobotVariable();
	if (NULL != pFileVariable)
	{
		m_bEnalbe = pFileVariable->getValue(PENMA_ENABLE, true).toBool();
		m_strComName = pFileVariable->getValue(PENMA_COM_NAME).toCString();
		m_idelayTime = pFileVariable->getValue(PENMA_DELAY_TIMES, 1000).toInt();
	}
	//
	QString strPath = getSaveResultFilePath();
	strPath += "/" + getCurrentProducsTimeStrPath() + "/penma_com_result.log";
	//
    stPrMark* pstPrMark = (stPrMark*)(param);
    const char* ioName = "do_pen_out";
	ICom* pCom = Utils::getPenmaCom();
	if (pCom != NULL)
	{

		char msg[4][64] = { 0 };
		toPenmaMsg(pstPrMark->strPrinterMsg, msg);
		for (int i = 0; i < 4; i++)
		{
			char buff[64] = { 0 };
			buff[0] = 0x02;
            sprintf(buff + 1, "SCM00%d:::0:1%s:", i + 1, msg[i]);
			buff[strlen(buff)] = 0x03;

			int ilenght = pCom->WriteCom((void*)buff, strlen(buff));
			//LOG
			QString strlog = QString("WriteCom data:%1;WriteData len:%2;Res len:%3").arg(buff).arg(strlen(buff)).arg(ilenght);
			LOG_INFO_F_SIMP(strPath.toStdString().data(), strlog.toLatin1());
			//LOG
			//
			//
			if (ilenght <0)
			{
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("WriteCom失败！"), UILOG_WARN);
			}
			else
			{
				Sleep(100);
				char rcvBuff[128] = { 0 };
				int iReclength = pCom->ReadCom(rcvBuff, 128);
				if (iReclength >0)
				{
					int irecv = rcvBuff[0];
					if (rcvBuff[0] == 0x06)
					{
						CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("发送指令成功！"));
					}
					else
					{
						//
						char buf[16] = { 0 };
						sprintf(buf, "ReadCom Res Error!iReclength:%d:data;%d:%d:%d:%d", iReclength, rcvBuff[0], rcvBuff[1], rcvBuff[2], rcvBuff[3]);
						LOG_INFO_F_SIMP(strPath.toStdString().data(), buf);
						//
						CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("接受返回码错误(%1)").arg(buf), UILOG_WARN);

					}
				}
				else
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("ReadCom 错误"), UILOG_WARN);
				}
			}
		}    
	}
    Sleep(50);
    //通知外部触发
	if (m_pIo != NULL)
	{
		//m_idelayTime
		QTime startTime = QTime::currentTime();
		QTime endTime = QTime::currentTime();
		do
		{
			Sleep(50);
			endTime = QTime::currentTime();
		} while (startTime.msecsTo(endTime)<m_idelayTime);
		//
		m_pIo->writeio(ioName, true);
        Sleep(50);
        m_pIo->writeio(ioName, false);
	}

    // 等待喷码完成信号 1
    QTime startTime = QTime::currentTime();
    QTime endTime = QTime::currentTime();

    char finshbuf[4] = {0};
    int iReclength = 0;
    bool bReadFinsishFlag = false;;
    do
    {
        int ilen = pCom->ReadCom(finshbuf+iReclength, 3-iReclength);
        if (ilen >0)
        {
            iReclength += ilen;
            if(finshbuf[0] == 0x02 && finshbuf[1] == 0x31 && finshbuf[2] == 0x03)
            {

                bReadFinsishFlag = true;
                break;
            }
        }
        else
        {
            Sleep(50);
        }
        endTime = QTime::currentTime();
    } while (startTime.msecsTo(endTime) < 1000);

    if(bReadFinsishFlag)
    {
        LOG_INFO_F_SIMP(strPath.toStdString().data(), QString::fromLocal8Bit("已经等待到喷码完成字符 0x02 0x31 0x03").toStdString().data());
    }
    else
    {
        LOG_INFO_F_SIMP(strPath.toStdString().data(), QString::fromLocal8Bit("未读到喷码完成信号").toStdString().data());
    }

    if (pCom != NULL)
    {
        pCom->CloseCom();
    }
}

void CPrPrinterCtr::toPenmaMsg(QStringList & msgList, char penmsg[4][64])
{
	int msgNumber = msgList.size();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < msgNumber && j <64; j++)
		{
			QString msg = msgList.at(j);
			if (msg.length() == 4)
			{
				penmsg[i][j] = msg.at(i).toLatin1();
			}
		}
	}
}

bool CPrPrinterCtr::startPrinterCtr(stPrMark eFlag)
{
    CPrPrinterCtr::ThreadPrinterCtr(&eFlag);
//	if (NULL != m_pthread)
//	{
//		if (!m_pthread->IsRunning())
//		{
//			m_bRun = true;
//			mMark.init();
//			mMark = eFlag;//喷码信息传递
//			m_pthread->StartThread(this, &CPrPrinterCtr::ThreadPrinterCtr, this);
//			return true;
//		}
//	}
	return false;
}


