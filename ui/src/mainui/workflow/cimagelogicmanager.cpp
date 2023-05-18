#include "cimagelogicmanager.h"
#include "maindefine.h"
#include "rtdefine.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/camera/ilinecamera.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "./vscameratools/cameramanager/icamerabuffermanager.h"
#include "trace.h"
#include "ProductUnion.h"
#include "cimageprocess.h"
#include "g12common.h"
#include "callsdk.h"
#include "sendmessage.h"
#include "commonapi.h"
#include "uicommon/commonutils.h"
#include "inotifyservice.h"
#include "stringcommon.h"
#include "rtdefine.h"
#include <QDir>
#include <QDateTime>

CImageLogicManager::CImageLogicManager(int index) :
    m_pCameraBufferManager(NULL),
	m_product(NULL),
	m_iIndex(index)

{
    for (int i = 0; i < SEQUE_NUMBER; i++)
    {
        m_strSeqName[i] = "";
        m_ImageProInstance[i] = NULL;
    }
    m_startGrabTime = QTime::currentTime();
    m_strScene = SString::IntToStr(m_iIndex);
}

CImageLogicManager::~CImageLogicManager()
{
    uninit();
}

void CImageLogicManager::init()
{
    bool bIsEnableChannel[CAMERA_NUMBER][SEQUE_NUMBER] = { 0 };
    int iCameraOn[CAMERA_NUMBER] = { 0 };       // 1111//对应的4位4个相机使用
        
    initConfig();
	IBufferManagerFactory* pIBufferManagerFactory = (IBufferManagerFactory*)GetSDKInstance("cameramanager", IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY);
	if (pIBufferManagerFactory != NULL)
	{
		m_pCameraBufferManager = pIBufferManagerFactory->createBufferManager(m_strScene);
	}
	
    if (NULL != m_pCameraBufferManager)
    {
        m_pCameraBufferManager->init();
    }

    string programs = getCurrentProgramRecipesList();
    vector<string> vect = SString::split(programs, ",");
    for (int j = 0; j < vect.size() && vect.size() <= SEQUE_NUMBER; j++)
    {
        if (m_bEnable[j])
        {
            m_strSeqName[j] = vect[j];
            if (NULL == m_ImageProInstance[j])
            {
                m_ImageProInstance[j] = new CImageLogic(m_pCameraBufferManager);
            }   
            m_ImageProInstance[j]->setImageParam(m_strSeqName[j], j, m_strScene);
            m_ImageProInstance[j]->setEnable(m_bEnable[j]);
        }
        else
        {
            if (NULL != m_ImageProInstance[j])
            {
                delete m_ImageProInstance[j];
                m_ImageProInstance[j] = NULL;
            }
        }
    }

}

void CImageLogicManager::initConfig()
{
    //通过配置, 获取相机分组信息
    IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(),m_strScene);
    if (NULL != pFileVariable)
    {
        m_imageHeight = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY).toInt();
        pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY, &m_imageHeight);
    }

    string strChannelNames = getCurrentProgramRecipesList().c_str();
    std::vector<std::string> ChannelNameList = SString::split(strChannelNames, ",");
	pFileVariable = getSequenceSetCfgFileVariable_new(getCurrentProgramName(), m_strScene);
    for (int i = 0; i < SEQUE_NUMBER && i < ChannelNameList.size(); i++)
    {
        m_bEnable[i] = pFileVariable->getValue(ChannelNameList.at(i), ENABELCHANNLE).toBool();
		pFileVariable->bindValue(ChannelNameList.at(i), ENABELCHANNLE, &m_bEnable[i]);
    }
}

void CImageLogicManager::uninit()
{
    if (NULL != m_pCameraBufferManager)
    {
        m_pCameraBufferManager->stopInit();
        m_pCameraBufferManager = NULL;
    }
    for (int i = 0; i < SEQUE_NUMBER; i++)
    {
        m_strSeqName[i] = "";
        if (NULL != m_ImageProInstance[i])
        {
            delete m_ImageProInstance[i];
            m_ImageProInstance[i] = NULL;
        }
    }
}

bool CImageLogicManager::setImageParam(int iProLength)
{
    bool bRet = true;
    RETURN_V_IF(NULL == m_pCameraBufferManager, false);
    m_pCameraBufferManager->updateImageBuffer();
    return bRet;
}

bool CImageLogicManager::setProductPtr(CImageProduct *product)
{
    bool bRet = true;
    RETURN_V_IF(NULL == m_pCameraBufferManager, false);
    for (int index = 0; index < SEQUE_NUMBER; index++)
    {
        if (NULL != m_ImageProInstance[index])
        {
            if (!m_ImageProInstance[index]->bIsReadFinished())
            {
                //CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("上次取图未完成"), UILOG_WARN);
                bRet = false;
            }

            m_ImageProInstance[index]->SetProductPtr(product,(int) m_iIndex); //TBM
        }
    }
	m_product = product;
    return bRet;
}

bool CImageLogicManager::startGetImage()
{
    bool bRet = true;
    RETURN_V_IF(NULL == m_pCameraBufferManager, false);
    QTime curTime = QTime::currentTime();
    //CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("%1").arg(m_startGrabTime.msecsTo(curTime)));
    m_startGrabTime = curTime;
    for (int index = 0; index < SEQUE_NUMBER; index++)
    {
        if (NULL != m_ImageProInstance[index])
        {
            if (!m_ImageProInstance[index]->bIsReadFinished())
            {
                CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("上次取图未完成"), UILOG_WARN);
                bRet = false;
            }
            m_ImageProInstance[index]->BeginReadImage();
        }
    }
	if (NULL != m_product)
	{
		IFileVariable *pFile = getCameraSetCfgFileVariable();
		unsigned long lSN = m_product->getProductSn();
		pFile->setValue(WORK_PRODUCT_SN, lSN);
	}
    return bRet;
}

bool CImageLogicManager::isAcqFinished()
{
    bool bRet = true;
    RETURN_V_IF(NULL == m_pCameraBufferManager, false);
    for (int index = 0; index < SEQUE_NUMBER; index++)
    {
        if (NULL != m_ImageProInstance[index])
        {
            if (!m_ImageProInstance[index]->bIsReadFinished())
            {
                bRet = false;
                break;
            }
        }
    }
    return bRet;
}

bool CImageLogicManager::resetImageProcess()
{
    bool bRet = true;
    RETURN_V_IF(NULL == m_pCameraBufferManager, false);
    bRet = m_pCameraBufferManager->reset();
    for (int index = 0; index < SEQUE_NUMBER; index++)
    {
        if (NULL != m_ImageProInstance[index])
        {
            m_ImageProInstance[index]->Reset();
        }
    }
    return bRet;
}

bool CImageLogicManager::triggerGrabImage()
{
    bool bRet = true;
    RETURN_V_IF(NULL == m_pCameraBufferManager, false);
    bRet = m_pCameraBufferManager->startProcess();
    return bRet;
}

bool CImageLogicManager::abortGrabImage()
{
    bool bRet = true;
    RETURN_V_IF(NULL == m_pCameraBufferManager, false);

    bRet = m_pCameraBufferManager->abort();
	for (int index = 0; index < SEQUE_NUMBER; index++)
	{
		if (NULL != m_ImageProInstance[index])
		{
			m_ImageProInstance[index]->abortGrabImage();
		}
	}
    return bRet;
}
