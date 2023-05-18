#include "hiksourcecamera.h"
#include "hiksourcecameramanager.h"

CHIKSourceCameraManagerPlugin *CHIKSourceCameraManagerPlugin::m_pInstance = NULL;


CHIKSourceCameraManagerPlugin::CHIKSourceCameraManagerPlugin(void)
{

}

CHIKSourceCameraManagerPlugin::~CHIKSourceCameraManagerPlugin(void)
{

}

CHIKSourceCameraManagerPlugin * CHIKSourceCameraManagerPlugin::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CHIKSourceCameraManagerPlugin;
    }
    return m_pInstance;
}

void CHIKSourceCameraManagerPlugin::releaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

bool CHIKSourceCameraManagerPlugin::init()
{
    
    return true;
}

bool CHIKSourceCameraManagerPlugin::deinit()
{
    CISAPUGuard<JMutex> locker(m_lock);

    // 删除相机
    for (CameraIterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
    {
        if (it->second->IsOpen())
        {
            it->second->Close();
        }
        delete it->second;
    }
    m_cameraList.clear();
    return true;
}

bool CHIKSourceCameraManagerPlugin::GetCamList( std::vector<std::string> &vecGUID,unsigned long MaxCnt )
{
    char szBuf[20] = {0};
    long long iSN = 0;
    bool bRet = false;

    //枚举设备
    CISAPUGuard<JMutex> locker(m_lock);
    memset(&m_mvDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = CMyCamera::EnumDevices(&m_mvDevList);
    if (MV_OK != nRet)
    {
        return false;
    }


    //拼接相机SN
    for (int i = 0; i < m_mvDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_mvDevList.pDeviceInfo[i];
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
        else
        {
        }
        if ("" != strSerialNumber)
        {
            vecGUID.push_back(strSerialNumber);
        }
    }
    return true;
}

ICamera * CHIKSourceCameraManagerPlugin::CreateCamera( const char *pszGUID, unsigned int index )
{
    if (NULL == pszGUID)
    {
        return NULL;
    }
    

    CISAPUGuard<JMutex> locker(m_lock);
    CameraIterator it = m_cameraList.find(pszGUID);
    if (it == m_cameraList.end())
    {
        CHIKSourceCameraPlugin *pCam = new CHIKSourceCameraPlugin(pszGUID);
        pCam->Open();  //提前打开相机， 解决概率采图失败问题
        m_cameraList.insert(std::make_pair(pszGUID, pCam));
        return pCam;
    }

    return it->second;
}

//Demo
//bool CImagingSourceCameraPlugin::motoron( int i1,int i2 )
//{
//	return true;
//}



