#include "imageProcess.h".h"
#include "stdarg.h"
#include "filevariable/ifilevariable.h"
#include "g12common.h"
#include "sdkcore.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/camera/ilinecamera.h"
#include "rtdefine.h"
#include "stringcommon.h"
#include "trace.h"

CImageProcess::CImageProcess(string strCamName) : m_strCamName(strCamName)
    , m_pCameraInstance(NULL)
    , m_pCameraManager(NULL)
    , m_bIsRealTime(false)
    , m_iChannelCount(0)
    , m_camXOffset(0)
    , m_camYOffset(0)
    , m_currentChannalIndex(0)
    , m_pInitThread(NULL)
    , m_bIsInitRunning(false)
	, m_bMirror(false)
{
    m_proLocker.Init();
    m_eventDataRecv.Init(true, false);
    m_eventGetCamInstance.Init(true, false);
    memset(m_curRecvIndex, 0, sizeof(m_curRecvIndex));
    m_pInitThread = new JThreadTP<CImageProcess, void *, void>;
    for (int i = 0; i < m_iChannelCount; i++)
    {
        m_imageInfo[i] = NULL;
    }
    init();
}

CImageProcess::~CImageProcess()
{
    m_bIsInitRunning = false;
    m_eventGetCamInstance.SetEvent(); //置初始化完成信号
    while (NULL != m_pInitThread && m_pInitThread->IsRunning())
    {
        Sleep(100);
    }
	if (m_pInitThread !=NULL )
	{
		delete m_pInitThread;
		m_pInitThread = NULL;
	}
}

void CImageProcess::setCamName(string strCamName)
{
    m_strCamName = strCamName;
    init();
}

void CImageProcess::init()
{
    CISAPUGuard<JMutex> locker(m_proLocker);
    m_pCameraManager = (ILineCameraFactoryManage*)sdkcore::queryPriInterface("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
    m_bIsInitRunning = true;
    m_pCameraInstance = NULL;
    m_eventGetCamInstance.ResetEvent();
    m_pInitThread->StartThread(this, &CImageProcess::ThreadInitPro, this);
}

bool CImageProcess::waitForInitFinished()
{
    bool bRet = false;
    int iTimeOut = 6000;
    if (JEVENT_WAIT_OK == JEvent::WaitForSingleObject(&m_eventGetCamInstance, iTimeOut))
    {
        bRet = true;
    }

    return bRet;
}

void CImageProcess::ThreadInitPro(void* param)
{
    while (m_bIsInitRunning && NULL == m_pCameraInstance)
    {
        if (NULL == m_pCameraManager)
        {
            m_pCameraManager = (ILineCameraFactoryManage*)sdkcore::queryPriInterface("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
        }
        m_pCameraInstance = m_pCameraManager->GetCameraInstance(m_strCamName);
        if (NULL != m_pCameraInstance)
        {
            m_eventGetCamInstance.SetEvent(); //置初始化完成信号
			LINE_CAM_PARAM Caminfo;
			m_pCameraInstance->GetCameraInParam(Caminfo);
				m_bMirror = Caminfo.bIsInvert;
        }
		Sleep(5);
    }
}

void CImageProcess::setParam(int xOffset, int yOffset ,int ibufferWidth, int ibufferHeight)
{
    m_camXOffset = xOffset;
    m_camYOffset = yOffset;
	m_bufferWidth = ibufferWidth;
	m_bufferHeight = ibufferHeight;
}

bool CImageProcess::setBufferInfo(int iCount, stImageInfo **pBuffer)
{
    if (iCount > SEQUE_NUMBER || iCount <= 0 || NULL == pBuffer)
    {
        return false;
    }
    m_iChannelCount = iCount;   //相机通道数
    for (int i = 0; i < iCount; i++)
    {
        m_imageInfo[i] = pBuffer[i];
    }
    return true;
}

//TBM
bool CImageProcess::startProcess()
{
    bool bRet = false;
    waitForInitFinished();
    if (NULL != m_pCameraInstance)
    {
        m_bIsRealTime = false;
        m_pCameraInstance->SetTriggerFunction(CImageProcess::CallbackGrapFunction, this);
        //设置长度为block size,
        memset(m_curRecvIndex, 0, sizeof(m_curRecvIndex));
        m_currentChannalIndex = 0;
        bRet = m_pCameraInstance->StartAcq();
    }
    return bRet;
}

//TBM
bool CImageProcess::continuations(/*FnLineTriggerFunction fnTriggerFunction, void *pInParam*/)
{
    bool bRet = false;
    waitForInitFinished();
    if (NULL != m_pCameraInstance)
    {
        memset(m_curRecvIndex, 0, sizeof(m_curRecvIndex));
        m_currentChannalIndex = 0;
        m_pCameraInstance->SetTriggerFunction(CImageProcess::CallbackGrapFunction, this);
        //设置长度为1,
        if (m_pCameraInstance->Continuations(true, m_iChannelCount))
        {
            m_bIsRealTime = true;
        }
        //m_fnTriggerFunction = fnTriggerFunction;
        //m_pInParam = pInParam;
        m_eventDataRecv.ResetEvent();
        //m_pRealTimeThread->StartThread(this, &CImageProcess::ThreadRealTimePro, this);
        bRet = true;
    }
    return bRet;
}

bool CImageProcess::stopContinuations()
{
    bool bRet = false;
    waitForInitFinished();
    if (NULL != m_pCameraInstance)
    {
        bRet = m_pCameraInstance->Continuations(false);

        m_pCameraInstance->SetTriggerFunction(CImageProcess::CallbackGrapFunction, this);
        //恢复长度为block size,
        m_bIsRealTime = false;
    }
    return bRet;
}

bool CImageProcess::isRecvDataFinished()
{
    bool bRet = false;
    if (JEVENT_WAIT_OK == JEvent::WaitForSingleObject(&m_eventDataRecv, 100))
    {
        bRet = true;
    }
    return bRet;
}

void CImageProcess::resetFinishedStatus()
{
    m_eventDataRecv.ResetEvent();
}

bool CImageProcess::reset()
{
    bool bRet = false;
    waitForInitFinished();
    if (NULL != m_pCameraInstance)
    {
        bRet = m_pCameraInstance->AbortAcq();
		m_pCameraInstance->Reset();
        memset(m_curRecvIndex, 0, sizeof(m_curRecvIndex));
    }
    return bRet;
}

bool CImageProcess::abort()
{
    bool bRet = false;
    waitForInitFinished();
    if (NULL != m_pCameraInstance)
    {
        bRet = m_pCameraInstance->AbortAcq();
        for (int i = 0; i < m_iChannelCount; i++)
        {
            m_curRecvIndex[i] = m_bufferHeight + m_camYOffset;
        }
    }
    return bRet;
}

bool CImageProcess::isBusying()
{
    bool bRet = false;
    waitForInitFinished();
    if (NULL != m_pCameraInstance)
    {
        bRet = m_pCameraInstance->IsContinuations();
    }
    return bRet;
}

bool CImageProcess::isGrabFinished(int iChannel, int index /*= -1*/)
{
    bool bRet = false;
    waitForInitFinished();
    if (NULL != m_pCameraInstance)
    {
        if (m_curRecvIndex[iChannel] >= index + m_camYOffset)
        {
            bRet = true;
        }
    }
    return bRet;
}

bool CImageProcess::isInitFinished()
{
    CISAPUGuard<JMutex>  locker(m_proLocker);
    return m_eventGetCamInstance.IsSiangaled();
}

bool CImageProcess::stopInit()
{
    m_bIsInitRunning = false;
    m_eventGetCamInstance.SetEvent(); //置初始化完成信号
    return true;
}

int CImageProcess::getCamStatus()
{
    int iRet = ILineCamera::STATUS_UNINIT;
    if (NULL != m_pCameraInstance)
    {
        iRet = m_pCameraInstance->GetCamStatus();
    }
    return iRet;
}

void CImageProcess::CallbackGrapFunction(void *pInstance, unsigned char *buffer, int width, int height)
{
    CImageProcess *pCamInstance = (CImageProcess *)pInstance;
    if (NULL != pCamInstance)
    {
        pCamInstance->insertBuffer(buffer, width, height);
    }
}

void CImageProcess::insertBuffer(const unsigned char *buffer, int width, int height)
{
    CISAPUGuard<JMutex>  locker(m_proLocker);

    //实时采集时UI未处理完时放弃后面接收的数据
    if (m_bIsRealTime && m_eventDataRecv.IsSiangaled() || NULL == m_imageInfo[m_currentChannalIndex])
    {
        return;
    }
    if (m_bIsRealTime)
    {
        memset(m_curRecvIndex, 0, sizeof(m_curRecvIndex));
    }
    const unsigned char *pSourceBuf = NULL;
    unsigned char *pDestBuf = NULL;
    int      iRecvOffset = 0;
    //实时显示只取一行
    for (int iLine = 0; iLine < height; iLine++)
    {
        if (m_curRecvIndex[m_currentChannalIndex] < (m_bufferHeight + m_camYOffset)
            && m_curRecvIndex[m_currentChannalIndex] < m_imageInfo[m_currentChannalIndex]->height)
        {
            pSourceBuf = buffer + iLine * width;
            iRecvOffset = m_curRecvIndex[m_currentChannalIndex] * m_bufferWidth + m_camXOffset;
            pDestBuf = m_imageInfo[m_currentChannalIndex]->pixelR + iRecvOffset;

			if (m_bMirror)
			{
				for (int i = 0; i < width; i++)
				{
					pDestBuf[i] = pSourceBuf[width - i-1];
				}
			}
			else
			{
				memcpy(pDestBuf, pSourceBuf, width);
			}
            
        }
        m_curRecvIndex[m_currentChannalIndex]++;
        m_currentChannalIndex++;
        m_currentChannalIndex = m_currentChannalIndex%m_iChannelCount;
        //每个通道都有数据后，实时采集时置实时取完状态
        if (m_bIsRealTime && 0 == m_currentChannalIndex)
        {
            m_eventDataRecv.SetEvent();
            m_curRecvIndex[m_currentChannalIndex] = 0; //实时显示时只刷新第一行数据
        }
    }
	//LOG_INFO("===============insertBuffer[%s] [%d / %d / %d / %d / %d / %d / %d / %d].===============",m_strCamName.c_str(), m_curRecvIndex[0], m_curRecvIndex[1], m_curRecvIndex[2], m_curRecvIndex[3],m_curRecvIndex[4], m_curRecvIndex[5], m_curRecvIndex[6], m_curRecvIndex[7]);
	
}
