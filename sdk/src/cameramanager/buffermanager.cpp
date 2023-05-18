#include "buffermanager.h"
#include "stdarg.h"
#include "filevariable/ifilevariable.h"
#include "g12common.h"
#include "sdkcore.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/camera/ilinecamera.h"
#include "rtdefine.h"
#include "stringcommon.h"
#include "trace.h"
#include "filevariable/ifilevariable.h"

CBufferManager *CBufferManager::m_pInstance = NULL;

CBufferManager::CBufferManager(string SceneName) : m_fnTriggerFunction(NULL)
    , m_pInParam(NULL)
    , m_currentChannel(0)
    , m_bIsContinuations(false)
	, m_bIsFast(false)
	, m_SceneName(SceneName)
{
    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            m_ImageProInstance[i][j] = NULL;
        }
    }
	memset(m_bIsEnableChannel, 0, sizeof(m_bIsEnableChannel));
	memset(m_iEnableChannelCnt, 0, sizeof(m_iEnableChannelCnt));
	memset(m_iChannelEnableCamIndex, -1, sizeof(m_iChannelEnableCamIndex));
    m_proLock.Init();
    init();
    m_pRealTimeThread = new JThreadTP<CBufferManager, void*, void>;
}

CBufferManager::~CBufferManager()
{
    m_bIsContinuations = false;
    while (NULL != m_pRealTimeThread && m_pRealTimeThread->IsRunning())
    {
        Sleep(100);
    }
    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[i][j])
            {
                delete m_ImageProInstance[i][j];
                m_ImageProInstance[i][j] = NULL;
            }
        }
    }
    delete m_pRealTimeThread;
}

void CBufferManager::loadConfig()
{
	// 读取相机配置配方
	IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), m_SceneName);;
	if (pFileVariable != NULL)
	{
		m_bufferOverlapX = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPX).toInt();
		m_bufferOverlapY = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPY).toInt();
		m_bufferWidth = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH).toInt();
		m_bufferHeight = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT).toInt();
		m_camWidth = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONX).toInt();
		m_camHeight = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY).toInt();
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPX, &m_bufferOverlapX);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPY, &m_bufferOverlapY);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH, &m_bufferWidth);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, &m_bufferHeight);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONX, &m_camWidth);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY, &m_camHeight);
	}

	pFileVariable = getCameraSetCfgFileVariable();
	if (pFileVariable)
	{
		m_bIsFast = pFileVariable->getValue(IMAGE_PARAM_FAST_GRAB_ENABLE).toBool(false);
		pFileVariable->bindValue(IMAGE_PARAM_FAST_GRAB_ENABLE, &m_bIsFast);
	}
	// 读取光源通道配方	
	memset(m_bIsEnableChannel, 0, sizeof(m_bIsEnableChannel));
	memset(m_iEnableChannelCnt, 0, sizeof(m_iEnableChannelCnt));
	memset(m_iChannelEnableCamIndex, -1, sizeof(m_iChannelEnableCamIndex));
	memset(m_bIsEnable, 0, sizeof(m_bIsEnable));
	string strChannelNames = getCurrentProgramRecipesList().c_str();
	std::vector<std::string> ChannelNameList = SString::split(strChannelNames, ",");
	IFileVariable *pFileSequence = getSequenceSetCfgFileVariable_new(getCurrentProgramName(), m_SceneName);
	//初始化通道配置
	for (int k = 0; k < SEQUE_NUMBER && k < ChannelNameList.size(); k++)
	{
		int iType;
		int iCamEableIndex = 0;
		if (0 == getSqueLightTypeAndCameraIndex(getSeqNameWithID(k), iType, iCamEableIndex, m_SceneName) //获取当前通道里使能的相机组索引
			&& iCamEableIndex > 0 && iCamEableIndex <= CAMERA_NUMBER)
		{
			m_iEnableChannelCnt[iCamEableIndex - 1]++;				//同一组相机使能的通道计数
			m_bIsEnableChannel[iCamEableIndex - 1][k] = true;		//指定组内对应相机是否使能
			m_iChannelEnableCamIndex[k] = iCamEableIndex-1;
		}
		m_bIsEnable[k] = pFileSequence->getValue(ChannelNameList.at(k), ENABELCHANNLE).toBool();
		pFileSequence->bindValue(ChannelNameList.at(k), ENABELCHANNLE, &m_bIsEnable[k]);
	}
	///////////////////////////
	pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), m_SceneName);

	bool bEnableGrap = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_GRAP_ENABLE).toBool(true);
	for (int i = 0; i < CAM_GROUP_COUNT; i++)
	{
		for (int j = 0; j < CAM_INDEX_COUNT; j++)
		{
			m_strCamName[i][j] = "";
		}
		string KeyName = "ImageConfig_" + SString::IntToStr(i);
		int iCamCnt = pFileVariable->getValue(KeyName, ACQUISITION_CAM_CNT).toInt();
		bool bCam1Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM1_ENABLE).toBool();
		bool bCam2Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM2_ENABLE).toBool();
		bool bCam3Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM3_ENABLE).toBool();
		bool bCam4Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM4_ENABLE).toBool();
		string strCam1Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM1_SN).toCString("");
		string strCam2Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM2_SN).toCString("");
		string strCam3Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM3_SN).toCString("");
		string strCam4Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM4_SN).toCString("");

		if (bCam1Enable && iCamCnt > 0 && bEnableGrap)
		{
			m_strCamName[i][0] = strCam1Sn;
		}
		if (bCam2Enable && iCamCnt > 1 && bEnableGrap)
		{
			m_strCamName[i][1] = strCam2Sn;
		}
		if (bCam3Enable && iCamCnt > 2 && bEnableGrap)
		{
			m_strCamName[i][2] = strCam3Sn;
		}
		if (bCam4Enable && iCamCnt > 3 && bEnableGrap)
		{
			m_strCamName[i][3] = strCam4Sn;
		}
	}
}
bool CBufferManager::init()
{
	loadConfig();

    CISAPUGuard<JMutex> autoLock(m_proLock);
    bool bRet = true;  

	
    updateImageBuffer();

    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            m_strCamName[i][j] = "";
        }
        if (0 != getCamNameList(i, m_strCamName[i],m_SceneName))
        {
            continue;
        }
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (m_strCamName[i][j].size() > 0)
            {
                if (NULL == m_ImageProInstance[i][j])
                {
                    m_ImageProInstance[i][j] = new CImageProcess(m_strCamName[i][j].c_str());
                }
                else
                {
                    m_ImageProInstance[i][j]->setCamName(m_strCamName[i][j].c_str());
                }
                m_ImageProInstance[i][j]->setParam(j*(m_camWidth - m_bufferOverlapX), m_bufferOverlapY,m_bufferWidth,m_bufferHeight);//设置图像offset 

                stImageInfo *pImageAddr[SEQUE_NUMBER] = { NULL };
                //将使能的通道图片内存地址传入处理对象
                int index = 0;
                for (int k = 0; k < SEQUE_NUMBER; k++)
                {
                    if (m_bIsEnableChannel[i][k])
                    {
                        pImageAddr[index++] = &m_imageInfo[k];
                    }
                }
                m_ImageProInstance[i][j]->setBufferInfo(m_iEnableChannelCnt[i], pImageAddr);
            }
        }
    }
    return bRet;
}


bool CBufferManager::startProcess()
{
    bool bRet = true;

    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[i][j])
            {
                bRet = m_ImageProInstance[i][j]->startProcess();
                BREAK_IF(!bRet);
            }
        }
        BREAK_IF(!bRet);
    }
    return bRet;
}

void CBufferManager::getBufferSize(int &width, int &height)
{
    width = m_bufferWidth;
    height = m_bufferHeight;
}

const unsigned char * CBufferManager::getBuffer(int iChannel, int lineOffset /*= -1*/, int iLineNumb /*= 1*/)
{
    bool bFinished = false;
    const unsigned char *pData = NULL;
    CImageProcess  *pProcess = NULL;
    int iType;
    int iCamIndex = 0;
    int indexOffsetLine = lineOffset > 0 ? lineOffset : 0;
    int iIndexEnd = lineOffset + iLineNumb;
    if (iLineNumb <= 0 || iIndexEnd > m_bufferHeight)
    {
        iIndexEnd = m_bufferHeight;
		if (m_bIsFast)
		{
			iIndexEnd = m_bufferHeight / 2;
		}
    }
	if (iChannel >= SEQUE_NUMBER)
	{
		return NULL;
	}
	iCamIndex = m_iChannelEnableCamIndex[iChannel];		// 这个通道 使能哪一组相机
    int iCamChannel = -1;									// 这一组相机如果多个通道使用,这个iChannel通道取图的所以
    for (int i = 0; i <= iChannel; i++)
    {
        if (m_iChannelEnableCamIndex[i] == iCamIndex)
        {
            iCamChannel++;
        }
    }
    RETURN_V_IF(iCamChannel < 0, NULL);

    for (int index = 0; index < CAM_INDEX_COUNT; index++)
    {
        if (iCamIndex >= 0 && iCamIndex  < CAM_GROUP_COUNT)
        {
            pProcess = m_ImageProInstance[iCamIndex][index];
            if (NULL != pProcess)
            {
                if (pProcess->isGrabFinished(iCamChannel, iIndexEnd))
                {
                    bFinished = true;
                }
                else
                {
                    bFinished = false;
                    break;
                }
            }
        }
    }
    if (bFinished)
    {
        pData = m_imageInfo[iChannel].pixelR + indexOffsetLine*m_bufferWidth;
    }
    return pData;
}

const unsigned char * CBufferManager::getBufferAdress(int iChannel)
{
    return m_imageInfo[iChannel].pixelR;
}

bool CBufferManager::continuations(int channel, FnDataRecvFunction fnTriggerFunction, void *pInParam)
{
    bool bRet = true;
    m_fnTriggerFunction = fnTriggerFunction;
    m_pInParam = pInParam;
    int iType;
    int iCamIndex = m_iChannelEnableCamIndex[channel];
    for (int j = 0; j < CAM_INDEX_COUNT; j++)
    {
        if (iCamIndex >= 0 && NULL != m_ImageProInstance[iCamIndex][j])
        {
            bRet = m_ImageProInstance[iCamIndex][j]->continuations(/*CBufferManager::CallbackGrapFunction, this*/);
            BREAK_IF(!bRet);
        }
    }
    if (bRet)
    {
        m_currentChannel = channel;
		m_bIsContinuations = true;
        m_pRealTimeThread->StartThread(this, &CBufferManager::ThreadRealTimePro, this);     
    }
    else
    {
        stopContinuations(channel);
    }
    return bRet;
}

bool CBufferManager::stopContinuations(int channel)
{
    bool bRet = false;
    int iType;
	int iCamIndex = m_iChannelEnableCamIndex[channel];
    for (int j = 0; j < CAM_INDEX_COUNT; j++)
    {
        if (iCamIndex >= 0 && NULL != m_ImageProInstance[iCamIndex][j])
        {
            bRet = m_ImageProInstance[iCamIndex][j]->stopContinuations();
            BREAK_IF(!bRet);
        }
    }
    m_bIsContinuations = false;
    return bRet;
}

bool CBufferManager::reset()
{
    bool bRet = false;
	init();
    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[i][j])
            {
                bRet = m_ImageProInstance[i][j]->reset();
                BREAK_IF(!bRet);
            }
        }
        BREAK_IF(!bRet);
    }
    return bRet;
}

bool CBufferManager::abort()
{
    bool bRet = false;

    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[i][j])
            {
                bRet = m_ImageProInstance[i][j]->abort();
                BREAK_IF(!bRet);
            }
        }
        BREAK_IF(!bRet);
    }
    return bRet;
}

bool CBufferManager::isBusying()
{
    bool bRet = false;

    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[i][j])
            {
                bRet = m_ImageProInstance[i][j]->isBusying();
                BREAK_IF(bRet);
            }
        }
        BREAK_IF(bRet);
    }
    return bRet;
}

bool CBufferManager::isInitFinished()
{
    bool bRet = true;
    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[i][j])
            {
                bRet = m_ImageProInstance[i][j]->isInitFinished();
                BREAK_IF(!bRet);
            }
        }
        BREAK_IF(!bRet);
    }
    return bRet;
}

bool CBufferManager::updateImageBuffer()
{
    for (int k = 0; k < SEQUE_NUMBER; k++)
    {
        if (m_bIsEnable[k])
        {
            if (m_imageInfo[k].width != m_bufferWidth || m_imageInfo[k].height != m_bufferHeight)
            {
                if (NULL != m_imageInfo[k].pixelR)
                {
                    delete[]m_imageInfo[k].pixelR;
                    m_imageInfo[k].pixelR = NULL;
                }
                m_imageInfo[k].width = m_bufferWidth;
                m_imageInfo[k].height = m_bufferHeight;
            }
            if (NULL == m_imageInfo[k].pixelR)
            {
                m_imageInfo[k].pixelR = new unsigned char[m_bufferWidth * m_bufferHeight]; 
                LOG_INFO("===============updateImageBuffer[%d] [%d * %d].===============", k, m_bufferWidth, m_bufferHeight);
            }
        }
    }
    return true;
}

bool CBufferManager::stopInit()
{
    bool bRet = true;
    for (int i = 0; i < CAM_GROUP_COUNT; i++)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[i][j])
            {
                bRet = m_ImageProInstance[i][j]->stopInit();
                BREAK_IF(!bRet);
            }
        }
        BREAK_IF(!bRet);
    }
    return bRet;
}

int CBufferManager::getCamStatus(int iGroupIndex)
{
    int iRet = ILineCamera::STATUS_UNINIT;
    RETURN_V_IF(iGroupIndex > CAM_GROUP_COUNT, iRet);
    for (int j = 0; j < CAM_INDEX_COUNT; j++)
    {
        if (NULL != m_ImageProInstance[iGroupIndex][j])
        {
            iRet = m_ImageProInstance[iGroupIndex][j]->getCamStatus();
            RETURN_V_IF(iRet < CAM_GROUP_COUNT, iRet);
        }
		else
		{
			return ILineCamera::STATUS_CAM_IDLE;
		}
    }
    return iRet;
}

CBufferManager * CBufferManager::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CBufferManager("");
    }
    return m_pInstance;
}

void CBufferManager::releaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void CBufferManager::ThreadRealTimePro(void* param)
{
    int iType;
	int iCamIndex = m_iChannelEnableCamIndex[m_currentChannel];
    bool bIsFinished = false;
    if (iCamIndex < 0 || iCamIndex >= CAM_GROUP_COUNT)
    {
        return; //TBM 报错
    }
	Sleep(500);
    while (m_bIsContinuations)
    {
        for (int j = 0; j < CAM_INDEX_COUNT; j++)
        {
            if (NULL != m_ImageProInstance[iCamIndex][j] && m_ImageProInstance[iCamIndex][j]->isRecvDataFinished())
            {
                bIsFinished = true;
            }
            BREAK_IF(bIsFinished);
        }
        if (bIsFinished)
        {
            m_fnTriggerFunction(m_pInParam, m_imageInfo[m_currentChannel].pixelR, m_bufferWidth, 1);

            for (int j = 0; j < CAM_INDEX_COUNT; j++)
            {
                if (NULL != m_ImageProInstance[iCamIndex][j])
                {
                    m_ImageProInstance[iCamIndex][j]->resetFinishedStatus();
                }
            }
        }
        else  //TBM
        {
            ;
        }
    }
}


BufferManagerFactory *BufferManagerFactory::m_pBufferManagerFactory = NULL;

BufferManagerFactory::BufferManagerFactory()
{

}

BufferManagerFactory::~BufferManagerFactory()
{
	CISAPUGuard<JMutex> autoLock(m_mapMutex);
	map<string, CBufferManager*>::iterator it = m_mapBufferManager.begin();
	while (it != m_mapBufferManager.end())
	{
		CBufferManager* pBufferManager = it->second;
		if (pBufferManager != NULL)
		{
			delete pBufferManager;
		}
		it++;
	}
	m_mapBufferManager.clear();
}

BufferManagerFactory * BufferManagerFactory::getInstance()
{
	if (NULL == m_pBufferManagerFactory)
	{
		m_pBufferManagerFactory = new BufferManagerFactory;
	}
	return m_pBufferManagerFactory;
}

void BufferManagerFactory::releaseInstance()
{
	if (NULL != m_pBufferManagerFactory)
	{
		delete m_pBufferManagerFactory;
		m_pBufferManagerFactory = NULL;
	}
}

IBufferManager* BufferManagerFactory::createBufferManager(string SceneName)
{
	if (SceneName.length() ==0)
	{
		return NULL;
	}
	CISAPUGuard<JMutex> autoLock(m_mapMutex);
	IBufferManager* pBufferManager = NULL;
	map<string, CBufferManager*>::iterator it = m_mapBufferManager.find(SceneName);
	if (it != m_mapBufferManager.end())
	{
		pBufferManager = it->second;
	}
	else
	{
		CBufferManager* pCBufferManager = new CBufferManager(SceneName);
		m_mapBufferManager.insert(std::pair<string, CBufferManager*>(SceneName, pCBufferManager));
		pBufferManager = pCBufferManager;
	}
	return pBufferManager;
}

void BufferManagerFactory::relealseBufferManager(IBufferManager* pBufferManager)
{
	CISAPUGuard<JMutex> autoLock(m_mapMutex);
	map<string, CBufferManager*>::iterator it = m_mapBufferManager.begin();
	while (it != m_mapBufferManager.end())
	{
		if (it->second == pBufferManager)
		{
			if (pBufferManager != NULL)
			{
				delete pBufferManager;
			}
			m_mapBufferManager.erase(it);
			return;
		}
		it++;
	}
}

void BufferManagerFactory::relealseBufferManager(string SceneName)
{
	if (SceneName.length() == 0)
	{
		return ;
	}
	CISAPUGuard<JMutex> autoLock(m_mapMutex);
	IBufferManager* pBufferManager = NULL;
	map<string, CBufferManager*>::iterator it = m_mapBufferManager.find(SceneName);
	if (it != m_mapBufferManager.end())
	{
		pBufferManager = it->second;
		if (pBufferManager != NULL)
		{
			delete pBufferManager;
		}
		m_mapBufferManager.erase(it);
	}

	return ;
}
