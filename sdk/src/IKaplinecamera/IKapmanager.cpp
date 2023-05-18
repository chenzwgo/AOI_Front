#include "IKapmanager.h"
#include "IKaplinecamera.h"

#include "trace.h"
#include "rtdefine.h"

#define STRING_LENGTH		256

CIKAPCameraManager * CIKAPCameraManager::m_pInstance = NULL;


void CIKAPCameraManager::ThreadInitCam(void* param)
{
    LOG_INFO("===============[manager]open start===============");
    try
    {
        bool bRet = true;
        if (bRet)
        {
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

CIKAPCameraManager::CIKAPCameraManager(void)
    : m_errorCode(0)
    , m_bIsStartUp(false)
    , m_bIsInit(false)
    , m_pThreadInit(NULL)
	, m_bIsRunning(false)
	, m_bCallInit(false)
{
    m_lock.Init();
}


CIKAPCameraManager::~CIKAPCameraManager(void)
{
}


CIKAPCameraManager * CIKAPCameraManager::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CIKAPCameraManager;
    }
    return m_pInstance;
}

void CIKAPCameraManager::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

bool CIKAPCameraManager::init()
{
	m_bCallInit = true;
    bool bRet = false;
    CISAPUGuard<JMutex> locker(m_lock);
    LOG_INFO("===============[manager]init start===============");
    if (NULL == m_pThreadInit)
    {
        m_pThreadInit = new JThreadTP<CIKAPCameraManager, void*, void>;
    }
    if (!m_bIsInit && !m_pThreadInit->IsRunning())
    {
        m_bIsRunning = true;
        m_pThreadInit->StartThread(this, &CIKAPCameraManager::ThreadInitCam, this);
    }
    return bRet;
}

bool CIKAPCameraManager::deinit()
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
//        if (pInfo->pDevice)
//        {
//            pInfo->pDevice->Destroy();
//            delete pInfo->pDevice;
//            pInfo->pDevice = NULL;
//        }
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

    return true;
}

bool CIKAPCameraManager::GetCamList( std::vector<std::string> &vecGUID,unsigned long /*MaxCnt*/ )
{
    CISAPUGuard<JMutex> locker(m_lock);

    int serverCount = 0;
    int serverIndex = 0;
    char serverName[STRING_LENGTH];

    vecGUID.clear();
    map<string, cameraAcqInfo *>::iterator it;
    for (it = m_camAcqList.begin(); it != m_camAcqList.end(); ++it)
    {
        vecGUID.push_back(it->second->strCameraName);
    }
    return true;
}

ILineCamera * CIKAPCameraManager::CreateCamera( LINE_CAM_PARAM *param )
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
                CIKapLineCamera *pCam = new CIKapLineCamera(pInfo->strCameraName.c_str());
                if (NULL != pCam)
                {
                    pCam->InitCam(pInfo->serverIndex, pInfo->resourceIndex, pInfo->strDeviceName.c_str(), param);
					pInfo->pCam = pCam;
                }
			}
            break;
        }
    }
    return NULL != pInfo ? pInfo->pCam : NULL;
}

bool CIKAPCameraManager::isCameraInitFinished()
{
    CISAPUGuard<JMutex> locker(m_lock);
	if (!m_bCallInit)
		init();
    return m_bIsInit;
}

bool CIKAPCameraManager::FindCamera()
{
    int ret = 0;
    unsigned int nPCIeDevCount = 0;
    char configFilename[MAX_PATH];
    char resourceName[MAX_PATH];
    unsigned int resourceNameSize = MAX_PATH;

    // Get the count of PCIe interface boards.
    ret = IKapGetBoardCount( IKBoardPCIE, &nPCIeDevCount );
    if(ret != 1)
    {
        return false;
    }
    if ( nPCIeDevCount == 0)
    {
         return false;
    }

    for ( unsigned int i= 0; i< nPCIeDevCount;i++)
    {
        resourceNameSize = MAX_PATH;
        if ( IKapGetBoardName(IKBoardPCIE , i ,resourceName,&resourceNameSize) == 1 )
        {
            printf("Resource name %s\n" , resourceName);

            if (m_camAcqList.end() == m_camAcqList.find(resourceName))
            {
                cameraAcqInfo *pInfo = new cameraAcqInfo;
                pInfo->strCameraName = resourceName;
                pInfo->serverIndex = i;
                m_camAcqList.insert(make_pair(resourceName, pInfo));
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
