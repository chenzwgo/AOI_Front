#include "cameramanagerplugin.h"
#include "vscameratools/camera/icamerafactory.h"
#include "sdkcore.h"
#include "vision_error.h"
#include "cameramanagererror.h"
#include "vscameratools/camera/icamera.h"
#include "tools/hcameraparam.h"
#include "jdir.h"
#include "tinyxml.h"
#include "vsworkflowplugin/iscworkflow.h"
#include "proxymanager.h"
#include "trace.h"

const char *g_camTypeDesc[] = {
    "AVT",
    "IMAGING",
    "BASLER",
    "BAUMER",
    "DH-MER"
    "Other"
};

const char *g_camTypeName[] = {
    "avtcamera",
    "imgsourcecamera",
    "baslercamera",
    "baumercamera",
    "dhmercamera" ,
    "othercamera"
};

static const char *g_szCameraXmlBuf = {
    "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\
    <vstudiodoc version=\"2.0\">\
    </vstudiodoc>"};

#define CAMERA_MANAGER_PLUGIN_DIR     "/plugins/cameraplugin"
#define CAMERA_MANAGER_PLUGIN_FILE    "/cameraconfig.xml"

CCameraManagerPlugin *CCameraManagerPlugin::m_pInstance = NULL;

CCameraManagerPlugin::CCameraManagerPlugin(void)
{
    m_lock.Init();
}


CCameraManagerPlugin::~CCameraManagerPlugin(void)
{
    UnInit();
}

void CCameraManagerPlugin::Init()
{
    CISAPUGuard<JMutex> locker(m_lock);

    UnInit();
    ReadConfig();
    //先初始化个类型相机插件， 防止baumer相机占用其他相机的资源
    for (int iCount = 0; iCount < CAM_TYPE_COUNT; iCount++)
    {
        sdkcore::queryPriInterface(g_camTypeName[iCount],ICameraFactory::IID_CAMERAFACTORY);
    }
    RefreshValidCameraList();
}

void CCameraManagerPlugin::UnInit()
{
    //m_vecCameraFactory.resize(CAM_TYPE_COUNT);
    //for (int iIndex = 0; iIndex < CAM_TYPE_COUNT; iIndex++)
    //{
    //    m_vecCameraFactory[iIndex] = NULL;
    //}
    CISAPUGuard<JMutex> locker(m_lock);
    map<string, ICamera *>::iterator it;
    m_camList.clear();
    m_camConfigList.clear();
}

CCameraManagerPlugin *CCameraManagerPlugin::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CCameraManagerPlugin();
    }

    return m_pInstance;
}

void CCameraManagerPlugin::ReleaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}
//
//ICameraFactory* CCameraManagerPlugin::GetCameraFactory(int iCamType)
//{
//    if (iCamType > UNKNOWN_CAM && iCamType < CAM_TYPE_COUNT && iCamType < m_vecCameraFactory.size())
//    {
//        return m_vecCameraFactory[iCamType];
//    }
//
//    return NULL;
//}

void CCameraManagerPlugin::EnumCameraDesc(vector<string> &vecCamDesc)
{
    CISAPUGuard<JMutex> locker(m_lock);
    vecCamDesc.clear();
    for (int iIndex = 0; iIndex < CAM_TYPE_COUNT; iIndex++)
    {
        vecCamDesc.push_back(g_camTypeDesc[iIndex]);
    }
}

string CCameraManagerPlugin::GetCamDesc(int iCamType)
{
    CISAPUGuard<JMutex> locker(m_lock);
    if (iCamType > UNKNOWN_CAM && iCamType < CAM_TYPE_COUNT)
    {
        return g_camTypeDesc[iCamType];
    }

    return "";
}

int CCameraManagerPlugin::GetCamType(const string &strCamDesc)
{
    CISAPUGuard<JMutex> locker(m_lock);
    for (int iIndex = 0; iIndex < CAM_TYPE_COUNT; iIndex++)
    {
        if (strCamDesc.compare(g_camTypeDesc[iIndex]) == 0)
        {
            return iIndex;
        }
    }

    return UNKNOWN_CAM;
}

ICamera * CCameraManagerPlugin::GetCameraInstance( string id, int iCamType /*= UNKNOWN_CAM*/ )
{
    CISAPUGuard<JMutex> locker(m_lock);
    ICamera *pCamera = NULL;
    do 
    {
        if (id == "")
        {
            m_lastError = VISION_CAMERAMANAGER_EXECUTE_CAMID_ERROR;
            break;
        }
        map<string, VS_CAMERA_INFO>::iterator itInfo = m_camConfigList.find(id);
        if (itInfo == m_camConfigList.end())
        {
            m_lastError = VISION_CAMERAMANAGER_EXECUTE_CAMID_ERROR;
            break;
        }
        map<string, ICamera *>::iterator it;
        it = m_camList.find(itInfo->second.SerialNumber);
        if (it != m_camList.end() && NULL != it->second )
        {
            pCamera = it->second;
            m_lastError = VISION_SUCCESS;
        }
        else
        {
            m_lastError = VISION_CAMERAMANAGER_EXECUTE_CREATECAMERA_ERROR;
            break;
        }
    } while (0);

    return pCamera;
}

string CCameraManagerPlugin::GetCameraNameWithGuid( string ID )
{
    CISAPUGuard<JMutex> locker(m_lock);
    string strName = "";
    do 
    {
        if (ID == "")
        {
            m_lastError = VISION_CAMERAMANAGER_EXECUTE_CAMID_ERROR;
            break;
        }
        map<string, VS_CAMERA_INFO>::iterator it;
        for (it = m_camConfigList.begin(); it != m_camConfigList.end(); ++it)
        {
            //获取相机别名， 如果与传入匹配则返回
            string strID = it->second.SerialNumber;  
            if (0 == strID.compare(ID))
            {
                strName = it->first;
                break;
            }
        }
    } while (0);

    return strName;
}

int CCameraManagerPlugin::GetCameraNameList( vector<string> &vecName, int iCamType /*= UNKNOWN_CAM*/ )
{
    CISAPUGuard<JMutex> locker(m_lock);
    map<string, VS_CAMERA_INFO>::iterator it = m_camConfigList.begin();
    for (; it != m_camConfigList.end(); ++it)
    {
        vecName.push_back(it->second.strOutName);
    }
    return vecName.size();
}

bool CCameraManagerPlugin::GetCameraInfo( string strName, VS_CAMERA_INFO &info )
{
    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;
    map<string, VS_CAMERA_INFO>::iterator it = m_camConfigList.find(strName);
    if (it != m_camConfigList.end())
    {
        info = it->second;
        bRet = true;
    }
    return bRet;
}

bool CCameraManagerPlugin::SetCameraInParam( VS_CAMERA_INFO &param )
{
    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;
    map<string, VS_CAMERA_INFO>::iterator it = m_camConfigList.find(param.strOutName);
    if (it == m_camConfigList.end())
    {
        m_camConfigList.insert(make_pair(param.strOutName, param));
        it = m_camConfigList.find(param.strOutName);
    }
    //如果找到，且内容不相同， 则设置
    if (it != m_camConfigList.end() && param != it->second)
    {
        map<string, ICamera *>::iterator itCam = m_camList.find(param.SerialNumber);
        if (itCam != m_camList.end() && NULL != itCam->second)
        {
            bRet = itCam->second->SetCameraInParam(param);
            if (bRet)
            {
                it->second = param;
                SaveConfig();
                bRet = true;
            }
        }
    }
    return bRet;
}

bool CCameraManagerPlugin::CreateCameraConfig( string strName, VS_CAMERA_INFO &info )
{
    CISAPUGuard<JMutex> locker(m_lock);
    m_camConfigList.insert(make_pair(strName, info));
    map<string, ICamera *>::iterator it = m_camList.find(info.SerialNumber);
    if (it != m_camList.end())
    {
        it->second->SetCameraInParam(info);
    }
    SaveConfig();
    return true;
}

bool CCameraManagerPlugin::DeleteCameraConfig( string strName )
{
    CISAPUGuard<JMutex> locker(m_lock);
    map<string, VS_CAMERA_INFO>::iterator it = m_camConfigList.find(strName);
    if (it != m_camConfigList.end())
    {
        m_camConfigList.erase(it);
        SaveConfig();
        return true;
    }
    return false;
}

bool CCameraManagerPlugin::UpdateCameraName(VS_CAMERA_INFO &info, string strOldName )
{
    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;
    do 
    {
        //查找相机名称是否有效
        map<string, VS_CAMERA_INFO>::iterator it = m_camConfigList.find(strOldName);
        if (it == m_camConfigList.end())
        {
            break;
        }

        //更新原有属性
        VS_CAMERA_INFO cameraInfo = it->second;
        m_camConfigList.erase(it);
        cameraInfo.strOutName = info.strOutName;
        m_camConfigList.insert(make_pair(info.strOutName, cameraInfo));

        //更新相机内参数
        map<string, ICamera *>::iterator itC = m_camList.find(info.SerialNumber);
        VS_CAMERA_INFO cameraCurrent;
        if (itC != m_camList.end())
        {
            itC->second->GetCameraInfo(cameraCurrent);
            if (cameraCurrent != info)
            {
                itC->second->SetCameraInParam(info);
            }
        }
        SaveConfig();

        bRet = true;
    } while (0);

    return bRet;
}

bool CCameraManagerPlugin::ReadConfig()
{
    // 配置文件放在App同级目录，首先要获取App路径
    JDir dir;
    string strPath = dir.getCurrentPath();
    strPath += CAMERA_MANAGER_PLUGIN_DIR;
    strPath += CAMERA_MANAGER_PLUGIN_FILE;

    TiXmlDocument docConfig;
    bool bLoadOkay = docConfig.LoadFile(strPath);
    if (!bLoadOkay)
    {
        return false;
    }

    TiXmlElement *pRoot = docConfig.RootElement();
    if (pRoot == NULL 
        || pRoot->Value() == NULL
        || strcmp(pRoot->Value(), "vstudiodoc") != 0
        || pRoot->Attribute("version") == NULL
        || strcmp(pRoot->Attribute("version"), "2.0") != 0)
    {
        return false;
    }

    TiXmlElement *pCameraElem = pRoot->FirstChildElement("camera");
    while (pCameraElem != NULL)
    {
        //ReadJob(pJobElem, szDirName);
        TiXmlElement *pItem = pCameraElem->FirstChildElement("item");
        if (NULL != pItem)
        {
            VS_CAMERA_INFO info;
            const char *szTmp = pItem->Attribute("sn");
            if (NULL != szTmp)
            {
                info.SerialNumber = szTmp;
            }
            szTmp = pItem->Attribute("name");
            if (NULL != szTmp)
            {
                info.strOutName = szTmp;
            }
            szTmp = pItem->Attribute("exposure");
            if (NULL != szTmp)
            {
                info.exposure = atoi(szTmp);
            }
            szTmp = pItem->Attribute("gain");
            if (NULL != szTmp)
            {
                info.gain = atoi(szTmp);
            }
            szTmp = pItem->Attribute("trigger");
            if (NULL != szTmp)
            {
                info.TriggerMode = (CameraTriggerMode)atoi(szTmp);
            }
            szTmp = pItem->Attribute("polarity");
            if (NULL != szTmp)
            {
                info.Activation = (CameraTriggerActivation)atoi(szTmp);
            }
            m_camConfigList.insert(make_pair(info.strOutName, info));
        }
        pCameraElem = pCameraElem->NextSiblingElement("camera");
    }
    return true;
}

//写配置文件
bool CCameraManagerPlugin::SaveConfig()
{
    TiXmlDocument docConfig;
    JDir dir;
    string strPath = dir.getCurrentPath();
    strPath += CAMERA_MANAGER_PLUGIN_DIR;

    docConfig.Parse(g_szCameraXmlBuf, NULL, TIXML_ENCODING_UTF8);

    TiXmlElement *pRoot = docConfig.RootElement();
    if (pRoot != NULL)
    {
        map<string, VS_CAMERA_INFO>::iterator it = m_camConfigList.begin();
        for (; it != m_camConfigList.end(); ++it)
        {
            TiXmlElement *pCameraElem = new TiXmlElement("camera");
            if (NULL != pCameraElem)
            {
                TiXmlElement *pItemElem = new TiXmlElement("item");
                pItemElem->SetAttribute("sn", it->second.SerialNumber);
                pItemElem->SetAttribute("name", it->second.strOutName);
                pItemElem->SetAttribute("exposure", it->second.exposure);
                pItemElem->SetAttribute("gain", it->second.gain);
                pItemElem->SetAttribute("trigger", it->second.TriggerMode);
                pItemElem->SetAttribute("polarity", it->second.Activation);
                pCameraElem->LinkEndChild(pItemElem);
            }
            pRoot->LinkEndChild(pCameraElem);
        }
    }

    // 配置文件放在App同级目录，首先要获取App路径

    JDir dDir(strPath);
    if (!dDir.exists())
    {
        dDir.mkdir(strPath);
    }
    strPath.append(CAMERA_MANAGER_PLUGIN_FILE);

    return docConfig.SaveFile(strPath);
}

int CCameraManagerPlugin::GetValidCameraList( map<string, ICamera *> &mapCamera )
{
    CISAPUGuard<JMutex> locker(m_lock);
    mapCamera = m_camList;
    return m_camList.size();
}

bool CCameraManagerPlugin::CameraAcqImage(const char *szJobName, const char *szItemName, void *param/* = NULL*/)
{
    if (NULL == szJobName || NULL == szItemName)
    {
        return false;
    }

    ISCWorkFlow *pISCWorkFlow      = (ISCWorkFlow*)sdkcore::queryPriInterface("vsworkflowplugin", ISCWorkFlow::IID_SC_WORKFLOW);
    if (NULL == pISCWorkFlow)
    {
        return false;
    }
    STImageFormInfo stImageParam;
    if (NULL == param)
    {
        if (!pISCWorkFlow->GetItemParam(szJobName, szItemName, &stImageParam))
        {
            return false;
        }
    }
    else
    {
        stImageParam = *(STImageFormInfo *)param;
    }
    if (IMAGE_CAMERA != stImageParam.type)
    {
        return true;
    }
    ICamera *pCamera = NULL;
    pCamera = GetCameraInstance(stImageParam.cameraParamInfo.ID);
    if (NULL == pCamera)
    {
        return false;
    }
    VS_CAMERA_INFO inCameraParam;
    pCamera->GetCameraInfo(inCameraParam);
    inCameraParam.exposure = stImageParam.cameraParamInfo.ExposureTime;
    inCameraParam.gain     = stImageParam.cameraParamInfo.Gain;
    //设置曝光增益
    if (!pCamera->SetCameraInParam(inCameraParam))
    {
        return false;
    }
    if (!pCamera->SingleAcq(szJobName))
    {
        return false;
    }

    return true;
}

bool CCameraManagerPlugin::RefreshValidCameraList()
{
    bool bRet = true;
    ICamera *pCamera = NULL;
    vector<string> vecGuid;
    vector<string>::iterator itID;
    ICameraFactory *pCamFactory = NULL;
    m_camList.clear();
    LOG_INFO("===============Refresh Camera begin.===============");
    for (int iCount = 0; iCount < CAM_TYPE_COUNT; iCount++)
    {
        string strInfo = g_camTypeName[iCount];
        pCamFactory = (ICameraFactory *)sdkcore::queryPriInterface(g_camTypeName[iCount],ICameraFactory::IID_CAMERAFACTORY);
        if (NULL != pCamFactory)
        {
            //遍历， 创建相机实例
            vecGuid.clear();
            pCamFactory->GetCamList(vecGuid, 16); //获取相机SN列表
            for (itID = vecGuid.begin(); itID != vecGuid.end(); ++itID)
            {
                VS_CAMERA_INFO info;
                map<string, VS_CAMERA_INFO>::iterator itConfig;
                string camOutName = GetCameraNameWithGuid(*itID);
                itConfig = m_camConfigList.find(camOutName);
                if (itConfig != m_camConfigList.end())
                {
                    info = itConfig->second;
                }
                pCamera = pCamFactory->CreateCamera((*itID).c_str(), 1);
                if (NULL == pCamera)
                {
                    m_lastError = VISION_CAMERAMANAGER_EXECUTE_CREATECAMERA_ERROR;
                    bRet = false;
                }
                else
                {
                    m_camList.insert(make_pair((*itID), pCamera));
                    pCamera->SetCameraInParam(info);
                }
            }
            LOG_INFO("===============Refresh %s Camera end.===============", strInfo.c_str());
            //m_vecCameraFactory[iCount] = pCamFactory;
        }
    }
    LOG_INFO("===============Refresh Camera end.===============");
    return bRet;
}

//Demo
//bool CCameraManagerPluginPlugin::motoron( int i1,int i2 )
//{
//	return true;
//}



