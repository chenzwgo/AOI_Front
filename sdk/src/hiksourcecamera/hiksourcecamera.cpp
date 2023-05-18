#include "hiksourcecamera.h"
#include "jfile.h"
#include "rtdefine.h"
#include "jtime.h"

#include <iostream>
//software trigger
static const _GUID VCDElement_ST = { 0xFDB4003C, 0x552C, 0x4FAA, { 0xB8, 0x7B, 0x42, 0xE8, 0x88, 0xD5, 0x41, 0x47 } };
static bool m_bInitLib = false;

#define IMG_CAMERA_PARAM  "./plugins/cameraplugin/hikcamera.txt"

#define MAX_BUFFER 256

// Specify the number of buffers to be used.
#define NUM_BUFFERS 1

// 相机抓图回调, 主要用来获取图片
void   __stdcall CHIKSourceCameraPlugin::ImageCallBack(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
	if (((CHIKSourceCameraPlugin*)pUser)->PushBackImage(pData, pFrameInfo))
	{
		if (((CHIKSourceCameraPlugin*)pUser)->m_fnTriggerFunction != NULL)
		{
			((CHIKSourceCameraPlugin*)pUser)->m_fnTriggerFunction(((CHIKSourceCameraPlugin*)pUser)->m_pInParam, &((CHIKSourceCameraPlugin*)pUser)->m_pImageInfo);
		}
	};
}

void   __stdcall CHIKSourceCameraPlugin::ExceptionCallBack(unsigned int nMsgType, void* pUser)
{
	if (0x00008001 == nMsgType)
	{
		((CHIKSourceCameraPlugin*)pUser)->m_bOpen = false;
	}
}

CHIKSourceCameraPlugin::CHIKSourceCameraPlugin(const char *pszGUID)
	: m_fnTriggerFunction(NULL)
	, m_pInParam(NULL)
	, m_errorCode(0)
	, m_bIsColorFilter(false)
	, m_bIsRunning(false)
	, m_bIsContinuations(false)
	, m_pDeviceInfo(NULL)
	, m_pHikCamera(NULL)
	, m_nSaveImageType(MV_Image_Bmp)
	, m_pBufForSaveImage(NULL)
	, m_nBufSizeForSaveImage(0)
	, m_bOpen(false)
{
	m_CameraInfo.SerialNumber = pszGUID;

	m_lock.Init();
	m_imageLock.Init();
}

CHIKSourceCameraPlugin::~CHIKSourceCameraPlugin(void)
{
	if (IsOpen())
	{
		Close();
	}
	Reset();
}

bool CHIKSourceCameraPlugin::Open()
{
	if (IsOpen())
	{
		return true;
	}

	m_lock.Lock();
	int iRet = 0;
	do
	{
		JFile file(IMG_CAMERA_PARAM);
		int iRotationAngle = 0;
		if (file.exists())
		{
			if (file.open("r"))
			{
				char szBuf[MAX_BUFFER] = { 0 };
				if (0 < file.read(szBuf, MAX_BUFFER - 1))
				{
					iRotationAngle = atoi(szBuf);
				}
				file.close();
			}
		}
		MV_CC_DEVICE_INFO_LIST mvDeviceList;
		memset(&mvDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
		CMyCamera::EnumDevices(&mvDeviceList);
		MV_CC_DEVICE_INFO* pTargetDeviceInfo = NULL;
		for (int i = 0; i < mvDeviceList.nDeviceNum; i++)
		{
			MV_CC_DEVICE_INFO* pDeviceInfo = mvDeviceList.pDeviceInfo[i];
			if (NULL == pDeviceInfo)
			{
				continue;
			}

			string strSerialNumber;
			if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
			{
				strSerialNumber = (char*)pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber;

			}
			else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
			{
				strSerialNumber = (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber;

			}
			if (strSerialNumber == m_CameraInfo.SerialNumber)
			{
				pTargetDeviceInfo = pDeviceInfo;
				break;
			}
		}
		if (NULL == m_pHikCamera)
		{
			m_pHikCamera = new CMyCamera;
		}
		SetMCDeviveInfo(pTargetDeviceInfo);
		iRet = m_pHikCamera->Open(pTargetDeviceInfo);
		BREAK_IF(0 != iRet);

		iRet = m_pHikCamera->RegisterImageCallBack(ImageCallBack, this);
		BREAK_IF(0 != iRet);

		iRet = m_pHikCamera->RegisterExceptionCallBack(ExceptionCallBack, this);
		BREAK_IF(0 != iRet);

		iRet = m_pHikCamera->SetEnumValue("PixelFormat", PixelType_Gvsp_Mono8);
		BREAK_IF(0 != iRet);

		iRet = RotationAngle(iRotationAngle);
		BREAK_IF(0 != iRet);

		//设为出发模式
		iRet = m_pHikCamera->SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
		BREAK_IF(MV_OK != iRet);


		/*iRet = m_pHikCamera->SetEnumValue("DeviceLinkHeartbeatMode", 1);
		BREAK_IF(0 != iRet);*/

		//iRet = m_pHikCamera->SetIntValue("GevHeartbeatTimeout", 3000);
		//BREAK_IF(MV_OK != iRet);

		//iRet = m_pHikCamera->SetEnumValue("DeviceLinkHeartbeatMode", 0);
		//BREAK_IF(0 != iRet);
		iRet = m_pHikCamera->SetEnumValue("GainAuto", 0);   // 设置增益前先把自动增益关闭，失败无需返回
		BREAK_IF(MV_OK != iRet);

		///*调节这两个曝光模式，才能让曝光时间生效*/
		iRet = m_pHikCamera->SetEnumValue("ExposureMode", 0);
		BREAK_IF(MV_OK != iRet);


		/*iRet = m_pHikCamera->SetEnumValue("ExposureAuto", 0);
		BREAK_IF(MV_OK != iRet);*/

		//iRet = m_pHikCamera->SetFloatValue("ExposureTime", 60);
		//BREAK_IF(MV_OK != iRet);


		iRet = m_pHikCamera->StartGrabbing();
		BREAK_IF(MV_OK != iRet);
		unsigned int iWidth = 0;
		iRet = m_pHikCamera->GetIntValue("WidthMax", &iWidth);
		BREAK_IF(MV_OK != iRet);
		m_CameraInfo.width = iWidth;

		unsigned int iHeight = 0;
		iRet = m_pHikCamera->GetIntValue("HeightMax", &iHeight);
		BREAK_IF(MV_OK != iRet);
		m_CameraInfo.height = iHeight;
	} while (0);
	m_lock.Unlock();

	if (0 == iRet)
	{
		m_bOpen = true;
	}

	return 0 == iRet;
}

bool CHIKSourceCameraPlugin::Close()
{
	m_bIsRunning = false;
	m_pHikCamera->StopGrabbing();
	if (m_pHikCamera)
	{
		m_pHikCamera->Close();
	}
	return true;
}

bool CHIKSourceCameraPlugin::IsOpen()
{
	CISAPUGuard<JMutex> locker(m_lock);
	if (m_pHikCamera)
	{
		//return m_pHikCamera->IsOpen();
		return m_bOpen;
	}
	return false;
}

bool CHIKSourceCameraPlugin::GetImage(void *pR, void *pG, void *pB, const char *szJobName /*= 0*/, bool bSetParam/*=true*/, bool acq/* = true */)
{
	if (pR == NULL || (!IsOpen() && !Open()))
	{
		return false;
	}
	bool bRet = false;

	do
	{
		if (acq && !SingleAcq(szJobName))
		{
			bRet = false;
			break;
		}
		CISAPUGuard<JMutex> locker(m_lock);
		try
		{
			int iSize = m_CameraInfo.width * m_CameraInfo.height;
			CISAPUGuard<JMutex> imageLocker(m_imageLock);
			if (m_ImageArray.getVaildDataCount() <= 0)
			{
				return false;
			}
			if (NULL == pG || NULL == pB || !m_bIsColorFilter)
			{
				memcpy(pR, m_ImageArray.getBuffer(), iSize);
			}
			else
			{
			}
			bRet = true;
		}
		catch (...)
		{
			bRet = false;
		}
	} while (0);

	if (!m_ReadImageEvent.IsSiangaled())
	{
		m_ReadImageEvent.SetEvent();
	}
	return bRet;
}

bool CHIKSourceCameraPlugin::GetImageSize(int &w, int &h)
{
	if (!IsOpen() && !Open())
	{
		return false;
	}
	CISAPUGuard<JMutex> locker(m_lock);

	w = m_CameraInfo.width;
	h = m_CameraInfo.height;
	return true;
}

void CHIKSourceCameraPlugin::SetTriggerFunction(FnTriggerFunction fnTriggerFunction, void *pInParam)
{
	CISAPUGuard<JMutex> locker(m_lock);
	m_fnTriggerFunction = fnTriggerFunction;
	m_pInParam = pInParam;
}

void CHIKSourceCameraPlugin::Reset()
{
	CISAPUGuard<JMutex> locker(m_imageLock);
	ClearBuf();
}

void CHIKSourceCameraPlugin::AddJobNameToList(const char * szJobName)
{

}

bool CHIKSourceCameraPlugin::SingleAcq(const char *szJobName /*= 0*/)
{
	if (!Open())
	{
		return false;
	}
	bool bRet = false;

	CISAPUGuard<JMutex> locker(m_lock);
	try
	{
		if (mode_line != m_CameraInfo.TriggerMode)
		{
			//实时显示模式下来取图像时，清缓存取等待最新图像
			if (m_bIsContinuations)
			{
				SetGain(m_CameraInfo.gain);
				Reset();
			}
			else
			{
				int iRet = m_pHikCamera->CommandExecute("TriggerSoftware");
				if (0 != iRet)
				{
					return false;
				}
			}
		}

		int iTimeOut = m_CameraInfo.acqTimeOut;
		uint64 startTime = JTime::CurrentMillisecond();
		uint64 currentTime = JTime::CurrentMillisecond();
		do
		{
			m_imageLock.Lock();
			if (m_ImageArray.getVaildDataCount() > 0)
			{
				m_imageLock.Unlock();
				return true;
			}
			m_imageLock.Unlock();
			Sleep(10);
			currentTime = JTime::CurrentMillisecond();
		} while (iTimeOut >= currentTime - startTime);
		return false;

	}
	catch (...)
	{
		return false;
	}
	return true;
}


bool CHIKSourceCameraPlugin::IsContinuations()
{
	return m_bIsContinuations;
}

bool CHIKSourceCameraPlugin::Continuations(bool bOpen)
{
	bool bRet = false;
	do
	{
		if (!Open())
		{
			break;
		}
		CISAPUGuard<JMutex> locker(m_lock);
		if (bOpen)
		{
			//如果为外触发， 则等待外部触发后取像
			if (mode_line != m_CameraInfo.TriggerMode)
			{
				int iRst = m_pHikCamera->StopGrabbing();
				iRst = m_pHikCamera->SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
				RETURN_V_IF(0 != iRst, false);
				iRst = m_pHikCamera->StartGrabbing();
				RETURN_V_IF(0 != iRst, false);
				m_bIsContinuations = bOpen;
			}
		}
		else
		{
			if (m_bIsContinuations)
			{
				int iRst = m_pHikCamera->StopGrabbing();
				m_pHikCamera->SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
				iRst = m_pHikCamera->StartGrabbing();
				m_bIsContinuations = bOpen;
			}
		}
		bRet = true;
	} while (0);
	return bRet;
}

bool CHIKSourceCameraPlugin::SetCameraInParam(VS_CAMERA_INFO &param)
{
	bool bRet = false;
	do
	{
		if (IsOpen())
		{
			if (/*param.TriggerMode != m_CameraInfo.TriggerMode && */param.TriggerMode != mode_unknow)
			{
				BREAK_IF(!SetTriggerMode(param.TriggerMode));
				m_CameraInfo.TriggerMode = param.TriggerMode;
			}
			if (param.TriggerMode == mode_line/* && param.Activation != m_CameraInfo.Activation*/)
			{
				BREAK_IF(!SetTriggerPolarity(param.Activation));
				m_CameraInfo.Activation = param.Activation;
			}
			/*if (param.exposure != m_CameraInfo.exposure)*/
			{
				BREAK_IF(!SetExposureTime((double)(param.exposure)));
				m_CameraInfo.exposure = param.exposure;
			}
			/*if (param.gain != m_CameraInfo.gain)*/
			{
				BREAK_IF(!SetGain(param.gain));
				m_CameraInfo.gain = param.gain;
			}
		}
		m_CameraInfo = param;
		bRet = true;
	} while (0);
	return bRet;
}

int CHIKSourceCameraPlugin::GetLastError(char *errInfo /*= NULL*/, int *iSize /*= NULL*/)
{
	return m_errorCode;
}

bool CHIKSourceCameraPlugin::SetGain(double GainValue)
{
	RETURN_V_IF(NULL == m_pHikCamera, false);
	int nRet = m_pHikCamera->SetFloatValue("Gain", GainValue);
	RETURN_V_IF(MV_OK != nRet, false);

	return true;
}

bool CHIKSourceCameraPlugin::SetExposureTime(double ExpTimeValue)
{
	RETURN_V_IF(NULL == m_pHikCamera, false);

	int nRet = m_pHikCamera->SetFloatValue("ExposureTime", ExpTimeValue);
	RETURN_V_IF(MV_OK != nRet, false);

	return true;
}

bool CHIKSourceCameraPlugin::SetTriggerMode(CameraTriggerMode iMode)
{
	bool bRet = false;
	CISAPUGuard<JMutex> locker(m_lock);
	RETURN_V_IF(NULL == m_pHikCamera, false);
	int iErr = 0;
	if (mode_line == iMode)
	{
		iErr = m_pHikCamera->SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_LINE0);
	}
	else
	{
		iErr = m_pHikCamera->SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
	}

	return 0 == iErr;
}

bool CHIKSourceCameraPlugin::SetTriggerPolarity(CameraTriggerActivation iMode)
{
	if (!IsOpen() || mode_line != m_CameraInfo.TriggerMode)
	{
		return true;  //打开相机是会重新设置
	}
	CISAPUGuard<JMutex> locker(m_lock);
	RETURN_V_IF(NULL == m_pHikCamera, false);
	int iErr = m_pHikCamera->SetEnumValue("TriggerActivation", iMode);
	return 0 == iErr;
}

bool CHIKSourceCameraPlugin::PushBackImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo)
{
	if (NULL == pData || NULL == pFrameInfo)
	{
		return false;
	}
	CISAPUGuard<JMutex> imageLocker(m_imageLock);

	if (pFrameInfo)
	{
		if (MV_Image_Undefined == m_nSaveImageType || NULL == pData)
		{
			return false;
		}

		// 设置对应的相机参数

		// 仅在第一次保存图像时申请缓存，在 CloseDevice 时释放
		if (NULL == m_pBufForSaveImage)
		{
			// BMP图片大小：width * height * 3 + 2048(预留BMP头大小)
			m_nBufSizeForSaveImage = pFrameInfo->nWidth * pFrameInfo->nHeight;

			m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);
			if (NULL == m_pBufForSaveImage)
			{
				return false;
			}
		}
		memset(m_pBufForSaveImage, 0, m_nBufSizeForSaveImage);

		//MV_SAVE_IMAGE_PARAM_EX stParam = {0};
		//stParam.enImageType = m_nSaveImageType; // 需要保存的图像类型
		//stParam.enPixelType = pFrameInfo->enPixelType;  // 相机对应的像素格式
		//stParam.nBufferSize = m_nBufSizeForSaveImage;  // 存储节点的大小
		//stParam.nWidth      = pFrameInfo->nWidth;         // 相机对应的宽
		//stParam.nHeight     = pFrameInfo->nHeight;          // 相机对应的高
		//stParam.nDataLen    = pFrameInfo->nFrameLen;
		//stParam.pData       = pData;
		//stParam.pImageBuffer = m_pBufForSaveImage;
		//stParam.nJpgQuality = 80;
		//int nRet = m_pHikCamera->SaveImage(&stParam);

		MV_CC_PIXEL_CONVERT_PARAM stConvertParam = { 0 };
		// 从上到下依次是：图像宽，图像高，输入数据缓存，输入数据大小，源像素格式，
		// 目标像素格式，输出数据缓存，提供的输出缓冲区大小
		// Top to bottom are：image width, image height, input data buffer, input data size, source pixel format, 
		// destination pixel format, output data buffer, provided output buffer size
		stConvertParam.nWidth = pFrameInfo->nWidth;
		stConvertParam.nHeight = pFrameInfo->nHeight;
		stConvertParam.pSrcData = pData;
		stConvertParam.nSrcDataLen = pFrameInfo->nFrameLen;
		stConvertParam.enSrcPixelType = pFrameInfo->enPixelType;
		stConvertParam.enDstPixelType = PixelType_Gvsp_Mono8;
		stConvertParam.pDstBuffer = m_pBufForSaveImage;
		stConvertParam.nDstBufferSize = m_nBufSizeForSaveImage;
		int nRet = m_pHikCamera->ConvertPixelType(&stConvertParam);

		m_CameraInfo.width = pFrameInfo->nWidth;
		m_CameraInfo.height = pFrameInfo->nHeight;



		RETURN_V_IF(MV_OK != nRet, false);
		//限制图像对列大小
		m_ImageArray.addBuffer(m_pBufForSaveImage, stConvertParam.nDstLen);

		if (m_pImageInfo.pixelR == NULL)
		{
			m_pImageInfo.pixelR = new unsigned char[stConvertParam.nWidth*stConvertParam.nHeight];
		}
		m_pImageInfo.width = stConvertParam.nWidth;
		m_pImageInfo.height = stConvertParam.nHeight;

		memcpy(m_pImageInfo.pixelR, (unsigned char *)pData, stConvertParam.nWidth*stConvertParam.nHeight);

	}
	return true;
}

bool CHIKSourceCameraPlugin::IsGetImageFinished()
{
	if (m_ReadImageEvent.IsSiangaled())
	{
		return true;
	}
	return false;
}

bool CHIKSourceCameraPlugin::WaitGetImageFinished(int iTimeOut)
{
	long time = iTimeOut;
	if (iTimeOut < 0)
	{
		time = m_CameraInfo.acqTimeOut;
	}
	if (JEVENT_WAIT_OK == JEvent::WaitForSingleObject(&m_ReadImageEvent, time))
	{
		return true;
	}
	return false;
}

bool CHIKSourceCameraPlugin::ResetFinishState()
{
	CISAPUGuard<JMutex> locker(m_lock);
	m_ReadImageEvent.ResetEvent();
	return true;
}

void CHIKSourceCameraPlugin::ClearBuf()
{
	CISAPUGuard<JMutex> locker(m_imageLock);
	m_ImageArray.clear();
}

int CHIKSourceCameraPlugin::RotationAngle(long lAngle)
{
	RETURN_V_IF(NULL == m_pHikCamera, false);
	int nRet = 0;
	if (lAngle == 90)
	{
		nRet = m_pHikCamera->SetBoolValue("ReverseY", true);
	}
	else if (lAngle == 180)
	{
		nRet = m_pHikCamera->SetBoolValue("ReverseX", true);
	}
	else if (lAngle == 270)
	{
		nRet = m_pHikCamera->SetBoolValue("ReverseX", true);
		nRet = m_pHikCamera->SetBoolValue("ReverseY", true);
	}
	return nRet;
}

bool CHIKSourceCameraPlugin::SetMCDeviveInfo(MV_CC_DEVICE_INFO* pDeviceInfo)
{
	m_pDeviceInfo = pDeviceInfo;
	return true;
}



