#include "ikaplinecamera.h"
#include "JTime.h"
#include "rtdefine.h"
#include "jdir.h"
#include "trace.h"


#define FOR_TEST_DRAY_NUMB   0
#define FRAME_COUNT_LINE   512
#define REAL_TIME_BUFFER_LINES

/* This callback function will be called on grab start. */
void CALLBACK OnGrabStart(void* pContext);

/* This callback function will be called at the frame ready. */
void CALLBACK OnFrameReady(void* pContext);

/* This callback function will be called on grabbing timeout. */
void CALLBACK OnTimeout(void* pContext);

/* This callback function will be called on frame lost. */
void CALLBACK OnFrameLost(void* pContext);

/* This callback function will be called on grab stop. */
void CALLBACK OnGrabStop(void* pContext);


CIKapLineCamera::CIKapLineCamera(const char *pszGUID)
    : m_pInParam(NULL)
    , m_bImageArrived(false)
    , m_bIsOpen(false)   
    , m_bIsOffLine(false)
    , m_bIsColorFilter(false)
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
	, m_latestTime(0)
    , m_pCSerialPortControl(NULL)
{

	for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		m_pBufferChannel[i] = NULL;
    }

    m_pImageRecvBuf = NULL;
    m_CameraParam.strCameraID = pszGUID;
    m_lock.Init();
    m_imageLock.Init();
    m_camStatusLock.Init();
    m_AcqImageEvent.Init(false, false);
    m_GrabFinishedEvent.Init(false, false);
    m_ReadImageEvent.Init(false, true);  //默认可以取图像

    m_hBoard = INVALID_HANDLE_VALUE;
    m_nCurFrameIndex = 0;
    m_board_frmae_count = 5;
    m_pCSerialPortControl = new CSerialPortControl();
    m_pThreadOpenCam = new JThreadTP<CIKapLineCamera,void*,void>;
	m_bstartGrap = false;
	m_Acqlock.Init();
}


CIKapLineCamera::~CIKapLineCamera(void)
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

}

bool CIKapLineCamera::InitCam(unsigned int ServerIndex, unsigned int ResourceIndex, const char *szDeviceName, const LINE_CAM_PARAM *param)
{
    m_ServerIndex = ServerIndex;
    m_ResourceIndex = ResourceIndex;
	m_strDeviceName = szDeviceName;

    m_CameraParam = *param;
    m_bIsRunning = true;

    ThreadInitCam(this);//并行处理跟串行时间一样， 但复杂度更高
    return true;
}

const unsigned char *CIKapLineCamera::GetImage()
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
bool CIKapLineCamera::SingleAcq(bool bSoft)
{
    if ((!IsOpen() && !Open()))
    {
        return false;
    }

    bool bRet = false;

    CISAPUGuard<JMutex> locker(m_lock);
    try
    {

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

bool CIKapLineCamera::StartAcq()
{
    if ((!IsOpen() && !Open()))
    {
        return false;
    }

    bool bRet = false;

    CISAPUGuard<JMutex> locker(m_lock);
    try
    {
        if (INVALID_HANDLE_VALUE != m_hBoard)
        {
			CISAPUGuard<JMutex> autoLock(m_Acqlock);
			if (m_bstartGrap)
			{
				return true;
			}

			int nHeight = 0;
			/* Get image height.*/
			int ret = IKapGetInfo(m_hBoard, IKP_IMAGE_HEIGHT, &nHeight);

			if (m_CameraParam.blockSize != nHeight)
			{
				ret = IKapSetInfo(m_hBoard, IKP_IMAGE_HEIGHT, m_CameraParam.blockSize);	
				CreateImageBuffer();
			}
			/* Configure image buffers. */

			if (m_CameraParam.snapheight / m_CameraParam.blockSize > 0)
			{
				m_board_frmae_count = m_CameraParam.snapheight / m_CameraParam.blockSize;
				ret = IKapSetInfo(m_hBoard, IKP_FRAME_COUNT, IKP_IMAGE_BUFF_COUT);
				if (ret != IK_RTN_OK)
				{
					;
				}
				int frmae_count;
				ret = IKapGetInfo(m_hBoard, IKP_FRAME_COUNT, &frmae_count);
				m_board_frmae_count = frmae_count;
				if (ret != IK_RTN_OK)
				{
					;
				}
			}


            /*  Start capturing image. */
			m_nCurFrameIndex = 0;
			m_bstartGrap = true;
            ret = IKapStartGrab(m_hBoard, 0);
            if (ret != 1)
            {
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

bool CIKapLineCamera::GetImageSize(int &w, int &h)
{
    if (!IsOpen() && !Open())
    {
        return false;
    }

    try
    {
        CISAPUGuard<JMutex> locker(m_lock);
        int nWidth =0;
        int nHeight =0;
        /* Get image width.*/
        int ret = IKapGetInfo(m_hBoard, IKP_IMAGE_WIDTH, &nWidth);

        /* Get image height.*/
        ret = IKapGetInfo(m_hBoard, IKP_IMAGE_HEIGHT, &nHeight);
        if (nWidth != m_CameraParam.width)
        {
            m_CameraParam.height = nHeight;
            m_CameraParam.width = nWidth;
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

bool CIKapLineCamera::AbortAcq()
{
    if(m_hBoard == INVALID_HANDLE_VALUE)
    {
        return false;
    }
	CISAPUGuard<JMutex> autoLock(m_Acqlock);

    int ret = IKapStopGrab(m_hBoard);
	m_bstartGrap = false;
	m_nCurFrameIndex = 0;
    m_errorCode = ERR_OK;
    setCamStatus(STATUS_CAM_READ_ABORT);
	
    return true;
}

bool CIKapLineCamera::Open()
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
            m_hBoard = IKapOpen( IKBoardPCIE, m_ServerIndex);
            if ( m_hBoard == INVALID_HANDLE_VALUE)
            {
                break;
            }


            int ret = IKapLoadConfigurationFromFile(m_hBoard,(char*) m_CameraParam.strCamConfigPath.c_str());
            if(ret != IK_RTN_OK)
            {
                break;
            }
            
            /* Configure image buffers. */
             ret = IKapSetInfo(m_hBoard, IKP_FRAME_COUNT, IKP_IMAGE_BUFF_COUT);
            if(ret != IK_RTN_OK)
            {
                break;
            }
			int frmae_count;
			ret = IKapGetInfo(m_hBoard, IKP_FRAME_COUNT, &frmae_count);
			if (ret != IK_RTN_OK)
			{
				break;
			}

            /* Configure timeout. */
            int timeout = -1;
            ret = IKapSetInfo(m_hBoard, IKP_TIME_OUT, timeout);
            if(ret !=IK_RTN_OK)
            {
                break;
            }

            /* Configure grab start mode. */
            int grab_mode = IKP_GRAB_NON_BLOCK;
            ret = IKapSetInfo(m_hBoard, IKP_GRAB_MODE, grab_mode);
            if(ret !=IK_RTN_OK)
            {
                break;
            }

            /* Configure transfer mode. */
            int transfer_mode = IKP_FRAME_TRANSFER_SYNCHRONOUS_NEXT_EMPTY_WITH_PROTECT;
            ret = IKapSetInfo(m_hBoard, IKP_FRAME_TRANSFER_MODE, transfer_mode);
            if(ret !=IK_RTN_OK)
            {
                break;
            }


            m_CameraParam.height = m_CameraParam.blockSize;

            CreateImageBuffer();

            if(m_pCSerialPortControl != NULL)
            {
                //if(!m_pCSerialPortControl->IsOpen())
                //{
                //    string ComName = "COM13";
                //    m_pCSerialPortControl->OpenPort(ComName.c_str());
                //}
            }

			SetTriggerMode(m_CameraParam.bExtTrigger);
            SetExposureTime(m_CameraParam.exposure);
            SetGain(m_CameraParam.gain);
            SetCameraFrequency(m_CameraParam.grabFreq);
            bRet = true;
			RegisterCallback();

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

bool CIKapLineCamera::Close()
{
    CISAPUGuard<JMutex> locker(m_lock);
	bool bRet = true;
    if(m_hBoard != INVALID_HANDLE_VALUE)
    {

        UnRegisterCallback();

        /* Close board. */
        int ret = IKapClose(m_hBoard);
        if(ret != IK_RTN_OK)
        {

        }
        m_hBoard = INVALID_HANDLE_VALUE;
    }

    m_iCurrentIndex = 0;
	m_iRecvCount = 0;
    m_nCurFrameIndex = 0;
    setCamStatus(STATUS_CAM_OFF_LINE);
    return bRet;
}

bool CIKapLineCamera::IsOpen()
{
    CISAPUGuard<JMutex> locker(m_lock);
    if(m_hBoard == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    return true;
}

void CIKapLineCamera::Reset()
{
    CISAPUGuard<JMutex> locker(m_imageLock);
    m_AcqImageEvent.ResetEvent();
    m_ReadImageEvent.ResetEvent();
    m_GrabFinishedEvent.ResetEvent();

    m_iCurrentIndex = 0;
   // m_nCurFrameIndex = 0;
	m_iRecvCount = 0;
}

bool CIKapLineCamera::SetGain( double GainValue )
{
    if (GainValue < 1.0)
    {
        return false;
    }

    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;

    try
    {
//        if (NULL != m_camDevice && m_camDevice->SetGain(GainValue))
//        {
//            m_CameraParam.gain = GainValue;
//            bRet = true;
//        }

        if(m_pCSerialPortControl != NULL && m_pCSerialPortControl->IsOpen())
        {
            char buff[128] = {0};
            sprintf(buff,"angn=%d\r",GainValue);
            bool bSuccess = m_pCSerialPortControl->WriteDataToPort(buff,strlen(buff),2000);
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

bool CIKapLineCamera::SetExposureTime( double ExpTimeValue )
{
    bool bRet = false;
    if (ExpTimeValue < 1.0)
    {
        return false;
    }
    CISAPUGuard<JMutex> locker(m_lock);
    try
    {
        if(m_pCSerialPortControl != NULL && m_pCSerialPortControl->IsOpen())
        {
            char buff[128] = {0};
            sprintf(buff,"texp=%d\r",ExpTimeValue);
            bool bSuccess = m_pCSerialPortControl->WriteDataToPort(buff,strlen(buff),2000);
            m_CameraParam.exposure = ExpTimeValue;
            bRet = true;
        }

    }
    catch (...)
    {
        bRet = false;
    }
    return bRet;
}

bool CIKapLineCamera::SetCameraFrequency(unsigned int Freq)
{
    bool bRet = false;
    if (Freq < 1)
    {
        return false;
    }
    CISAPUGuard<JMutex> locker(m_lock);
    try
    {
//        RETURN_V_IF(m_CameraParam.bExtTrigger, true);
//        if (NULL != m_camDevice && m_camDevice->SetCameraFrequency(Freq))
//        {
//            m_CameraParam.grabFreq = Freq;
//            bRet = true;
//        }
    }
    catch (...)
    {
        LOG_WARN("===============[cam]set AcquisitionLineRate param failed.===============");
        bRet = false;
    }
    return bRet;
}

bool CIKapLineCamera::CreateImageBuffer()
{

    /* This function will be create buffer.*/
    int ret = 0;
    int nValue = 0;
    int nWidth =0;
    int nHeight =0;
    int nChannels = 3;
    int nDepth = 8;

    if(m_hBoard == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    /* Get image width.*/
    ret = IKapGetInfo(m_hBoard, IKP_IMAGE_WIDTH, &nWidth);
    if(ret != IK_RTN_OK)
    {
       return false;
    }


    /* Get image height.*/
    ret = IKapGetInfo(m_hBoard, IKP_IMAGE_HEIGHT, &nHeight);
    if(ret != IK_RTN_OK)
    {
       return false;
    }

    /* Get data format.*/
    ret = IKapGetInfo(m_hBoard, IKP_DATA_FORMAT, &nValue);
    if(ret != IK_RTN_OK)
    {
       return false;
    }
    if (nValue == 8)
        nDepth = 8;
    else
        nDepth = 16;

    /* Get image type.*/
    ret = IKapGetInfo(m_hBoard, IKP_IMAGE_TYPE, &nValue);
    if(ret != IK_RTN_OK)
    {
       return false;
    }

    /* Get format value.*/
    switch (nValue)
    {
    case 0:
        nChannels = 1;
        break;
    case 1:
    case 3:
        nChannels = 3;
        break;
    case 2:
    case 4:
        nChannels = 4;
        break;
    default:
        break;
    }


    if (m_pImageInfo.width != nWidth || m_pImageInfo.height != nHeight)
    {
        if (NULL != m_pImageInfo.pixelR)
        {
            delete[]m_pImageInfo.pixelR;
            m_pImageInfo.pixelR = NULL;
        }
    }
    if (NULL == m_pImageInfo.pixelR)
    {
        m_pImageInfo.width = nWidth;
        m_pImageInfo.height = nHeight;
        int iCount = nWidth * nHeight * sizeof(BYTE);
        m_pImageInfo.pixelR = new BYTE[iCount];
    }
    return true;
}

bool CIKapLineCamera::InsertBuffer( void *pData, int width, int height/* = 1*/)
{
    unsigned char *image = (unsigned char *)pData;

    if (width == m_pImageInfo.width && height == m_pImageInfo.height)
    {
        int iCount = width * height * sizeof(BYTE);
        memcpy(m_pImageInfo.pixelR, image, iCount);
    }
	m_iCurrentIndex++;
	m_iRecvCount += height;
    LOG_INFO_F_SIMP("Trace_IKapLineCamera.log","-------> InsertBuffer(%s-total:%d)---%d--%d)", m_CameraParam.strCameraID.c_str(), m_iRecvCount, m_iCurrentIndex, height);
	
    if (m_fnTriggerFunction != NULL)
    {
        m_fnTriggerFunction(m_pInParam, image, width, height);
    }
    else
    {
        LOG_INFO("===============Callback is NULL.===============");
    }
    return true;
}

void CIKapLineCamera::ThreadInitCam(void* param)
{
    if (!IsOpen())
    {
        Open();
    }
}

bool CIKapLineCamera::init()
{
    bool bRet = false;
    return bRet;
}



bool CIKapLineCamera::IsImplemented()
{
    m_bIsColorFilter = false;

    // Get image type
    int nImageType =IKP_IMAGE_TYPE_VAL_MONOCHROME;
    if(m_hBoard != INVALID_HANDLE_VALUE)
    {
        int iRet = IKapGetInfo( m_hBoard, IKP_IMAGE_TYPE, &nImageType) ;

    }
    m_bIsColorFilter = nImageType !=IKP_IMAGE_TYPE_VAL_MONOCHROME;

    return m_bIsColorFilter;
}


bool CIKapLineCamera::SetCameraInParam( LINE_CAM_PARAM &param )
{
    bool bRet = false;
    do 
    {
        bool bIsNeedClose = false;
        m_CameraParam.bIsInvert = param.bIsInvert;
        //step 1. 设置必须关闭相机的参数
        //step 2. 需要重启的配置
        if ( INVALID_HANDLE_VALUE != m_hBoard)
        {
            int nHeight = 0;
            /* Get image height.*/
            int ret = IKapGetInfo(m_hBoard, IKP_IMAGE_HEIGHT, &nHeight);
            if (nHeight != param.blockSize )
            {
                ret = IKapSetInfo(m_hBoard, IKP_IMAGE_HEIGHT, param.blockSize);				
            }

			if (param.snapheight / param.blockSize > 0)
			{
				m_board_frmae_count = param.snapheight / param.blockSize;
				int ret = IKapSetInfo(m_hBoard, IKP_FRAME_COUNT, IKP_IMAGE_BUFF_COUT);
				if (ret != 1)
				{
					LOG_INFO("===============IKapSetInfo false ===============");
				}
			}
        }
        if (param.blockSize != m_CameraParam.blockSize)
        {
            m_CameraParam.blockSize = param.blockSize;
        }

        if (m_CameraParam.strCamConfigPath != param.strCamConfigPath)
        {
            m_CameraParam.strCamConfigPath = param.strCamConfigPath;
            bIsNeedClose = true;//如果配置文件路径有改变， 则重新打开相机
        }
        if (m_CameraParam.height != param.height )
        {
            CreateImageBuffer();
        }
        if (bIsNeedClose && IsOpen())
        {
            Close();
        }

        m_CameraParam = param;
        m_CameraParam.height = m_CameraParam.snapheight;
        
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

void CIKapLineCamera::ClearBuf()
{
    m_iCurrentIndex = 0;
	m_iRecvCount = 0;
    m_nCurFrameIndex = 0;
}

bool CIKapLineCamera::Continuations(bool bOpen, int iSize/* = 1*/)
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
    if (INVALID_HANDLE_VALUE !=  m_hBoard)
    {
        CISAPUGuard<JMutex> locker(m_lock);
        if (bOpen)
        {
            ClearRecvBuffer();

            int nHeight =0;
            /* Get image height.*/
            int ret = IKapGetInfo(m_hBoard, IKP_IMAGE_HEIGHT, &nHeight);

			// 埃及技术反馈,行高不能太小，如果设置太小 *200 进行放大
			//if (iSize < 10)
			//{
			//	iSize = iSize * 200;
			//}
			iSize = m_CameraParam.blockSize;
            if (nHeight != iSize)
            {
                 AbortAcq();
                 int ret = IKapSetInfo(m_hBoard, IKP_IMAGE_HEIGHT, iSize);
                CreateImageBuffer();
            }
            

			if (m_CameraParam.snapheight / m_CameraParam.blockSize > 0)
			{
				m_board_frmae_count = m_CameraParam.snapheight / m_CameraParam.blockSize;
				ret = IKapSetInfo(m_hBoard, IKP_FRAME_COUNT, IKP_IMAGE_BUFF_COUT);
				if (ret != IK_RTN_OK)
				{
					;
				}
				int frmae_count;
				ret = IKapGetInfo(m_hBoard, IKP_FRAME_COUNT, &frmae_count);
				m_board_frmae_count = frmae_count;
				if (ret != IK_RTN_OK)
				{
					;
				}
			}
			m_bIsGrab = true;
			ret = IKapStartGrab(m_hBoard, 0);
			if (ret != 1)
			{
				LOG_INFO("===============[cam]start grab(%s).===============", m_CameraParam.strCameraName.c_str());
			}
        }
       else
        {
            m_bIsGrab = false;
            AbortAcq();
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

bool CIKapLineCamera::IsContinuations()
{
    bool bRet = true;
    if ((!IsOpen() && !Open()))
    {
        return false;
    }
    int iGrab = 0;
    int ret = IKapGetInfo(m_hBoard, IKP_GRAB_STATUS, &iGrab);
    return iGrab ==1;
}


int CIKapLineCamera::GetCamStatus() 
{
    CISAPUGuard<JMutex> locker(m_camStatusLock); 
    return m_CamStatus;
}

void CIKapLineCamera::setCamStatus(unsigned long status)
{
    CISAPUGuard<JMutex> locker(m_camStatusLock);
    m_CamStatus = /*m_CamStatus | */status;
}

int CIKapLineCamera::GetLastError(char *errInfo/* = NULL*/, int *iSize/* = NULL*/)
{
    //int iLength = 0;
    if (NULL != iSize)
    {
        IKAPERRORINFO pIKErrInfo;
        memset( &pIKErrInfo, 0, sizeof(IKAPERRORINFO) );
        IKapGetLastError( &pIKErrInfo, true );
    }
    return m_errorCode;
}

void CIKapLineCamera::SetTriggerFunction( FnLineTriggerFunction fnTriggerFunction, void *pInParam )
{
    CISAPUGuard<JMutex> locker(m_lock);
    m_fnTriggerFunction = fnTriggerFunction;
    m_pInParam = pInParam;
}

bool CIKapLineCamera::IsInitFinished()
{
    return !m_pThreadOpenCam->IsRunning();
}

bool CIKapLineCamera::IsSnapFinished()
{
    bool bRet = true;
    if ((!IsOpen() && !Open()))
    {
        return false;
    }
    int iGrab = 0;
    if(m_hBoard != INVALID_HANDLE_VALUE)
    {
        int ret = IKapGetInfo(m_hBoard, IKP_GRAB_STATUS, &iGrab);
    }

    return iGrab == 1;
}

bool CIKapLineCamera::WaitGetImageFinished(int iTimeOut)
{
    long time = iTimeOut; 
    if (JEVENT_WAIT_OK == JEvent::WaitForSingleObject(&m_ReadImageEvent, time))
    {
        return true;
    }
    return false;
}

bool CIKapLineCamera::ResetFinishState()
{
    CISAPUGuard<JMutex> locker(m_lock);
    m_ReadImageEvent.ResetEvent();
    m_GrabFinishedEvent.ResetEvent();
    return true;
}
bool CIKapLineCamera::SetTriggerMode( bool triger )
{
	bool bRet = false;
	do 
    {
        //int ret = IKapSetInfo(m_hBoard, IKP_IMAGE_HEIGHT, param.blockSize);

        //int ret = IKapGetInfo(m_hBoard, IKP_IMAGE_WIDTH, &nWidth);

        //IKP_BOARD_TRIGGER_MODE_VAL_INNER
//        if (NULL != m_camDevice && !m_camDevice->SetTriggerMode(triger))
//        {
//            break;
//        }
//		BOOL bIsTriger;
//        int emTrigger;
//        BREAK_IF(!m_Acq->GetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, &bIsTriger));
//		if (bIsTriger != triger)
//		{
//			if (triger)
//			{
//                BREAK_IF(!m_Acq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, 1));
//			}
//			else
//			{
//                BREAK_IF(!m_Acq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, 0));
//			}
//			m_CameraParam.bExtTrigger = triger;
//		}
		bRet = true;
	} while (0);
	return bRet;
}

void CIKapLineCamera::ClearRecvBuffer()
{

}

void CIKapLineCamera::UnRegisterCallback()
{
    int ret = IK_RTN_OK;
    if(m_hBoard != INVALID_HANDLE_VALUE)
    {
        ret = IKapUnRegisterCallback(m_hBoard, IKEvent_GrabStart);
        ret = IKapUnRegisterCallback(m_hBoard, IKEvent_FrameReady);
        ret = IKapUnRegisterCallback(m_hBoard, IKEvent_FrameLost);
        ret = IKapUnRegisterCallback(m_hBoard, IKEvent_TimeOut);
        ret = IKapUnRegisterCallback(m_hBoard, IKEvent_GrabStop);
    }

}

void CIKapLineCamera::RegisterCallback()
{
    int ret = IK_RTN_OK;
    if(m_hBoard != INVALID_HANDLE_VALUE)
    {
        ret = IKapRegisterCallback(m_hBoard, IKEvent_GrabStart, OnGrabStart, this);
        ret = IKapRegisterCallback(m_hBoard, IKEvent_FrameReady, OnFrameReady, this);
        ret = IKapRegisterCallback(m_hBoard, IKEvent_FrameLost, OnFrameLost, this);
        ret = IKapRegisterCallback(m_hBoard, IKEvent_TimeOut, OnTimeout, this);
        ret = IKapRegisterCallback(m_hBoard, IKEvent_GrabStop, OnGrabStop, this);
    }
}

bool CIKapLineCamera::GetOption(char* lpConfigFileName)
{
//    OPENFILENAME iniofn;
//    wchar_t	gcMsgBuf[MAX_PATH] = _T("\0");
//    wchar_t  lpTempFileName[_MAX_PATH] = _T("\0");

//    GetModuleFileName(NULL, gcMsgBuf, MAX_PATH);
//    PathRemoveFileSpec(gcMsgBuf);

//    iniofn.lStructSize = sizeof(OPENFILENAME);
//    iniofn.hwndOwner = NULL;
//    iniofn.lpstrFilter = _T("Configuration File(*.vlcf)\0*.vlcf\0");
//    iniofn.lpstrCustomFilter = NULL;
//    iniofn.nFilterIndex = 1;
//    iniofn.lpstrFile = lpTempFileName;
//    iniofn.nMaxFile = MAX_PATH;
//    iniofn.lpstrFileTitle = NULL;
//    iniofn.nMaxFileTitle = MAX_PATH;
//    iniofn.lpstrInitialDir = gcMsgBuf;
//    iniofn.lpstrTitle = _T("The 'Vulcan Configuration File' of the purpose read is chosen.");
//    iniofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
//    iniofn.lpstrDefExt = _T("vlcf");

//    if (GetOpenFileName(&iniofn) == FALSE)
//        return false;

//    WideCharToMultiByte(CP_OEMCP, 0, lpTempFileName, (int)_tcsclen(lpTempFileName) + 1, lpConfigFileName, _MAX_PATH, NULL, NULL);

    return true;
}


/* This callback function will be called on grabbing start. */
void CALLBACK OnGrabStart(void* pContext)
{
    CIKapLineCamera* pCIKapLineCamera = (CIKapLineCamera*)(pContext);
    LOG_INFO_F_SIMP("Trace_IKapLineCamera.log","-------> OnGrabStart(%s))", pCIKapLineCamera->m_CameraParam.strCameraID.c_str());
}

/* This callback function will be called on grabbing stop. */
void CALLBACK OnFrameReady(void* pContext)
{

    CIKapLineCamera* pCIKapLineCamera = (CIKapLineCamera*)(pContext);
	CISAPUGuard<JMutex> autoLock(pCIKapLineCamera->m_Acqlock);
	if (!pCIKapLineCamera->m_bstartGrap && !pCIKapLineCamera->m_bIsGrab)
	{
		return;
	}

    //LOG_INFO_F_SIMP("Trace_IKapLineCamera.log","-------> OnFrameReady(%s))", pCIKapLineCamera->m_CameraParam.strCameraID.c_str());
    HANDLE hDev = (HANDLE)pCIKapLineCamera->m_hBoard;
    unsigned char* pUserBuffer = NULL;
    int nFrameSize = 0;
    int nFrameCount = 0;
    IKAPBUFFERSTATUS status;
    IKapGetInfo( hDev, IKP_FRAME_COUNT, &nFrameCount );
    IKapGetBufferStatus( hDev, pCIKapLineCamera->m_nCurFrameIndex, &status );
    if ( status.uFull == 1 )
    {
        // get frame grabber buffer
        IKapGetInfo( hDev, IKP_FRAME_SIZE, &nFrameSize );
        IKapGetBufferAddress( hDev, pCIKapLineCamera->m_nCurFrameIndex, (void**)&pUserBuffer );
        int w;
        int h;
        pCIKapLineCamera->GetImageSize(w, h);
		LOG_INFO_F_SIMP("Trace_IKapLineCamera.log", "-------> OnFrameReady(%s)  nFrameSize = %d,w=%d ,h=%d)", pCIKapLineCamera->m_CameraParam.strCameraID.c_str(), nFrameSize,  w, nFrameSize / w);
        pCIKapLineCamera->InsertBuffer(pUserBuffer, w, nFrameSize/w);

    }
    pCIKapLineCamera->m_nCurFrameIndex++;
    pCIKapLineCamera->m_nCurFrameIndex = pCIKapLineCamera->m_nCurFrameIndex % pCIKapLineCamera->m_board_frmae_count;
}

/* This callback function will be called on grabbing timeout. */
void CALLBACK OnTimeout(void* pContext)
{
    CIKapLineCamera* pCIKapLineCamera = (CIKapLineCamera*)(pContext);
    LOG_INFO_F_SIMP("Trace_IKapLineCamera.log","-------> OnTimeout(%s))", pCIKapLineCamera->m_CameraParam.strCameraID.c_str());
}

/* This callback function will be called on frame lost. */
void CALLBACK OnFrameLost(void* pContext)
{
    CIKapLineCamera* pCIKapLineCamera = (CIKapLineCamera*)(pContext);
    LOG_INFO_F_SIMP("Trace_IKapLineCamera.log","-------> OnFrameLost(%s))", pCIKapLineCamera->m_CameraParam.strCameraID.c_str());
}

/* This callback function will be called on grab stop. */
void CALLBACK OnGrabStop(void* pContext)
{
    CIKapLineCamera* pCIKapLineCamera = (CIKapLineCamera*)(pContext);
    LOG_INFO_F_SIMP("Trace_IKapLineCamera.log","-------> OnGrabStop(%s))", pCIKapLineCamera->m_CameraParam.strCameraID.c_str());
}

