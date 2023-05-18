#include "cimagelogic.h"
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

CImageLogic::CImageLogic(IBufferManager* pCameraBufferManager):
	  m_pCameraBufferManager(pCameraBufferManager)
    , m_pThreadImagePro(NULL)
    , m_product(NULL)
    , m_pNotifyService(NULL)
    , m_bReset(false)
    , m_imageXOffset(0)
    , m_bIsFast(false)
{
    m_eStartImagePro.Init(false, false);
	m_eImageReadFinished.Init(true, false);
	m_eImageReading.Init(true, false);
    m_ImageLock.Init();
	m_EventLock.Init();
    m_bRunning = true;
	m_index = 0;
    m_pThreadImagePro = new JThreadTP<CImageLogic,void*,void>;   
}

void CImageLogic::setImageParam(string strSeqName, int index, string strSceneName)
{
    m_strSeqName = strSeqName;
    m_seqIndex = index;
	m_strScene = strSceneName;
    readConfig();

}

void CImageLogic::setEnable(bool bEnable)
{
	m_bIsEnable = bEnable;
}
void CImageLogic::readConfig()
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), m_strScene);
    if (pFileVariable != NULL)
    {
        m_bufferWidth = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH).toInt();
        m_bufferHeight = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT).toInt();
		m_bufferOverlapY = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY).toInt();
		m_blockSize = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKLINES).toInt(256);
        if (0 == m_blockSize)
        {
            m_blockSize = 256;
        }
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH, &m_bufferWidth);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, &m_bufferHeight);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY, &m_bufferOverlapY);
		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKLINES, &m_blockSize);
    }
    m_fileVariable = getCameraSetCfgFileVariable();
    if (m_fileVariable)
    {
        m_bProWhole = m_fileVariable->getValue(IMAGE_PARAM_IS_WHOLE, true).toBool();
        m_fileVariable->bindValue(IMAGE_PARAM_IS_WHOLE, &m_bProWhole);
    }
    pFileVariable = getCameraSetCfgFileVariable();
    if (pFileVariable)
    {
        m_bIsFast = pFileVariable->getValue(IMAGE_PARAM_FAST_GRAB_ENABLE).toBool(false);
        pFileVariable->bindValue(IMAGE_PARAM_FAST_GRAB_ENABLE, &m_bIsFast);
    }
}

CImageLogic::~CImageLogic()
{
    m_bRunning = false;
    m_eStartImagePro.SetEvent();
    if (NULL != m_pThreadImagePro);
    {
        while (m_pThreadImagePro->IsRunning())
        {
            Sleep(10);
        }
        delete m_pThreadImagePro;
        m_pThreadImagePro = NULL;
    }
}

bool CImageLogic::abortGrabImage()
{
	m_bRunning = false;
	return false;
}

void CImageLogic::ThreadImageProcess(void* param)
{
	long waitRes = JEVENT_WAIT_TIME_OUT;
	int iTimeOut = 6500;//6秒5内没收到图像则认为超时
	int iFrameSize = 0;
	int iIndex = 0;        //取到图像到的行数的索引
	int iOverIndex = 0;    //图像处理到的行数的索引
	int m_bufferOverlapY = 0;    //重叠行数
	int iStep = 0;         //没张小图在在大图中移动的行数
	int blockIndex = 0;    //图像处理块的索引
	bool bIsTimeOut = false;
    int  recvBufferHeight = m_bufferHeight;
	if (m_pCameraBufferManager == NULL)
	{
		return;
	}
	stImageInfo *pImage = NULL;
	stImageInfo *pImageArray[MAX_IMAGE_COUNT] = {0};

	IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), m_strScene);
	if (pFileVariable != NULL)
	{
		iTimeOut = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ACQ_TIME_OUT).toInt();
	}

	int iProductSN = m_product->getProductSn();
    //循环取像处理
	m_product->setStartGrapImageTime();//add20191021
    int iReadLine = m_blockSize;  //单次处理的图像行数
    if (iReadLine <= 0)
    {
        iReadLine = 256;
    }
    int iCurIndex = 0; //记录每个相机当前取到的位置
    if (iFrameSize != m_bufferWidth*iReadLine)
    {
        iFrameSize = m_bufferWidth*iReadLine;
    }
    //外部不做重叠处理
    iStep = iReadLine - m_bufferOverlapY;

    if (m_bIsFast)
    {
        recvBufferHeight = recvBufferHeight/2;
    }
	//LOG_INFO("Get Image(%d)", iIndex);
	int iGetChannelIndex = 0; //通道索引复位

	QTime startTime = QTime::currentTime();
	QTime endTime = startTime;
	int iSleepTime = 10;
	//不能被整除
	if (iIndex + iReadLine > recvBufferHeight)
	{
		iReadLine = recvBufferHeight - iIndex;
		iFrameSize = m_bufferWidth*iReadLine;
	}
			
	while (iCurIndex < recvBufferHeight && m_bRunning)
	{
		endTime = QTime::currentTime();
		if (startTime.msecsTo(endTime) >= iTimeOut)
		{
			bIsTimeOut = true;
			break;
		}

		const unsigned char* pData = NULL;
		pData = m_pCameraBufferManager->getBuffer(m_seqIndex, iCurIndex, iReadLine);
		if (NULL == pData)
		{
			Sleep(iSleepTime);
			continue;
		}

		iCurIndex += iReadLine;
		unsigned char *szImageBuffer = NULL;
		ImageBuffer* pImageBufferST = m_product->getImageBuffer(m_strSeqName.c_str(), m_index);
		if (pImageBufferST == NULL)
		{
			return;
		}

		szImageBuffer = pImageBufferST->rawData.img_data;
		int imageWidth = pImageBufferST->m_iWidth;
		int imageHeight = pImageBufferST->m_iHeight;
		//拼装图片
		if (NULL != szImageBuffer)
		{
			int imageOffset = m_imageXOffset + iIndex*imageWidth;   //整行偏移+拼图偏移
			unsigned char *szBufferOffset = NULL;
			const unsigned char *szSourceBuffer = NULL;
			for (int i = 0; i < iReadLine; i++)
			{
				//防呆判断
				szBufferOffset = szImageBuffer + (iIndex + i)*imageWidth;
				szSourceBuffer = pData + i*m_bufferWidth;
				memcpy(szBufferOffset, szSourceBuffer, m_bufferWidth*sizeof(unsigned char));
				//
				//
			}
		}
		iIndex += iReadLine;
        blockIndex++;
        pImageBufferST->rawData.img_blockIndex = blockIndex;
	}

	if (bIsTimeOut && NULL != m_product)
	{
		//停止取像， 将未采集完的图像填充取出
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("-----工位%1 通道%2 停止取像-m_pCameraBufferManager->abort()------").arg(QString::fromStdString(m_strScene)).arg(m_seqIndex), UILOG_WARN);
		m_pCameraBufferManager->abort();
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("--sn:%4----工位%3-取图像超时，部分图像丢失(通道%1)[%2]-------").arg(m_seqIndex).arg(iCurIndex).arg(QString::fromStdString(m_strScene)).arg(iProductSN), UILOG_WARN);
		m_product->setProcessErrCode(er_Grap);
		return;
	}
    ImageCorrection(m_strSeqName.c_str(), 0);
	if (iCurIndex < recvBufferHeight)
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("-sn:%3----工位%2-通道%1-取图丟失[已收行-%4]-------").arg(m_seqIndex).arg(QString::fromStdString(m_strScene)).arg(iProductSN).arg(iCurIndex), UILOG_WARN);
	}
	else
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("-sn:%3----工位%2-通道%1-取图结束[已收行-%4]-------").arg(m_seqIndex).arg(QString::fromStdString(m_strScene)).arg(iProductSN).arg(iCurIndex));
	}
}


void CImageLogic::ImageCorrection(const char *channelName, int index )
{
    if (NULL != m_product)
    {
		m_product->setChannelAllStatus(channelName, en_grapfinish,m_index);
		//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("设置状态采图完成,%1，工站%2，Sn(%3)").arg(channelName).arg(m_index).arg(m_product->getProductSn()),UILOG_WARN);
		CSendMessage::GetInstance()->sendUpdateUiStus(QString::number(m_product->getProductSn()), update_getimage);
    }
}
//
//void CImageLogic::SetImageInfo(/* bool *bIsEnableChannel, */int iCount, string *seq )
//{
//    if (/*NULL == bIsEnableChannel || */iCount <= 0 || NULL == seq)
//    {
//        return;
//    }
//
//    for (int i = 0; i < MAX_IMAGE_COUNT && i < iCount; i++)
//    {
//        //m_bIsEnableChannel[i] = bIsEnableChannel[i];
//        m_seqName[i] = seq[i];
//    }
//}

void CImageLogic::BeginReadImage(bool bOnLine/* = true*/)
{
	RETURN_IF(!m_bIsEnable);
	RETURN_IF(m_product==NULL);
    m_bReset = false;
    if (bOnLine)
    {
		if (m_pThreadImagePro != NULL)
		{
			m_bRunning = true;
			m_pThreadImagePro->StartThread(this, &CImageLogic::ThreadImageProcess, this);
		}
    }
	else
	{

	}

}

bool CImageLogic::bIsReadFinished()
{
    return !m_pThreadImagePro->IsRunning();
}

void CImageLogic::SetProductPtr( CImageProduct *product,int index)
{
    m_product = product;
	m_index = index;
    ImageBuffer* pImageBuffer = m_product->getImageBuffer(m_strSeqName.c_str(), m_index);
    if (pImageBuffer != NULL)
    {
        pImageBuffer->rawData.img_blockIndex=0;     //已接受到的数据块数
        return;
    }
}

void CImageLogic::Reset()
{
    m_bReset = true;
	m_bRunning = false;

    m_fileVariable = getCameraSetCfgFileVariable();
}
