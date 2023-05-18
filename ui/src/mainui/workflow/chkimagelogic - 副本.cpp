#include "chkimagelogic.h"
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
#include "g12utils.h"

CHkImageLogicManager::CHkImageLogicManager(int index) :
	m_product(NULL),
	m_pCameraInfo(),
	m_pCameraFactory(NULL),
	m_pCameraInstance(NULL),
	m_bRunning(true),
	m_bufferWidth(0),
    m_bufferHeight(0),
    m_iIndex(index)
{
	for (int i = 0; i < SEQUE_NUMBER; i++)
	{
		m_strSeqName[i] = "";
		m_ImageProInstance[i] = NULL;
	}
	m_iChannelCount = getSequenceNumber(SString::IntToStr(m_iIndex));
	m_iAreaiChanelCount = 1;
	m_iOverSize = 0;
	char key[100] = { 0 };
	sprintf(key, "ImageStation_%d", m_iIndex);
	IFileVariable* file = getStationCfgVariable();
	if (file != NULL) //面阵能通道个数使用配置文件指定
	{
		m_iAreaiChanelCount = file->getValue(key, ACQUISITION_MAX_CHANNEL_CNT).toInt();
		m_iOverSize = file->getValue(key, "oversize","0").toInt();
	}
	m_startGrabTime = QTime::currentTime();
    m_strScene = SString::IntToStr(m_iIndex);
	m_iGetImageIndex = 0;

}

VS_CAMERA_INFO CHkImageLogicManager::ReadConfig()
{
	VS_CAMERA_INFO param;

    IFileVariable* pFile = getAcquisitionFileVariable(getCurrentProgramName(),m_strScene);

    string camName = "ImageConfig_0";
	param.strOutName = camName;
	param.SerialNumber = pFile->getValue(camName, HKCAM_ATTRIBUTE_SN).toCString();
	param.height = pFile->getValue(camName, ACQUISITION_IMAGEHEIGHT).toLong();
	param.width = pFile->getValue(camName, ACQUISITION_IMAGEWIDTH).toInt();
	param.exposure = pFile->getValue(camName, HKCAM_ATTRIBUTE_EXPOSURE).toInt();
	param.gain = pFile->getValue(camName, HKCAM_ATTRIBUTE_GAIN).toInt();
    param.TriggerMode =(CameraTriggerMode) pFile->getValue(camName, HKCAM_ATTRIBUTE_TRIGMODE).toInt();
    param.Activation =(CameraTriggerActivation) pFile->getValue(camName, HKCAM_ATTRIBUTE_TRIGEDGE).toInt();

	m_bufferWidth = param.width;
	m_bufferHeight = param.height;

	return param;
}

CHkImageLogicManager::~CHkImageLogicManager()
{
	m_bRunning = false;
	uninit();
}

void CHkImageLogicManager::init()
{

    m_pCameraInfo = ReadConfig();
    m_pCameraFactory = (ICameraFactory*)GetSDKInstance("hiksourcecamera", ICameraFactory::IID_CAMERAFACTORY);
    if (NULL != m_pCameraFactory)
    {
        m_pCameraInstance = m_pCameraFactory->CreateCamera(m_pCameraInfo.SerialNumber.c_str(), 0);
        m_pCameraInstance->SetCameraInParam(m_pCameraInfo);
        m_pCameraInstance->SetTriggerFunction(CallbackGrapFunction, this);
    }
	// 初始化buffer
    updateImageBuffer();
    string strChannelNames = getCurrentProgramRecipesList().c_str();
    vector<string> vect= SString::split(strChannelNames, ",");

	for (int j = 0; j < m_iChannelCount && j < SEQUE_NUMBER; j++)
	{
		m_strSeqName[j] = vect[j];
		if (NULL == m_ImageProInstance[j])
		{
            m_ImageProInstance[j] = new CHkImageLogic(m_strSeqName[j], j);
            m_ImageProInstance[j]->setImageParam(m_strSeqName[j], j,m_strScene);
			m_ImageProInstance[j]->setBufferInfo(j, &m_ImageInfo[j]);//每一个相机都有多通道
		}
		else
		{
            m_ImageProInstance[j]->setImageParam(m_strSeqName[j], j,m_strScene);
			m_ImageProInstance[j]->setBufferInfo(j, &m_ImageInfo[j]);//每一个相机都有多通道
		}
	}
}

void CHkImageLogicManager::CallbackGrapFunction(void * obj, stImageInfo * buffer)
{
	if (NULL != obj && NULL != buffer)
	{
		CHkImageLogicManager* pInstence = (CHkImageLogicManager*)obj;
		//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("HK CallbackGrapFunction "), UILOG_WARN);
		if (NULL != pInstence && NULL != pInstence->m_pCameraInstance)
		{
			if (pInstence->m_iGetImageIndex >= pInstence->m_iAreaiChanelCount)
			{
				pInstence->m_iGetImageIndex = 0;
			}
			int index = 0;
			//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("HK取图%1").arg(pInstence->m_iGetImageIndex), UILOG_WARN);
			//if (pInstence->m_ImageInfo[index].pixelR != NULL && pInstence->m_ImageInfo[index].width== buffer->width && pInstence->m_ImageInfo[index].height == buffer->height)
			if (pInstence->m_ImageInfo[index].pixelR != NULL && pInstence->m_ImageInfo[index].width == buffer->width&& ((pInstence->m_ImageInfo[index].height/ pInstence->m_iAreaiChanelCount)== buffer->height))
			{
				//memcpy(pInstence->m_ImageInfo[index].pixelR, buffer->pixelR, pInstence->m_ImageInfo[index].width* pInstence->m_ImageInfo[index].height);

				unsigned char *szBufferOffset = NULL;

				int overSize = pInstence->m_iOverSize*buffer->width*pInstence->m_iGetImageIndex;;

				int ilen = buffer->width* buffer->height*pInstence->m_iGetImageIndex- overSize;

				szBufferOffset = pInstence->m_ImageInfo[index].pixelR + ilen;//地址偏移一个图像索引个数

				memcpy(szBufferOffset, buffer->pixelR , buffer->width* buffer->height);

				//CImageProcess::SaveImageToFile(buffer->pixelR, buffer->width, buffer->height, QString("D://test%1.bmp").arg(pInstence->m_iGetImageIndex));

				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("HK取图%1 memcpy OK").arg(pInstence->m_iGetImageIndex), UILOG_WARN);
			}
			else
			{

				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("HK pInstence->m_ImageInfo[index].width = %1 ;buffer->width =%2").arg(pInstence->m_ImageInfo[index].width).arg(buffer->width), UILOG_WARN);
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("HK pInstence->m_ImageInfo[index].height = %1 ;buffer->height =%2").arg(pInstence->m_ImageInfo[index].height).arg(buffer->height), UILOG_WARN);
			}
			pInstence->m_iGetImageIndex++;

			if (pInstence->m_iGetImageIndex == pInstence->m_iAreaiChanelCount)
			{
				//CImageProcess::SaveImageToFile(pInstence->m_ImageInfo[index].pixelR, pInstence->m_ImageInfo[index].width, pInstence->m_ImageInfo[index].height, "D://testT.bmp");
				pInstence->m_ImageProInstance[index]->pushImageBuffer(&pInstence->m_ImageInfo[index]);
			}		
		}
	}
}

void CHkImageLogicManager::uninit()
{
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

bool CHkImageLogicManager::setProductPtr(CImageProduct *product)
{
	bool bRet = true;
	for (int index = 0; index < m_iChannelCount; index++)
	{
		if (NULL != m_ImageProInstance[index])
		{
			if (!m_ImageProInstance[index]->bIsReadFinished())
			{
				bRet = false;
			}
            m_ImageProInstance[index]->SetProductPtr(product,m_iIndex ); //TBM
		}
	}
	m_product = product;
    return bRet;
}

bool CHkImageLogicManager::setImageParam(int iMinLength)
{
	updateImageBuffer();
    return true;
}

bool CHkImageLogicManager::startGetImage()
{
	bool bRet = true;
	QTime curTime = QTime::currentTime();
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("HK-%1").arg(m_startGrabTime.msecsTo(curTime)));
	m_startGrabTime = curTime;

	for (int index = 0; index < m_iChannelCount; index++)
	{
		if (NULL != m_ImageProInstance[index])
		{
			if (!m_ImageProInstance[index]->bIsReadFinished())
			{
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("HK上次取图未完成"), UILOG_WARN);
				bRet = false;
			}
			m_ImageProInstance[index]->BeginReadImage();
		}
	}
	return bRet;
}

bool CHkImageLogicManager::isAcqFinished()
{
	bool bRet = true;
	for (int index = 0; index < m_iChannelCount; index++)
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

bool CHkImageLogicManager::resetImageProcess()
{
	bool bRet = true;
	for (int index = 0; index < m_iChannelCount; index++)
	{
		if (NULL != m_ImageProInstance[index])
		{
			m_ImageProInstance[index]->Reset();
		}
	}
	m_iGetImageIndex = 0;
	char key[100] = { 0 };
	sprintf(key, "ImageStation_%d", m_iIndex);
	IFileVariable* file = getStationCfgVariable();
	if (file != NULL) //面阵能通道个数使用配置文件指定
	{
		m_iOverSize = file->getValue(key, "oversize", "0").toInt();
	}
    return bRet;
}

bool CHkImageLogicManager::triggerGrabImage()
{
	m_pCameraInfo = ReadConfig();
	m_pCameraFactory = (ICameraFactory*)GetSDKInstance("hiksourcecamera", ICameraFactory::IID_CAMERAFACTORY);
	if (NULL != m_pCameraFactory)
	{
		m_pCameraInstance = m_pCameraFactory->CreateCamera(m_pCameraInfo.SerialNumber.c_str(), 0);
		if (m_pCameraInstance != NULL)
		{
			m_pCameraInstance->SetCameraInParam(m_pCameraInfo);
			m_pCameraInstance->SetTriggerFunction(CallbackGrapFunction, this);
		}
	}
    return true;
}

bool CHkImageLogicManager::abortGrabImage()
{
	bool bRet = false;

	if (NULL != m_pCameraInstance)
	{
		m_pCameraInstance->Reset();

		bRet = true;
	}

	return bRet;
}

bool CHkImageLogicManager::updateImageBuffer()
{
	for (int k = 0; k < SEQUE_NUMBER && k < m_iChannelCount; k++)
	{
		if (m_ImageInfo[k].width != m_bufferWidth || m_ImageInfo[k].height != m_bufferHeight)
		{
			if (NULL != m_ImageInfo[k].pixelR)
			{
				delete[]m_ImageInfo[k].pixelR;
				m_ImageInfo[k].pixelR = NULL;
			}
			if (NULL != m_ImageInfo[k].pixelG)
			{
				delete[]m_ImageInfo[k].pixelG;
				m_ImageInfo[k].pixelG = NULL;
			}
			if (NULL != m_ImageInfo[k].pixelB)
			{
				delete[]m_ImageInfo[k].pixelB;
				m_ImageInfo[k].pixelB = NULL;
			}
			m_ImageInfo[k].width = m_bufferWidth;
			m_ImageInfo[k].height = m_bufferHeight;
		}
		if (NULL == m_ImageInfo[k].pixelR)
		{
			m_ImageInfo[k].pixelR = new unsigned char[m_bufferWidth * m_bufferHeight];
			LOG_INFO("===============updateImageBuffer[R][%d] [%d * %d].===============", k, m_bufferWidth, m_bufferHeight);
		}
	}
	return true;
}

bool CHkImageLogicManager::GetSetExporeTime(int expore ,double &time)
{	 
	double CycleTime;
	QString strStartTime = QDateTime::currentDateTime().toString("hh:mm:ss:zzz");
	m_pCameraInfo.exposure = expore;
	bool rtn = m_pCameraInstance->SetCameraInParam(m_pCameraInfo);
	QString strCurTime = QDateTime::currentDateTime().toString("hh:mm:ss:zzz");
	time = QDateTime::fromString(strStartTime, "hh:mm:ss:zzz").msecsTo(QDateTime::fromString(strCurTime, "hh:mm:ss:zzz"));
	return rtn;
}

CHkImageLogic::CHkImageLogic(string strSeqName, int indexSeq) :
	m_strSeqName(strSeqName),
	m_seqIndex(indexSeq),
	m_iMaxChannelCount(0),
	m_bRunning(true),
	m_bReset(false),
	m_pThreadImagePro(NULL),
	m_pNotifyService(NULL),
	m_product(NULL)
{
	m_ImageLock.Init();
	m_EventLock.Init();
	m_eImageReading.Init(true, false);
	m_eStartImagePro.Init(false, false);
	m_eImageReadFinished.Init(false, false);

	m_pThreadImagePro = new JThreadTP<CHkImageLogic, void *, void>;
	m_pThreadImagePro->StartThread(this, &CHkImageLogic::ThreadImageProcess, this);
}

CHkImageLogic::~CHkImageLogic()
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

void CHkImageLogic::setImageParam(string strSeqName, int index, string strSceneName)
{
	m_strSeqName = strSeqName;
	m_seqIndex = index;
    m_strScene = strSceneName;
}

void CHkImageLogic::BeginReadImage(bool bOnLine /*= true*/)
{
	m_EventLock.Lock();
	m_bReset = false;
	if (bOnLine)
	{
		m_eStartImagePro.SetEvent();  //通知开始读取图像

	}
	else
	{
		

	}
	m_eImageReading.SetEvent();
	m_eImageReadFinished.ResetEvent();
	m_EventLock.Unlock();
}

bool CHkImageLogic::bIsReadFinished()
{
	bool bRet = true;
	m_EventLock.Lock();
	if (m_eImageReading.IsSiangaled())
	{
		bRet = false;
	}
	m_EventLock.Unlock();
	return bRet;
}

void CHkImageLogic::SetProductPtr(CImageProduct *product,int index )
{
	m_product = product;
    m_index = index;
}

void CHkImageLogic::Reset()
{
	m_bReset = true;
	m_eStartImagePro.ResetEvent();
	m_eImageReadFinished.ResetEvent();
	m_eImageReading.ResetEvent();
	clearImageBuffer();

}

void CHkImageLogic::ThreadImageProcess(void* param)
{
	long waitRes = JEVENT_WAIT_TIME_OUT;
	int iTimeOut = 5000;//5秒内没收到图像则认为超时
	bool bIsTimeOut = false;
	QTime startTime = QTime::currentTime();
	QTime endTime = startTime;
	int iSleepTime = 10;

	while (m_bRunning)
	{
		waitRes = JEvent::WaitForSingleObject(&m_eStartImagePro, 1000);  //收到开始采集信号后开始取图
		if (JEVENT_WAIT_OK == waitRes)
		{
			IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), m_strScene);
			if (pFileVariable != NULL)
			{
				iTimeOut = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ACQ_TIME_OUT).toInt();
			}
			stImageInfo* pData = NULL;
			QTime startTime = QTime::currentTime();
			bIsTimeOut = false;
			do 
			{
				endTime = QTime::currentTime();
				pData = popImageBuffer();			// 获取相机通道数据
				if (NULL == pData)
				{				
					if (startTime.msecsTo(endTime) >= iTimeOut)
					{
						bIsTimeOut = true;
						break;
					}
				}
				else
				{
					break;
				}
				Sleep(50);

				RETURN_IF(!m_bRunning);	//对象销毁时退出线程
				BREAK_IF(m_bReset);		//复位后退出当前处理
			} while (!bIsTimeOut);

			if (NULL != m_product)
			{
				RETURN_IF(!m_bRunning);	//对象销毁时退出线程
				ImageBuffer* pImageBufferST = NULL;
                pImageBufferST = m_product->getImageBuffer(m_strSeqName.c_str(), m_index);

				if (NULL != pImageBufferST && pData != NULL)
				{
					// 将数据拷到m_product里面

                    if (NULL != pImageBufferST->rawData.img_data)
					{
                        memcpy(pImageBufferST->rawData.img_data, pData->pixelR, pImageBufferST->rawData.img_height* pImageBufferST->rawData.img_width * sizeof(unsigned char));

						//CImageProcess::SaveImageToFile(pImageBufferST->rawData.img_data, pImageBufferST->rawData.img_width, pImageBufferST->rawData.img_height, "D://test3.bmp");
						//CImageProcess::SaveImageToFile(pData->pixelR, pImageBufferST->rawData.img_width, pImageBufferST->rawData.img_height, "D://test2.bmp");
					}

					BREAK_IF(!m_bRunning);//
					CONTINUE_IF(m_bReset);//复位后退出当前处理
					if (bIsTimeOut)
					{
						m_product->setProcessErrCode(er_Grap);
					}
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("-sn:%3--%2-(%1)取图结束-------").arg(m_seqIndex).arg(QString::fromStdString(m_strScene)).arg(m_product->getProductSn()));
				}
				else
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("-sn:%3--%2--(%1)取图超時-------").arg(m_seqIndex).arg(QString::fromStdString(m_strScene)).arg(m_product->getProductSn()));
				}
				// 将图像块丢入处理队列中 
				ImageCorrection(m_strSeqName.c_str(), 0);
				m_EventLock.Lock();
				m_eImageReadFinished.SetEvent();
				m_eImageReading.ResetEvent();
				m_EventLock.Unlock();
			}
		}
	}
}

void CHkImageLogic::ImageCorrection(const char *channelName, int index)
{
	if (NULL != m_product)
	{
        if (NULL != m_product)
        {
            m_product->setChannelAllStatus(channelName, en_grapfinish,m_index);
            CSendMessage::GetInstance()->sendUpdateUiStus(QString::number(m_product->getProductSn()), update_getimage);
        }
	}
}

void CHkImageLogic::readConfig()
{

}

void CHkImageLogic::clearImageBuffer()
{
	CISAPUGuard<JMutex> autoLock(m_imageBufferMutex);
	m_imageBuffer.clear();
}

stImageInfo* CHkImageLogic::popImageBuffer()
{
	stImageInfo* pstImageInfo = NULL;
	CISAPUGuard<JMutex> autoLock(m_imageBufferMutex);
	if (m_imageBuffer.size() >0)
	{
		pstImageInfo = m_imageBuffer.front();
		m_imageBuffer.pop_front();
	}
	return  pstImageInfo;
}

void CHkImageLogic::pushImageBuffer(stImageInfo* pstImageInfo)
{
	CISAPUGuard<JMutex> autoLock(m_imageBufferMutex);
	m_imageBuffer.push_back(pstImageInfo);
}

bool CHkImageLogic::setBufferInfo(int index, stImageInfo *pBuffer)
{
	if ((index > SEQUE_NUMBER || index < 0) || NULL == pBuffer)
	{
		return false;
	}
	return true;
}



