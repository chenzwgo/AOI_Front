#include "dalsalinecamera.h"
#include "JTime.h"
#include "rtdefine.h"
#include "jdir.h"
#include "trace.h"
#include "device/dalsalinecameradevice.h"

#define FOR_TEST_DRAY_NUMB   0
#define FRAME_COUNT_LINE   512
#define REAL_TIME_BUFFER_LINES    3

CDalsaGigeLineCamera::CDalsaGigeLineCamera(const char *pszGUID)
    : m_pInParam(NULL)
    , m_bImageArrived(false)
    , m_bIsOpen(false)   
    , m_bIsOffLine(false)
    , m_bIsColorFilter(false)
    , m_Acq(NULL)
    , m_AcqDevice(NULL)
    , m_Buffer(NULL)
    , m_Xfer(NULL)
    , m_pImageBuf(NULL)
    , m_iCurrentIndex(0)
	, m_iRecvCount(0)
    , m_bIsGrab(false)
    , m_fnTriggerFunction(NULL)
    , m_ServerIndex(0)
    , m_ResourceIndex(0)
    , m_iCurrentWidth(0)
    , m_iCurrentHeight(0)
    , m_iCurrentCount(0)
    , m_errorCode(0)
    , m_CamStatus(STATUS_UNINIT)
    , m_camDevice(NULL)
	, m_latestTime(0)
//    , m_nPixelColorFilter(GX_COLOR_FILTER_NONE)
{
    m_camDevice = new dalsalinecameradevice;
	for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		m_pBufferChannel[i] = NULL;
    }
    //for (int i = 0; i < 512; i++)
    //{
    //    m_pImageRecvBufFrame[i] = NULL;
    //}
    m_pImageRecvBuf = NULL;
    m_CameraParam.strCameraID = pszGUID;
    m_lock.Init();
    m_imageLock.Init();
    m_camStatusLock.Init();
    m_AcqImageEvent.Init(false, false);
    m_GrabFinishedEvent.Init(false, false);
    m_ReadImageEvent.Init(false, true);  //默认可以取图像

    m_pThreadOpenCam = new JThreadTP<CDalsaGigeLineCamera,void*,void>;
}


CDalsaGigeLineCamera::~CDalsaGigeLineCamera(void)
{
    m_bIsRunning = false;
    int iTimeOut = 3000;  //3秒超时退出等待

    m_ReadImageEvent.SetEvent();
    while (m_pThreadOpenCam->IsRunning())
    {
        if (iTimeOut < 0)
        {
            break;
        }
        iTimeOut -= 10;
    }
    if (NULL != m_pImageRecvBuf)
    {
        delete []m_pImageRecvBuf;
        m_pImageRecvBuf = NULL;
    }
    if (NULL != m_pImageInfo.pixelR)
    {
        delete[]m_pImageInfo.pixelR;
        m_pImageInfo.pixelR = NULL;
    }
    //if (NULL != m_Acq)
    //{
    //    if (*m_Acq)
    //    {
    //        m_Acq->Destroy();
    //    }
    //    delete m_Acq;
    //    m_Acq = NULL;
    //}
    //SapAcqDevice camera(serverName);
    //BOOL status = camera.Create();
    //if (status)
    //    status = camera.GetFeatureCount(&featureCount);
    //if (status && featureCount > 0)
    //{
    //    // Get Serial Number Feature Value
    //    status = camera.GetFeatureValue("DeviceID", serialNumberName, sizeof(serialNumberName));
    //    printf("%d/ %s\n", cameraIndex+1, status ? serialNumberName : "N/A");
    //    cameraIndex++;
    //}
}

bool CDalsaGigeLineCamera::InitCam(unsigned int ServerIndex, unsigned int ResourceIndex, const char *szDeviceName, const LINE_CAM_PARAM *param)
{
    m_ServerIndex = ServerIndex;
    m_ResourceIndex = ResourceIndex;
	m_strDeviceName = szDeviceName;

    m_CameraParam = *param;
    m_bIsRunning = true;
    if (NULL != m_camDevice)
    {
        m_camDevice->init(szDeviceName);
        m_camDevice->setCamPluginId(m_CameraParam.strCamPluginID);
    }
    ThreadInitCam(this);//并行处理跟串行时间一样， 但复杂度更高
    //m_pThreadOpenCam->StartThread(this, &CDalsaLineCamera::ThreadInitCam, this);
    //Open();
    return true;
}
//void CDalsaLineCamera::SaveImage(int index)
//{
//    //for test save image  TBM
//    char szPath[256] = {0};
//    SapBuffer outBuf;
//    int  iLineNumbs = m_CameraParam.height/* * m_CameraParam.channel*/;
//    JTime curTime = JTime::CurrentLocalDateTime();
//    outBuf.SetParameters(1, m_CameraParam.width, iLineNumbs, m_Buffer->GetFormat(), m_Buffer->GetType());
//    if (!outBuf.Create())
//        return;
//    //fill
//    BYTE *dest = NULL;
//    outBuf.GetAddress(0, (void **)&dest);
//    //for (int j = 0; j < m_CameraParam.height; j++)
//    //{
//    //	memcpy(dest + j*m_CameraParam.width, m_pBufferChannel[ichannel]+j*m_CameraParam.width, sizeof(BYTE) * m_CameraParam.width);
//    //         }
//    for (int j = 0; j < iLineNumbs; j++)
//    {
//        memcpy(dest + j*m_CameraParam.width, m_pBufferChannel[index]+j*m_CameraParam.width, sizeof(BYTE) * m_CameraParam.width);
//    }
//    sprintf(szPath, "E:\\image\\%d-%d-%d", curTime.year(), curTime.month(), curTime.day());
//    JDir dir;
//    if (!dir.exists(szPath))
//    {
//        dir.mkdir(szPath);
//    }
//    memset(szPath, 0, 256);
//    sprintf(szPath, "E:\\image\\%d-%d-%d\\%d_%d-%d-%d-%d.bmp", curTime.year(), curTime.month(), curTime.day(), index, curTime.hour(), curTime.minute(), curTime.second(), curTime.msec());
//    outBuf.Save(szPath, "-format bmp", 0, iLineNumbs);
//}
const unsigned char *CDalsaGigeLineCamera::GetImage()
{
    return NULL;
    unsigned char *pData = NULL;

    if (!IsSnapFinished())
    {
        m_errorCode = ERR_GRAP_RUNNING;
        return NULL;
    }
    pData = m_pImageInfo.pixelR;
    m_errorCode = ERR_OK;
    return pData;
}
bool CDalsaGigeLineCamera::SingleAcq(bool bSoft)
{
    if ((!IsOpen() && !Open()))
    {
        return false;
    }

    bool bRet = false;

    CISAPUGuard<JMutex> locker(m_lock);
    try
    {
        //ClearRecvBuffer();
        //if (m_Xfer->IsGrabbing() || m_bIsAcq || m_bIsGrab)
        //{
        //    Continuations(false);
        //    m_Xfer->Abort();
        //    m_bIsAcq = false;
        //}

        if (NULL != m_Xfer)
        {
            //采集行数小于单位行数时， 修改单帧行数
            //int iLineCount = m_CameraParam.blockSize;
            //if (m_CameraParam.snapheight < m_CameraParam.blockSize)
            //{
            //    iLineCount = m_CameraParam.snapheight;
            //}
            if (m_Xfer->IsGrabbing())
            {
                m_Xfer->Abort();
            }

            int height = 0;
			m_AcqDevice->GetFeatureValue("Height", &height);

            if (m_CameraParam.snapheight != height)
            {
                m_Xfer->Destroy();
               // m_Acq->SetParameter(CORACQ_PRM_CROP_HEIGHT, m_CameraParam.height*m_CameraParam.channel); //
				m_AcqDevice->SetFeatureValue("Height", (UINT)m_CameraParam.snapheight); //
			
				setTriggerHeight(m_CameraParam.snapheight);
				
                m_Xfer->Create();
                CreateImageBuffer();
            }
            //m_Xfer->Abort();
            if (m_CameraParam.bExtTrigger && bSoft)
            {
				m_AcqDevice->SetParameter(CORACQ_PRM_EXT_TRIGGER_DETECTION, LINE_TG_EXTERNAL);
                //bRet = m_AcqDevice->SoftwareTrigger(SapAcquisition::SoftwareTriggerExt);
            }
            else
            {
                if (!m_Xfer->IsGrabbing())
                {
                    bRet = m_Xfer->Grab();
                }
                //bRet = m_Xfer->Snap(1);
                //bRet = m_Xfer->Snap((m_CameraParam.channel));
                //连续触发(m_CameraParam.height*m_CameraParam.channel)/iLineCount 次
                //bRet = m_Xfer->Snap(m_iCurrentHeight*m_iCurrentCount) / iLineCount);
            }
        }

        bRet = true;
    }
    catch (...)
    {
        LOG_WARN("===============[cam]acq failed.===============");
        m_errorCode = ERR_ACQ_FAILED;
        bRet = false; 
        setCamStatus(STATUS_GRAB_FAILED);
    }
    if (bRet)
    {
        m_errorCode = ERR_OK;
        setCamStatus(STATUS_CAM_READING);
    }
    return bRet;
}

bool CDalsaGigeLineCamera::StartAcq()
{
    if ((!IsOpen() && !Open()))
    {
        return false;
    }

    bool bRet = false;

    CISAPUGuard<JMutex> locker(m_lock);
    try
    {
        if (NULL != m_Xfer)
        {
            unsigned int height = 0;
			m_AcqDevice->GetFeatureValue("Height", &height);
            m_CameraParam.height = m_CameraParam.blockSize;
            if (m_CameraParam.blockSize != height)
            {
                if (m_Xfer->IsGrabbing())
                {
                    m_Xfer->Abort();
                }
                m_Xfer->Destroy();
				//m_AcqDevice->SetFeatureValue(CORACQ_PRM_CROP_HEIGHT, m_CameraParam.blockSize); //
				setTriggerHeight(m_CameraParam.blockSize);
				m_Xfer->Create();

            }
            if (!m_Xfer->IsGrabbing())
            {
                bRet = m_Xfer->Grab();
                LOG_INFO("===============[cam]start grab(%s).===============", m_CameraParam.strCameraName.c_str());
            }
        }
        bRet = true;
    }
    catch (...)
    {
        LOG_WARN("===============[cam]acq failed.===============");
        m_errorCode = ERR_ACQ_FAILED;
        bRet = false;
        setCamStatus(STATUS_GRAB_FAILED);
    }
    if (bRet)
    {
        m_errorCode = ERR_OK;
        setCamStatus(STATUS_CAM_READING);
    }
	m_iCurrentIndex = 0;
	m_iRecvCount = 0;
    return bRet;
}

bool CDalsaGigeLineCamera::GetImageSize(int &w, int &h)
{
    if (!IsOpen() && !Open())
    {
        return false;
    }

    try
    {
        CISAPUGuard<JMutex> locker(m_lock);
        unsigned long width = 0;
        if (FALSE == m_Acq->GetParameter(CORACQ_PRM_CROP_WIDTH, &width)) //
        {
            LOG_WARN("===============[cam]get param(CORACQ_PRM_CROP_WIDTH) failed.===============");
            m_errorCode = ERR_SET_PARAM_FAILED;
            return false;
        }
        if (width != m_CameraParam.width)
        {
            m_CameraParam.width = width;
            CreateImageBuffer();
        }
		w = m_CameraParam.width;
		h = m_CameraParam.height;
    }
    catch (...)
    {
        LOG_WARN("===============[cam]get image size failed.===============");
        m_errorCode = ERR_GET_SIZE_FAILED;
        return false;
    }
    m_errorCode = ERR_OK;
    return true;
}

bool CDalsaGigeLineCamera::AbortAcq()
{
    //m_iCurrentIndex = m_CameraParam.height * m_CameraParam.channel;
    //m_iRecvCount = m_iCurrentIndex;
    if (NULL == m_Xfer || !m_Xfer->Abort())
    {
        LOG_WARN("===============[cam]abort acq failed.===============");
        m_errorCode = ERR_ABORT_FAILED;
        return false;
    }
    m_errorCode = ERR_OK;
    setCamStatus(STATUS_CAM_READ_ABORT);
    return true;
}

bool CDalsaGigeLineCamera::Open()
{
    bool bRet = false;
    try
    {
        do 
        {
            if (m_ServerIndex < 0/* || m_strDeviceName.empty()*/)
            {
                break;
            }
            CISAPUGuard<JMutex> locker(m_lock);
            //m_CameraParam.strCameraID = strCameraID;
            //if (NULL != m_camDevice && !m_camDevice->openDevice())
            //{
            //    break;
            //}
            SapLocation loc(m_ServerIndex, m_ResourceIndex);
            m_Acq           = new SapAcquisition(loc, m_CameraParam.strCamConfigPath.c_str());
            m_AcqDevice     = new SapAcqDevice(loc, m_CameraParam.strCamConfigPath.c_str());
            m_Buffer        = new SapBufferWithTrash(2, m_AcqDevice);
            m_Xfer          = new SapAcqDeviceToBuf(m_AcqDevice, m_Buffer, CDalsaGigeLineCamera::BufferCallback, this); //TBM
            //m_Xfer          = new SapAcqDeviceToBuf(m_Acq, m_Buffer, CDalsaCamera::BufferCallback, this); //TBM
            // Create acquisition object NULL == *m_Acq is not create
            if (NULL == m_AcqDevice || !m_AcqDevice->Create())
                break;
            //m_Acq->SetParameter(CORACQ_PRM_CROP_HEIGHT, m_CameraParam.height); //
			//m_Acq->SetParameter(CORACQ_PRM_CROP_HEIGHT, m_CameraParam.blockSize); //for fix      
           // m_Acq->SetParameter(CORACQ_PRM_CROP_HEIGHT, m_CameraParam.height*m_CameraParam.channel); //

            //if (!m_Acq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_ENABLE, true))
			setTriggerHeight(m_CameraParam.blockSize);

            // Create buffer object
            if (NULL == m_Buffer || !m_Buffer->Create())
                break;
				
			m_CameraParam.height = m_CameraParam.blockSize;
            // Create transfer object
            if (NULL == m_Xfer || !m_Xfer->Create()) 
                break;

            //获取图像大小,分配空间;
            CreateImageBuffer();
            //if (m_CameraParam.bExtTrigger)
            //{
            //    m_Acq->SetParameter(CORACQ_PRM_EXT_TRIGGER_ENABLE, 1);//TBM
            //    m_Acq->SetParameter(CORACQ_PRM_EXT_TRIGGER_FRAME_COUNT, m_CameraParam.fullHiget*m_CameraParam.channel);
            //    m_Acq->SetParameter(CORACQ_PRM_EXT_TRIGGER_DETECTION, m_CameraParam.tgDetection);
            //}
            //else
            //{
            //    m_Acq->SetParameter(CORACQ_PRM_EXT_TRIGGER_ENABLE, 0);//TBM
            //}
            if (NULL != m_camDevice)
            {
				m_camDevice->openDevice(m_AcqDevice);
               // m_camDevice->setTDIFeature(true);
            }
			if (m_Buffer != NULL)
			{
				//m_Buffer->SetHeight(m_CameraParam.height);
				UINT32 itmp = m_Buffer->GetHeight();
				UINT32 itmp2 = itmp;
			}
			//if (NULL != m_camDevice && !))
			//{
			//    break;
			//}
            //int value = 0;
            //m_AcqDevice->GetFeatureValue("sensorTDIStagesSelection", &value);
            //if (value != 1)
            //{
            //    m_AcqDevice->SetFeatureValue("sensorTDIStagesSelection", 1);  //TDI设置为1,防止干扰到多通道采图
            //}
			SetTriggerMode(m_CameraParam.bExtTrigger);
            SetExposureTime(m_CameraParam.exposure);
            SetGain(m_CameraParam.gain);
            SetCameraFrequency(m_CameraParam.grabFreq);
            bRet = true;
        } while (0);
    }
    catch (...)
    {
        //handleException(ex);
        bRet = false;
    }
    if (!bRet)
    {
        LOG_WARN("===============[cam]open cam failed%s.===============", m_CameraParam.strCameraID.c_str());
        m_errorCode = ERR_OPEN_FAILED;
        setCamStatus(STATUS_CAM_OPEN_FAILED);
    }
    else
    {
        setCamStatus(STATUS_CAM_IDLE);
        m_errorCode = ERR_OK;
    }
    return bRet;
}

bool CDalsaGigeLineCamera::Close()
{
    CISAPUGuard<JMutex> locker(m_lock);
	bool bRet = true;
	if (NULL != m_Xfer)
    {
        if (m_Xfer->IsGrabbing())
        {
            m_Xfer->Abort();
        }
		if (*m_Xfer)
		{
			m_Xfer->Destroy();
		}
		delete m_Xfer;
		m_Xfer = NULL;
	}
	if (NULL != m_Buffer)
	{
		if (*m_Buffer)
		{
			//m_Buffer->Destroy();
		}
		delete m_Buffer;
		m_Buffer = NULL;
    }
    if (NULL != m_camDevice)
    {
        m_camDevice->closeDevice();
    }
	//if (NULL != m_AcqDevice && *m_AcqDevice)
	//{
	//	if (*m_AcqDevice)
	//	{
	//		m_AcqDevice->Destroy();
	//	}
	//	delete m_AcqDevice;
	//	m_AcqDevice = NULL;
	//}
  //  if (NULL != m_AcqDevice && *m_AcqDevice)
  //  {
  //      if (*m_AcqDevice)
  //      {
		//	m_AcqDevice->Destroy();
  //      }
  //      //delete m_AcqDevice;
		//m_AcqDevice = NULL;
  //  }
    m_iCurrentIndex = 0;
	m_iRecvCount = 0;
    setCamStatus(STATUS_CAM_OFF_LINE);
    return bRet;
}

bool CDalsaGigeLineCamera::IsOpen()
{
    CISAPUGuard<JMutex> locker(m_lock);
    if (NULL == m_AcqDevice || NULL == *m_AcqDevice)
    {
        return false;
    }
    if (NULL == m_Buffer || NULL == *m_Buffer)
    {
        return false;
    }
    if (NULL == m_Xfer || NULL == *m_Xfer)
    {
        return false;
    }
    //if (m_hDevice != NULL && m_bIsOpen && !m_bIsOffLine)
    //{
    //    return true;
    //}
    //if (m_bIsOffLine)
    //{
    //    ProcessOffline();
    //}
    return true;
}

void CDalsaGigeLineCamera::Reset()
{
    CISAPUGuard<JMutex> locker(m_imageLock);
    m_AcqImageEvent.ResetEvent();
    m_ReadImageEvent.ResetEvent();
    m_GrabFinishedEvent.ResetEvent();

    m_iCurrentIndex = 0;
	m_iRecvCount = 0;
}

bool CDalsaGigeLineCamera::SetGain( double GainValue )
{
    if (GainValue < 1.0)
    {
        return false;
    }
	if (m_camDevice == NULL)
	{
		return false;
	}
    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;

    try
    {
        if (NULL != m_camDevice && m_camDevice->SetGain(GainValue))
        {
            m_CameraParam.gain = GainValue;
            bRet = true;
        }
    }
    catch (...)
    {
        LOG_WARN("===============[cam]set gain param failed.===============");
        bRet = false;
    }
    return bRet;
}

bool CDalsaGigeLineCamera::SetExposureTime( double ExpTimeValue )
{
    bool bRet = false;
    if (ExpTimeValue < 1.0)
    {
        return false;
    }
	if (m_camDevice == NULL)
	{
		return false;
	}
    CISAPUGuard<JMutex> locker(m_lock);
    try
    {
        if (NULL != m_camDevice && m_camDevice->SetExposureTime(ExpTimeValue))
        {
            m_CameraParam.exposure = ExpTimeValue;
            bRet = true;
        }
    }
    catch (...)
    {
        //try
        //{
        //    if (NULL != m_pFeature && m_AcqDevice->GetFeatureInfo("ExposureTime", m_pFeature))
        //    {
        //        double curValue;
        //        if(m_AcqDevice->GetFeatureValue("ExposureTime", &curValue))
        //        {
        //            m_CameraParam.exposure = ExpTimeValue;
        //        }
        //    }
        //}
        //catch (...)
        //{
        //}
        //LOG_WARN("===============[cam]set ExposureTime param failed.===============");
        bRet = false;
    }
    return bRet;
}

bool CDalsaGigeLineCamera::SetCameraFrequency(unsigned int Freq)
{
    bool bRet = false;
    if (Freq < 1)
    {
        return false;
    }
	if (m_camDevice == NULL)
	{
		return false;
	}
    CISAPUGuard<JMutex> locker(m_lock);
    try
    {
		RETURN_V_IF(m_CameraParam.bExtTrigger, true);
        if (NULL != m_camDevice && m_camDevice->SetCameraFrequency(Freq))
        {
            m_CameraParam.grabFreq = Freq;
            bRet = true;
        }
    }
    catch (...)
    {
        LOG_WARN("===============[cam]set AcquisitionLineRate param failed.===============");
        bRet = false;
    }
    return bRet;
}

bool CDalsaGigeLineCamera::CreateImageBuffer()
{
    unsigned int width = 0;
    unsigned int height = 0;
	//m_AcqDevice->GetFeatureValue("Width", &width); 
	//m_AcqDevice->GetFeatureValue("Height", &height);
	if (m_Buffer != NULL)
	{
		height = m_Buffer->GetHeight();
		width = m_Buffer->GetWidth();
	}

    if (m_pImageInfo.width != width || m_pImageInfo.height != height)
    {
        if (NULL != m_pImageInfo.pixelR)
        {
            delete[]m_pImageInfo.pixelR;
            m_pImageInfo.pixelR = NULL;
        }
    }
    if (NULL == m_pImageInfo.pixelR)
    {
        m_pImageInfo.width = width;
        m_pImageInfo.height = height;
        int iCount = width * height * sizeof(BYTE);
        m_pImageInfo.pixelR = new BYTE[iCount];
    }
    return true;
}

bool CDalsaGigeLineCamera::InsertBuffer( void *pData, int width, int height/* = 1*/)
{
	CDalsaGigeLineCamera* test = this;
    unsigned char *image = (unsigned char *)pData;

    if (width == m_pImageInfo.width && height == m_pImageInfo.height)
    {
        int iCount = width * height * sizeof(BYTE);
        memcpy(m_pImageInfo.pixelR, image, iCount);
    }
	m_iCurrentIndex++;
	m_iRecvCount += height;
	LOG_INFO_F_SIMP("Trace_DalsaGigeLineCamera.log","-------> InsertBuffer(%s-total:%d)---%d--%d)", m_CameraParam.strCameraID.c_str(), m_iRecvCount, m_iCurrentIndex, height);
	
    if (m_fnTriggerFunction != NULL)
    {
        m_fnTriggerFunction(m_pInParam, image, width, height);
    }
    else
    {
        LOG_INFO("===============Callback is NULL.===============");
    }
    //if (m_iCurrentIndex == 0)
    //{
    //    LOG_INFO("===============Grab Image begin.===============");
    //}

    //防呆
    //if (m_CameraParam.channel < 1)
    //{
    //    m_CameraParam.channel = 1;
    //}
    //for (int i = 0; i < icount; i++)
    //{
    //    if (m_iCurrentIndex < m_CameraParam.height * m_CameraParam.channel) //如果为有效数据则放到内存
    //    {
    //        iCurChannel = m_iCurrentIndex%m_CameraParam.channel;
    //        iCurIndex   = m_iCurrentIndex/m_CameraParam.channel;
    //        unsigned char *pBufferTmp = m_pBufferChannel[iCurChannel] + iCurIndex * lineWidth;
    //        unsigned char *pBufferSrc = image + i*lineWidth;
    //        if (m_CameraParam.bIsInvert)
    //        {
    //            for (int i = 0; i < lineWidth; i++)
    //            {
    //                pBufferTmp[i] = pBufferSrc[lineWidth-i];
    //            }
    //        }
    //        else
    //        {
    //            memcpy(pBufferTmp, pBufferSrc, lineWidth);
    //        }
    //        m_iCurrentIndex++;
    //    }//无效图片抛弃
    //}

    //if (IsSnapFinished())
    //{
    //    //LOG_INFO("===============Grab Image Finished.===============");
    //    if (m_bIsGrab)
    //    {
    //        if (m_fnTriggerFunction != NULL)
    //        {
    //            m_fnTriggerFunction(m_pInParam, m_CameraParam.channel, m_pBufferChannel, m_CameraParam.width, m_CameraParam.height, m_CameraParam.xoffset);
    //        }
    //        m_iCurrentIndex = 0;
    //        m_iRecvCount = 0;
    //    }
    //    m_bIsAcq = false;
    //    setCamStatus(STATUS_CAM_READ_END);
    //}
    return true;
}

void CDalsaGigeLineCamera::ThreadInitCam(void* param)
{
    if (!IsOpen())
    {
        Open();
    }
}

void CDalsaGigeLineCamera::setTriggerHeight(int iHeight)
{
	//m_AcqDevice->GetParameter(CORACQ_PRM_CROP_WIDTH, &width); 
	//m_AcqDevice->GetParameter(CORACQ_PRM_CROP_HEIGHT, &height);
	if (iHeight < m_CameraParam.blockSize)
	{
		return;
	}

	if (m_AcqDevice != NULL)
	{
		//m_AcqDevice->SetParameter(CORACQ_PRM_CROP_HEIGHT, iHeight);
		m_AcqDevice->SetFeatureValue("Height", iHeight);
		UINT32 itmp = 0;
		m_AcqDevice->GetFeatureValue("Height", &itmp);
		UINT32 itmp2 = itmp;
	}
	if (m_Buffer != NULL )
	{
		m_Buffer->SetHeight(iHeight);
		UINT32 itmp = m_Buffer->GetHeight();
		UINT32 itmp2 = itmp;
	}
}

bool CDalsaGigeLineCamera::init()
{
    bool bRet = false;
    return bRet;
}


/*********************************************************************
/**相机掉线处理
*********************************************************************/
void CDalsaGigeLineCamera::ProcessOffline()
{
}

void __stdcall CDalsaGigeLineCamera::OnDeviceOfflineCallbackFun( void* pUserParam )
{
    CDalsaGigeLineCamera *pCtrDevice = (CDalsaGigeLineCamera *)pUserParam;
    if (NULL != pCtrDevice)
    {
        pCtrDevice->m_bIsOffLine = true;
    }
}

bool CDalsaGigeLineCamera::IsImplemented()
{
    m_bIsColorFilter = false;
    //if (NULL != m_Buffers)
    {
        //SapFormat format = m_Buffers->GetFormat();
        //if (!CORDATA_FORMAT_IS_MONO(format))
        {
            //m_bIsColorFilter = true;
        }
    }
    return m_bIsColorFilter;
}

void CDalsaGigeLineCamera::TransformToRGB( void *pBuffer, void *pR, void *pG, void *pB )
{
}

bool CDalsaGigeLineCamera::SetCameraInParam( LINE_CAM_PARAM &param )
{
    bool bRet = false;
    do 
    {
        bool bIsNeedClose = false;
        m_CameraParam.bIsInvert = param.bIsInvert;
        //step 1. 设置必须关闭相机的参数
        //step 2. 需要重启的配置

        if (NULL != m_AcqDevice)
        {
            int blockSize = 0;
			m_AcqDevice->GetFeatureValue("Height", &blockSize);
            if (blockSize != param.blockSize && NULL != m_Xfer && m_Xfer->IsGrabbing())
            {
				//m_AcqDevice->SetParameter(CORACQ_PRM_CROP_HEIGHT, param.blockSize);
				
				setTriggerHeight(param.blockSize);
				

            }
        }
        if (param.blockSize != m_CameraParam.blockSize)
        {
            m_CameraParam.blockSize = param.blockSize;
        }
        //if (param.channel != m_CameraParam.channel)
        //{
        //    m_CameraParam.channel = param.channel;
        //    bIsNeedClose = true;
        //}
        if (m_CameraParam.strCamConfigPath != param.strCamConfigPath)
        {
            m_CameraParam.strCamConfigPath = param.strCamConfigPath;
            bIsNeedClose = true;//如果配置文件路径有改变， 则重新打开相机
        }
        if (m_CameraParam.height != param.height && NULL != m_AcqDevice && NULL != m_Xfer)
        {
            if (*m_Xfer)
            {
                m_Xfer->Destroy();
            }
            //m_Acq->SetParameter(CORACQ_PRM_CROP_HEIGHT, m_CameraParam.height*m_CameraParam.channel); //
            m_Xfer->Create();
            CreateImageBuffer();
        }
        if (bIsNeedClose && IsOpen())
        {
            Close();
        }

        if (!IsOpen())
        {
            m_CameraParam = param;
            m_CameraParam.height = m_CameraParam.snapheight;
        }
        //step 3. 必须在打开的情况下才能设置的参数
        if (NULL != m_camDevice)
        {
            m_camDevice->setCamPluginId(m_CameraParam.strCamPluginID);
        }
        if (IsOpen() || Open())
        {
            //缓冲参数
			m_CameraParam.snapheight = param.snapheight;
			//m_CameraParam.realheight = param.realheight;
            if (param.width != m_CameraParam.width || param.height != m_CameraParam.height)
			{
				m_CameraParam.width = param.width;
				m_CameraParam.height = param.height;
                CreateImageBuffer();
            }
            //需要下发的参数
			bRet = SetTriggerMode(param.bExtTrigger);
            if (param.bExtTrigger && m_CameraParam.tgDetection != param.tgDetection)
            {
				m_AcqDevice->SetParameter(CORACQ_PRM_EXT_TRIGGER_DETECTION, param.tgDetection);
                m_CameraParam.tgDetection = param.tgDetection;
            }
            if (m_CameraParam.exposure != param.exposure)
            {
                SetExposureTime(param.exposure);
            }
            if (m_CameraParam.gain != param.gain)
            {
                SetGain(param.gain);
            }
            if (m_CameraParam.grabFreq != param.grabFreq)
            {
                SetCameraFrequency(param.grabFreq);
            }
        }
        if (param != m_CameraParam)
        {
            bRet = true;
            Reset();
        }
        bRet = true;
    } while (0);
    if (!bRet)
    {
        LOG_WARN("===============[cam]set param failed.===============");
        m_errorCode = ERR_SET_PARAM_FAILED;
    }
    else
    {
        m_errorCode = ERR_OK;
    }
    return bRet;
}

void CDalsaGigeLineCamera::ClearBuf()
{
    m_iCurrentIndex = 0;
	m_iRecvCount = 0;
}

bool CDalsaGigeLineCamera::Continuations(bool bOpen, int iSize/* = 1*/)
{
    bool bRet = true;
    if ((!IsOpen()))
    {
		if (!bOpen)
		{
			return true;
		}
		if (!Open())
		{
			return false;
		}
    }
    if (NULL != m_Xfer)
    {
        CISAPUGuard<JMutex> locker(m_lock);
        if (bOpen)
        {
            ClearRecvBuffer();
            unsigned int height = 0;
			m_AcqDevice->GetFeatureValue("Height", &height);
            if (height != iSize)
            {
                //m_CameraParam.height = m_CameraParam.realheight;

                if (*m_Xfer)
                {
                    m_Xfer->Abort();
                }
                m_Xfer->Destroy();
							
				setTriggerHeight(iSize);	
                m_Xfer->Create();


				// for test begin
				//if (NULL != m_Xfer)
				//{
				//	if (m_Xfer->IsGrabbing())
				//	{
				//		m_Xfer->Abort();
				//	}
				//	if (*m_Xfer)
				//	{
				//		m_Xfer->Destroy();
				//	}
				//	delete m_Xfer;
				//	m_Xfer = NULL;
				//}
				//if (NULL != m_Buffer)
				//{
				//	if (*m_Buffer)
				//	{
				//		m_Buffer->Destroy();
				//	}
				//	delete m_Buffer;
				//	m_Buffer = NULL;
				//}

				//m_Buffer = new SapBufferWithTrash(2, m_AcqDevice);
				//m_Xfer = new SapAcqDeviceToBuf(m_AcqDevice, m_Buffer, CDalsaGigeLineCamera::BufferCallback, this); //TBM				
				//if (NULL == m_Buffer || !m_Buffer->Create())
				//if (NULL == m_Xfer || !m_Xfer->Create())
				//setTriggerHeight(iSize);
				// for test end

                CreateImageBuffer();
            }
            m_bIsGrab = true;
            bRet = m_Xfer->Grab();
        }
        else
        {
            m_bIsGrab = false;
            bRet = m_Xfer->Freeze();
            m_Xfer->Abort();
        }
    }
    if (bRet)
    {
        m_errorCode = ERR_OK;
        if (bOpen)
        {
            setCamStatus(STATUS_CAM_READING);
        }
        else
        {
            setCamStatus(STATUS_CAM_READ_END);
        }
        
    }
    return bRet;
}

bool CDalsaGigeLineCamera::IsContinuations()
{
    bool bRet = true;
    if ((!IsOpen() && !Open()))
    {
        return false;
    }
    if (NULL != m_Xfer)
    {
        bRet = m_Xfer->IsGrabbing();
    }
    return bRet;
}

void CDalsaGigeLineCamera::BufferCallback( SapXferCallbackInfo * pInfo)
{
    CDalsaGigeLineCamera *pInstance = (CDalsaGigeLineCamera *)pInfo->GetContext();
    SapBuffer *pXer = pInstance->m_Buffer;
    int iWidth = pXer->GetWidth();
	int iHight = pXer->GetHeight();
    unsigned long height = 0;
   // pInstance->m_Acq->GetParameter(CORACQ_PRM_CROP_HEIGHT, &height);
    void *pData;
    pXer->GetAddress(&pData); //获取相机缓存Buffer指针
    //将整块内存拷贝到内存中
    BYTE *bData = (BYTE *)pData;

    pInstance->InsertBuffer(bData, iWidth, iHight);
	uint64 curtime = JTime::CurrentMillisecond();
	if (curtime - pInstance->m_latestTime > 10000)
	{
		LOG_INFO("===============[cam recv]recv.(%s)===============", pInstance->m_CameraParam.strCameraName.c_str());
	}
	pInstance->m_latestTime = curtime;
}
int CDalsaGigeLineCamera::GetCamStatus() 
{
    CISAPUGuard<JMutex> locker(m_camStatusLock); 
    return m_CamStatus;
};
void CDalsaGigeLineCamera::setCamStatus(unsigned long status)
{
    CISAPUGuard<JMutex> locker(m_camStatusLock);
    m_CamStatus = /*m_CamStatus | */status;
}

//void CDalsaLineCamera::CleanCamStatus(unsigned long status)
//{
//    unsigned int sttmp = 0xFFFF ^ status;
//    m_CamStatus = m_CamStatus & sttmp;
//}

int CDalsaGigeLineCamera::GetLastError(char *errInfo/* = NULL*/, int *iSize/* = NULL*/)
{
    //int iLength = 0;
    if (NULL != iSize)
    {
        //*iSize = 0;
        //if (NULL != errInfo)
        //{
        //    memset(errInfo, 0, *iSize);
        //}
        //size_t tSize = 0;
        //GXGetLastError(&m_errorCode, errInfo, &tSize);
        //*iSize = tSize;
    }
    return m_errorCode;
}

void CDalsaGigeLineCamera::SetTriggerFunction( FnLineTriggerFunction fnTriggerFunction, void *pInParam )
{
    CISAPUGuard<JMutex> locker(m_lock);
    m_fnTriggerFunction = fnTriggerFunction;
    m_pInParam = pInParam;
}

bool CDalsaGigeLineCamera::IsInitFinished()
{
    return !m_pThreadOpenCam->IsRunning();
}

bool CDalsaGigeLineCamera::IsSnapFinished()
{
    bool bRet = true;
    if ((!IsOpen() && !Open()))
    {
        return false;
    }
    if (NULL != m_Xfer)
    {
        bRet = !m_Xfer->IsGrabbing();// (/*!m_Xfer->IsGrabbing() && */m_iRecvCount >= m_CameraParam.height * m_CameraParam.channel); //TBM
    }
    return bRet;
}

bool CDalsaGigeLineCamera::WaitGetImageFinished(int iTimeOut)
{
    long time = iTimeOut; 
    //if (iTimeOut < 0)
    //{
    //    time = m_CameraInfo.acqTimeOut;
    //}
    if (JEVENT_WAIT_OK == JEvent::WaitForSingleObject(&m_ReadImageEvent, time))
    {
        return true;
    }
    return false;
}

bool CDalsaGigeLineCamera::ResetFinishState()
{
    CISAPUGuard<JMutex> locker(m_lock);
    m_ReadImageEvent.ResetEvent();
    m_GrabFinishedEvent.ResetEvent();
    return true;
}
bool CDalsaGigeLineCamera::SetTriggerMode( bool triger )
{
	bool bRet = false;
	if (m_camDevice == NULL)
	{
		return false;
	}
	do 
    {
        if (NULL != m_camDevice && !m_camDevice->SetTriggerMode(triger))
        {
            break;
        }
		m_CameraParam.bExtTrigger = triger;
		BOOL bIsTriger;
        int emTrigger;
/*        BREAK_IF(!m_AcqDevice->GetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, &bIsTriger));
		if (bIsTriger != triger)
		{
			if (triger)
			{
                BREAK_IF(!m_AcqDevice->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, 1));
			}
			else
			{
                BREAK_IF(!m_AcqDevice->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, 0));
			}
			m_CameraParam.bExtTrigger = triger;
		}*/
		bRet = true;
	} while (0);
	return bRet;
}

void CDalsaGigeLineCamera::ClearRecvBuffer()
{
}

