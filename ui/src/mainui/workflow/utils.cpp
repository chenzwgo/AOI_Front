#include "utils.h"
#ifdef Q_OS_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <IPHlpApi.h>
#include <IcmpAPI.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#endif
#include <QDir>
#include <QFile>
#include <QSettings>
#include "commonapi.h"
#include "cimageprocess.h"
#include "g12common.h"
#include "filevariable/ifilevariable.h"
#include "iuiplugin.h"
#include "netdevui/isocketlistcfg.h"
#include "netdevui/isocketproxy.h"
#include "sendmessage.h"
#include "uicommon\commonutils.h"
#include "netdevui/isocketlistcfg.h"
#include "netdevui/isocketproxy.h"
#include "iodefine.h"
#include "signalcheck.h"
#include "rtdefine.h"
#include "station/istation.h"
#include "motion/imotion.h"
#include "trace.h"
#include "callsdk.h"
#include "sdkcommon\com\icommanager.h"
#include "sdkcommon\com\icom.h"

ICom* Utils::m_pPenmaCom = NULL;
ISocketproxy* Utils::m_pCSocketproxy = NULL;
Utils::Utils()
{

}

Utils::~Utils()
{

}


bool Utils::myping( const char *pIP, int nOverTime )
{
	bool bPing = false;

	if (NULL == pIP)
	{
		return bPing;
	}
#ifdef Q_OS_WIN
	// 创建ping句柄
	HANDLE hIcmp = IcmpCreateFile();

	if (NULL == hIcmp)
	{
		return bPing;
	}

	IPAddr pAdd = inet_addr(pIP);

	// ping数据包类型
	ICMP_ECHO_REPLY pData;

	// 发送“回声”数据
	IcmpSendEcho(hIcmp, pAdd, NULL, 0, NULL, &pData, sizeof(pData), nOverTime);

	if (IP_SUCCESS == pData.Status)
	{
		bPing = true;
	}

	// 关闭句柄
	IcmpCloseHandle(hIcmp);
#else
#endif
	return bPing;
}

bool Utils::GetNewFileNameAndTime( const QString strFilePath,QString& strFileName,QDateTime& createTime )
{
	bool bflag = false;
	QDir dir(strFilePath) ;
	if (!dir.exists())
	{
		return bflag;
	}
	foreach(QFileInfo mfi ,dir.entryInfoList())  
	{  
		if(mfi.isFile())  
		{  
			 QDateTime tmp = mfi.created();
			 if(tmp > createTime)
			 {
				createTime = tmp;
				strFileName = mfi.fileName();
				bflag = true;
			 }
		} 
	} 

	 
	return bflag;
}

bool Utils::GetFileContent( const QString strFileName, QString& strContent )
{
	if (!QFile::exists(strFileName))
	{
		return false;
	}
	QFile filetmp(strFileName)  ; 
    if (!filetmp.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
	strContent = filetmp.readAll();
	return true;
}

bool Utils::ParseFile( const QString strContent,const QString strkey,QString & strValue )
{
	QStringList strlist  = strContent.split("\r\n");
	foreach(QString strline,strlist)
	{
		if (strline.contains(strkey))
		{
			strValue = strline;
			return true;
		}
	}
	return false;;
}


QString Utils::getConfigPath()
{
    QSettings setting("config/globalSetting.ini", QSettings::IniFormat);
    setting.beginGroup("Machine");
    return setting.value("ConfigPath").toString();
}

QString Utils::getConfigFilePath( const QString& fileName )
{
    return getConfigPath() + "/" +fileName;
}

MachineType Utils::getMachineType()
{
    QSettings setting("config/globalSetting.ini", QSettings::IniFormat);
    setting.beginGroup("Machine");
    int type = setting.value("MachineType",UnKown).toInt();
    if (UnKown >= type || type > Machine_Tail ){
        return UnKown;
    }
    setting.endGroup();
    return MachineType(type);
}

void Utils::saveRawImgage(unsigned char * pImageData, unsigned int iwidth, unsigned iHeight, QString name)
{
	string  strPath = "D:\\IMAGE";
	IFileVariable* pfileVariable = getCameraSetCfgFileVariable();
	if (pfileVariable != NULL)
	{
		strPath = pfileVariable->getValue("image", "savepath", strPath.c_str()).toCString();
	}

	QString strDatePth = getCurrentProducsTimeStrPath();
	QString strSavePath = QString("%1\\%2").arg(QString::fromLocal8Bit(strPath.c_str())).arg(strDatePth);
	QDir dir(strSavePath);
	if (!dir.exists(strSavePath))
	{
		dir.mkpath(strSavePath);
	}

	strSavePath = strSavePath + "\\" + name;
	strSavePath += ".bmp";
	if (!QFile::exists(strSavePath))
	{
		CImageProcess::SaveImageToFile(pImageData, iwidth, iHeight, strSavePath);
	}
}

QString Utils::getBakPath()
{
    return getConfigPath() + "/bak";
}

QString Utils::getBakFilePath( const QString& fileName )
{
    return getBakPath() + "/" +fileName;
}


ISocketproxy* Utils::getPCSocketproxy(QString strPcName)
{
	if (NULL!=m_pCSocketproxy)
	{
		return m_pCSocketproxy;
	}
	ISocketproxy* pCSocketproxy = NULL;
	unsigned int proSN = CProductUnion::getInstance()->readCurrentDayPrSn();;
	IUIPlugin* pIUIPlugni = CommonUtils::getIUIPlugin("netdevui");
	if (pIUIPlugni == NULL)
	{
		return NULL;
	}
	ISocketClientListCfg* pISocketClientListCfg = NULL;
	pIUIPlugni->queryInterface(ISocketClientListCfg::IID_ISOCKETCLIENTLISTCFG, (void**)&pISocketClientListCfg);
	if (pISocketClientListCfg == NULL)
	{
		return NULL;
	}
	pCSocketproxy = pISocketClientListCfg->getSocketClientProxy(strPcName.toStdString());
	if (pCSocketproxy == NULL)
	{
		CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("获取通讯socket为空，请检查通讯IP与port")), UILOG_WARN);
		return NULL;
	}
	m_pCSocketproxy = pCSocketproxy;
	return pCSocketproxy;
}


int  Utils::WaitGlassLeave(const char*IOName, int iTimeoutMin,int iTimeoutMax,bool& bRun)
{
    uint64 iStartTime = JTime::CurrentMillisecond();
    uint64 iTime = 0;
    if (iTimeoutMin > iTimeoutMax)
    {
        iTimeoutMax = iTimeoutMin;
    }
    do
    {
        RETURN_V_IF(!bRun, 1111);
        iTime = JTime::CurrentMillisecond() - iStartTime;
        Sleep(10);
    } while (iTime < iTimeoutMin);
    do
    {
        iTime = JTime::CurrentMillisecond() - iStartTime;
        if (iTime > iTimeoutMax)//固定時間内不取走 報錯
        {
            return 1;
        }
        Sleep(10);
        RETURN_V_IF(!bRun, 1111);
    } while (SignalCheck::readIoEx(IOName, true, 100,0));

    return 0;
}


int Utils::checkSigForStop(const char* pIoName ,int itimeout,bool& bRun)
{

    const char *szIOName = pIoName;

    bool bIsMoving = false;
    Utils::getStation()->IsAxisMoving(0, bIsMoving);
    if(!bIsMoving)
    {
        return 0;
    }

    uint64 iStartTime = JTime::CurrentMillisecond();
    uint64 iEndTime = JTime::CurrentMillisecond();

    bool bStop = false;
    while ( bRun)
    {
        if (SignalCheck::readIo(szIOName))
        {
            iEndTime = JTime::CurrentMillisecond();
        }
        else
        {
            iStartTime = JTime::CurrentMillisecond();
        }

        int iEsTime= iEndTime- iStartTime;
        if( iEsTime > itimeout)
        {
            bStop = true;
            break;
        }
        else
        {
            Sleep(100);
        }

    }
    if(bStop && bRun)
    {
        Utils::getStation()->StopAxis(0);
    }
    return 0;

}

IStation *Utils::getStation()
{
    iMotion* m_pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
    RETURN_V_IF(NULL == m_pMotion,NULL);
    return  m_pMotion->GetStationInstance(TARGET_STATION);
}

void Utils::saveSn0Log(int sn)
{
    if (0 == sn)
    {
        QString strPath = getSaveResultFilePath();
        strPath += "/"+ getCurrentProducsTimeStrPath()+"/miss_product.log";
        QString msg = "sn=0";
        LOG_INFO_F_SIMP(strPath.toStdString().data(), msg.toLocal8Bit());
    }
}


bool Utils::CheckProductIsOnIO()
{
    bool flag = false;
	if (SignalCheck::readIo(DI_SNAP_SIG_0))
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("拍照光纤上有产品！"), UILOG_WARN);
		flag = true;
	}
    if (SignalCheck::readIo(DI_SNAP_SIG_1))
    {
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("拍照1光纤上有产品！"),UILOG_WARN);
        flag = true;
    }
    if (SignalCheck::readIo(DI_SNAP_SIG_2))
    {
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("拍照2光纤上有产品！"),UILOG_WARN);
        flag = true;
    }
    if (SignalCheck::readIo(DI_SNAP_SIG_3))
    {
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("拍照3光纤上有产品！"),UILOG_WARN);
        flag = true;
    }
    if (SignalCheck::readIo(DI_OUT_END_SIG))
    {
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("出料口光纤上有产品！"), UILOG_WARN);
        flag = true;
    }
    //if (SignalCheck::readIo(DI_IN_SIG))
    //{
    //    CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("入料口光纤上有产品！"), UILOG_WARN);
    //    flag = true;
    //}

    if (flag)
    {
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("请取走光纤上的产品再复位启动软件！"), UILOG_ERR);
    }
    return flag;
}

int Utils::callFunCheckAutoClearData()
{
	if (!isAutoDeleteData())
	{
		return -1;
	}
	int daysInterval = 7;
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{
		daysInterval = pFile->getValue(AUTO_DELETE_DATA_DAYS, 7).toInt();
	}
	QString dataSavePath = getSaveResultFilePath();
	clearFile(daysInterval, dataSavePath);
	return 0;
}

int Utils::callFunCheckAutoClearImage()
{
	if (!isAutoDeleteImage())
	{
		return -1;
	}
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	QString path;
	int daysInterval = 7;
	if (NULL != pFile)
	{
		path = pFile->getValue(IMAGE_SAVE_PATH).toCString().c_str();
		daysInterval = pFile->getValue(AUTO_DELETE_IMAGE_DAYS, 7).toInt();
	}
	clearFile(daysInterval, path);
	return 0;
}

void Utils::clearFile(int timeInterval, QString path)
{
	QDir dir(path);
	//遍历文件夹
	QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

	if (fileList.size() == 0)
	{
		return;
	}
	int interval = timeInterval;
	QDate curDate = QDate::currentDate();
	for (int i = 0; i < fileList.size(); ++i)
	{
		QFileInfo fileinfo = fileList.at(i);
		QString name = fileinfo.fileName();
		QDate date = fileList.at(i).created().date();
		int daysInterval = abs(curDate.daysTo(date));
		if (daysInterval >= interval)
		{
			QString rmParh = path + "/" + name;
			CommonUtils::RemoveDir(rmParh);
		}
	}
}

ICom * Utils::getPenmaCom()
{
	if (m_pPenmaCom == NULL)
	{
		string m_strComName;
		IFileVariable* pFileVariable = getCurrentRobotVariable();
		if (NULL != pFileVariable)
		{
			 m_strComName = pFileVariable->getValue(PENMA_COM_NAME).toCString();
		}

		IComManager* pIComManager = (IComManager*)GetSDKInstance("sdkcommon", IComManager::IID_COMMANAGER);
		if (pIComManager != NULL)
		{
			m_pPenmaCom = pIComManager->CreateCom(m_strComName.c_str());
			m_pPenmaCom->SetComName(m_strComName.c_str());
		}
	}
	if (m_pPenmaCom != NULL && !m_pPenmaCom->IsOpen())
	{
		if (m_pPenmaCom->OpenCom())
		{
			unsigned long BaudRate = 9600;
			unsigned long Parity = 0;
			unsigned long ByteSize = 8;
			unsigned long StopBits = 1;
			m_pPenmaCom->SetComState(BaudRate, Parity, ByteSize, StopBits);
		}
	}
	return m_pPenmaCom;
}

string Utils::getCamSuffix(int station)
{
	IFileVariable *pFile = getStationCfgVariable();
	string suffix = "";
	if (pFile != NULL)
	{
		char key[100] = { 0 };
		sprintf(key, "ImageStation_%d", station);
		suffix = pFile->getValue(key, "suffix").toCString();
	}
	return suffix;
}
