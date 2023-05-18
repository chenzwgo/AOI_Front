#include "dalsamanager.h"
#include "dalsalinecamera.h"

#include "SapClassBasic.h"
#include "trace.h"
#include "rtdefine.h"

#define STRING_LENGTH		256 

CDalsaCameraManager * CDalsaCameraManager::m_pInstance = NULL;

void CDalsaCameraManager::ServerCallback( SapManCallbackInfo *pInfo )
{
    int serverIndex = pInfo->GetServerIndex();
    CDalsaCameraManager *pManager = static_cast<CDalsaCameraManager *>(pInfo->GetContext());
    char serverName[64];
    SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));

    switch (pInfo->GetEventType())
    {
        //Camera %s was connected for the first time
    case SapManager::EventServerNew:
        break;
        //Camera %s was disconnected
    case SapManager::EventServerDisconnected:
    {
        LOG_WARN("===============[manager]cam disconnected(%s).===============", serverName);
        map<string, cameraAcqInfo *>::iterator it = pManager->m_camAcqList.find(serverName);
        if (it != pManager->m_camAcqList.end() && NULL != it->second->pCam)
        {
            CDalsaLineCamera *pCam = static_cast<CDalsaLineCamera *>(it->second->pCam);
            pCam->setCamStatus(ILineCamera::STATUS_CAM_OFF_LINE);
        }
    }
        break;
        //Camera %s was reconnected
    case SapManager::EventServerConnected:
        break;
    }
}

void CDalsaCameraManager::ThreadInitCam(void* param)
{
    LOG_INFO("===============[manager]open start===============");
    try
    {
        BOOL bRet = SapManager::Open();
		bRet = true;
        if (bRet)
        {
            LOG_INFO("===============[manager]open finished===============");
            SapManager::RegisterServerCallback(SapManager::EventServerNew | SapManager::EventServerDisconnected
                | SapManager::EventServerConnected | SapManager::EventResourceInfoChanged, ServerCallback, this);
            RETURN_IF(!m_bIsRunning);
            if (FindCamera())
            {
                CISAPUGuard<JMutex> locker(m_lock);
                m_bIsInit = true;
            }
        }
        else
        {
            LOG_WARN("===============[manager]open failed===============");
        }
    }
    catch (...)
    {
    	
    }
    LOG_INFO("===============[manager]init finished===============");
}

CDalsaCameraManager::CDalsaCameraManager(void)
    : m_errorCode(0)
    , m_bIsStartUp(false)
    , m_bIsInit(false)
    , m_pThreadInit(NULL)
	, m_bIsRunning(false)
	, m_bCallInit(false)
{
    m_lock.Init();
}


CDalsaCameraManager::~CDalsaCameraManager(void)
{
}


CDalsaCameraManager * CDalsaCameraManager::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CDalsaCameraManager;
    }
    return m_pInstance;
}

void CDalsaCameraManager::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

bool CDalsaCameraManager::init()
{
	m_bCallInit = true;
    bool bRet = false;
    CISAPUGuard<JMutex> locker(m_lock);
    LOG_INFO("===============[manager]init start===============");
    if (NULL == m_pThreadInit)
    {
        m_pThreadInit = new JThreadTP<CDalsaCameraManager, void*, void>;
    }
    if (!m_bIsInit && !m_pThreadInit->IsRunning())
    {
        m_bIsRunning = true;
        m_pThreadInit->StartThread(this, &CDalsaCameraManager::ThreadInitCam, this);
    }
    return bRet;
}

bool CDalsaCameraManager::deinit()
{
    m_bIsRunning = false;
    while (NULL != m_pThreadInit && m_pThreadInit->IsRunning())
    {
        Sleep(1000);
    }
    CISAPUGuard<JMutex> locker(m_lock);
    for (map<string, cameraDeviceInfo *>::iterator it = m_camDeviceList.begin(); it != m_camDeviceList.end(); ++it)
    {
        cameraDeviceInfo *pInfo = it->second;
        if (pInfo->pDevice)
        {
            pInfo->pDevice->Destroy();
            delete pInfo->pDevice;
            pInfo->pDevice = NULL;
        }
        delete pInfo;
    }
    for (map<string, cameraAcqInfo *>::iterator it = m_camAcqList.begin(); it != m_camAcqList.end(); ++it)
    {
        cameraAcqInfo *pInfo = it->second;
        if (pInfo->pCam)
        {
			if (pInfo->pCam->IsOpen())
			{
				pInfo->pCam->Close();
			}
            delete pInfo->pCam;
            pInfo->pCam = NULL;
        }
        delete pInfo;
    }
	m_camAcqList.clear();
    SapManager::UnregisterServerCallback();
    SapManager::Close();

    return true;
}

bool CDalsaCameraManager::GetCamList( std::vector<std::string> &vecGUID,unsigned long /*MaxCnt*/ )
{
    CISAPUGuard<JMutex> locker(m_lock);

    int serverCount = 0;
    int serverIndex = 0;
    char serverName[STRING_LENGTH];

    if (0 <= m_vecCamList.size())
    {
        if (m_bIsStartUp && SapManager::DetectAllServers(SapManager::DetectServerAll))
        {
            // let time for the detection to execute
            Sleep(5000);
        }
        m_bIsStartUp = true;
        vecGUID.clear();

        serverCount = SapManager::GetServerCount();

        for (serverIndex = 0; serverIndex < serverCount; serverIndex++)
        {
            if (SapManager::IsResourceAvailable(serverIndex, SapManager::ResourceAcq, 0))
            {
                SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
                bool bFound = false;
                vector<string>::iterator itCam = m_vecCamList.begin();
                for (; itCam != m_vecCamList.end(); ++itCam)
                {
                    if (0 == itCam->compare(serverName))
                    {
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    m_vecCamList.push_back(serverName);
                }
            }
        }
    }
    vecGUID = m_vecCamList;
    return true;
}

ILineCamera * CDalsaCameraManager::CreateCamera( LINE_CAM_PARAM *param )
{
    if (NULL == param || param->strCameraID.size() <= 0)
    {
        return NULL;
    }
    if (!m_bIsInit)
    {
        init();
    }

    CISAPUGuard<JMutex> locker(m_lock);
    BOOL status = FALSE;
    int featureCount = 0;
    char serialNumberName[STRING_LENGTH];
    cameraAcqInfo *pInfo = NULL;
    map<string, cameraAcqInfo *>::iterator it;
    for ( it = m_camAcqList.begin(); it != m_camAcqList.end(); ++it)
    {
        pInfo = it->second;
        if (NULL == pInfo)
        {
            return false;
        }
        if (0 == pInfo->strCameraName.compare(param->strCameraID) && param->bEable)
        {
            if (NULL == pInfo->pCam)
            {
                //ID创建后就不能修改， 1个ID对应一个相机实例
                CDalsaLineCamera *pCam = new CDalsaLineCamera(pInfo->strCameraName.c_str());
                if (NULL != pCam)
                {
                    pCam->InitCam(pInfo->serverIndex, pInfo->resourceIndex, pInfo->strDeviceName.c_str(), param);
					pInfo->pCam = pCam;
                    //pCam->SetCameraInParam(*param); //同步配置参数
                }
			}
            break;
        }
    }
    return NULL != pInfo ? pInfo->pCam : NULL;
}

bool CDalsaCameraManager::isCameraInitFinished()
{
    CISAPUGuard<JMutex> locker(m_lock);
	if (!m_bCallInit)
		init();
    return m_bIsInit;
}

bool CDalsaCameraManager::FindCamera()
{
    int serverCount = 0;
    int serverIndex = 0;
    char userDefinedName[STRING_LENGTH];
    char strBuffer[STRING_LENGTH];
    serverCount = SapManager::GetServerCount();
    m_camDeviceList.clear();
    m_camAcqList.clear();
	map<string, string> mapDeviceName;
    for (serverIndex = 0; serverIndex < serverCount; serverIndex++)
    {
        RETURN_V_IF(!m_bIsRunning, false);
        if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
		{
			memset(strBuffer, 0, sizeof(char) * STRING_LENGTH);
            SapManager::GetServerName(serverIndex, strBuffer, sizeof(strBuffer));
			string strServerName = strBuffer;
            if (mapDeviceName.end() == mapDeviceName.find(strBuffer))
            {
                memset(strBuffer, 0, sizeof(char) * STRING_LENGTH);
                LOG_INFO("===============[manager]GetResourceName begin===============");
                SapManager::GetResourceName(strServerName.c_str(), SapManager::ResourceAcqDevice, 0, strBuffer, sizeof(char) * STRING_LENGTH);
                LOG_INFO("===============[manager]GetResourceName end===============");
				mapDeviceName.insert(make_pair(strServerName, strBuffer));
            }
        }
        else if (SapManager::IsResourceAvailable(serverIndex, SapManager::ResourceAcq, 0))
		{
			memset(strBuffer, 0, sizeof(char) * STRING_LENGTH);
            SapManager::GetServerName(serverIndex, strBuffer, sizeof(strBuffer));
            if (m_camAcqList.end() == m_camAcqList.find(strBuffer))
            {
                cameraAcqInfo *pInfo = new cameraAcqInfo;
                pInfo->strCameraName = strBuffer;
                pInfo->serverIndex = serverIndex;
                m_camAcqList.insert(make_pair(strBuffer, pInfo));
            }
        }
    }
	for (map<string, string>::iterator itM = mapDeviceName.begin(); itM != mapDeviceName.end(); ++itM)
    {
        RETURN_V_IF(!m_bIsRunning, false);
		//将AcqDevice关联到Acq -- 将相机与卡关联
		for (map<string, cameraAcqInfo *>::iterator itC = m_camAcqList.begin(); itC != m_camAcqList.end(); ++itC)
		{
			if (string::npos != itM->second.find(itC->second->strCameraName)) //AcqDevice枚举到的name有附加信息
			{
				itC->second->strDeviceName = itM->first;
				break;
			}
		}
	}
    return true;
}