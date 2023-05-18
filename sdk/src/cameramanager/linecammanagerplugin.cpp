#include "linecammanagerplugin.h"
#include "sdkcore.h"
#include "vision_error.h"
#include "cameramanagererror.h"
#include "vscameratools/camera/ilinecamera.h"
#include "tools/hcameraparam.h"
#include "jdir.h"
#include "tinyxml.h"
#include "vsworkflowplugin/iscworkflow.h"
#include "proxymanager.h"
#include "trace.h"
#include "rtdefine.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "g12common.h"
#include <string>
using namespace std;

const char *g_linecamTypeDesc[] = {
    "Dalsa",
    "Ikap"
    "Other"
};

const char *g_linecamTypeName[] = {
    "dalsalinecamera",
    "IKaplinecamera",
    "othercamera"
};

static const char *g_szCameraXmlBuf = {
    "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\
    <vstudiodoc version=\"2.0\">\
    </vstudiodoc>"};

#define CAMERA_MANAGER_PLUGIN_DIR     "/plugins/cameraplugin"
#define CAMERA_MANAGER_PLUGIN_FILE    "/linecameraconfig.xml"

CLineCamManagerPlugin *CLineCamManagerPlugin::m_pInstance = NULL;

CLineCamManagerPlugin::CLineCamManagerPlugin(void) : m_bIsInit(false)
    , m_pThreadInit(NULL)
{
    m_lock.Init();
    memset(m_pCamFactory, 0, sizeof(m_pCamFactory));
}


CLineCamManagerPlugin::~CLineCamManagerPlugin(void)
{
    UnInit();
}

void CLineCamManagerPlugin::ReInit()
{
    Init();
}

void CLineCamManagerPlugin::Init()
{
    CISAPUGuard<JMutex> locker(m_lock);
    LOG_INFO("===============[Cam manager]CamManager init begin===============");
    UnInit();
    ReadConfig();
	//CLineCamManagerPlugin::ThreadInitCam(this);
    if (NULL == m_pThreadInit)
    {
        m_pThreadInit = new JThreadTP<CLineCamManagerPlugin, void*, void>;
    }
    m_bIsRunning = true;
    for (int iCount = 0; iCount < ILineCameraFactoryManage::LINE_CAM_TYPE_COUNT; iCount++)
    {
        m_pCamFactory[iCount] = (ILineCameraFactory *)sdkcore::queryPriInterface(g_linecamTypeName[iCount], ILineCameraFactory::IID_LINE_CAMERAFACTORY);
    }
    m_pThreadInit->StartThread(this, &CLineCamManagerPlugin::ThreadInitCam, this);
}

void CLineCamManagerPlugin::UnInit()
{
    //m_vecCameraFactory.resize(CAM_TYPE_COUNT);
    //for (int iIndex = 0; iIndex < CAM_TYPE_COUNT; iIndex++)
    //{
    //    m_vecCameraFactory[iIndex] = NULL;
    //}

    m_bIsRunning = false;
    while (NULL != m_pThreadInit && m_pThreadInit->IsRunning())
    {
        Sleep(1000);
    }
    CISAPUGuard<JMutex> locker(m_lock);
    m_camList.clear();
    m_camConfigList.clear();
}

CLineCamManagerPlugin *CLineCamManagerPlugin::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CLineCamManagerPlugin();
    }

    return m_pInstance;
}

void CLineCamManagerPlugin::ReleaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void CLineCamManagerPlugin::EnumCameraDesc(vector<string> &vecCamDesc)
{
    RETURN_IF(!IsInitFinished());
    CISAPUGuard<JMutex> locker(m_lock);
    vecCamDesc.clear();
    for (int iIndex = 0; iIndex < ILineCameraFactoryManage::LINE_CAM_TYPE_COUNT; iIndex++)
    {
        vecCamDesc.push_back(g_linecamTypeDesc[iIndex]);
    }
}

string CLineCamManagerPlugin::GetCamDesc(int iCamType)
{
    RETURN_V_IF(!IsInitFinished(), "");
    CISAPUGuard<JMutex> locker(m_lock);
    if (iCamType > ILineCameraFactoryManage::UNKNOWN_CAM && iCamType < ILineCameraFactoryManage::LINE_CAM_TYPE_COUNT)
    {
        return g_linecamTypeDesc[iCamType];
    }

    return "";
}

int CLineCamManagerPlugin::GetCamType(const string &strCamDesc)
{
    RETURN_V_IF(!IsInitFinished(), UNKNOWN_CAM);
    CISAPUGuard<JMutex> locker(m_lock);
    for (int iIndex = 0; iIndex < ILineCameraFactoryManage::LINE_CAM_TYPE_COUNT; iIndex++)
    {
        if (strCamDesc.compare(g_linecamTypeDesc[iIndex]) == 0)
        {
            return iIndex;
        }
    }

    return UNKNOWN_CAM;
}

ILineCamera * CLineCamManagerPlugin::GetCameraInstance( string strCamName, int iCamType /*= UNKNOWN_CAM*/ )
{
    RETURN_V_IF(!IsInitFinished(), NULL);
    CISAPUGuard<JMutex> locker(m_lock);
    ILineCamera *pCamera = NULL;
    do 
    {
        if (strCamName == "")
        {
            m_lastError = VISION_CAMERAMANAGER_EXECUTE_CAMID_ERROR;
            break;
        }
        map<string, LINE_CAM_PARAM>::iterator itInfo = m_camConfigList.find(strCamName);
        if (itInfo == m_camConfigList.end())
        {
            m_lastError = VISION_CAMERAMANAGER_EXECUTE_CAMID_ERROR;
            break;
        }
        map<string, ILineCamera *>::iterator it;
        it = m_camList.find(itInfo->second.strCameraID);
        if (it != m_camList.end() && NULL != it->second )
        {
            pCamera = it->second;
            m_lastError = VISION_SUCCESS;
        }
        else
        {
			map<string, ILineCamera *>::iterator it2= m_camList.begin();
			while (it2 != m_camList.end())
			{
				LINE_CAM_PARAM info2;
				if (it2->second != NULL)
				{
					it2->second->GetCameraInParam(info2);
					if (info2.strCameraSerialNumber == itInfo->second.strCameraSerialNumber)
					{
						pCamera = it2->second;
						m_lastError = VISION_SUCCESS;
						break;
					}
				}

				it2++;
			}
        }
		if (pCamera == NULL)
		{
			m_lastError = VISION_CAMERAMANAGER_EXECUTE_CREATECAMERA_ERROR;
		}		
    } while (0);

    return pCamera;
}

string CLineCamManagerPlugin::GetCameraNameWithGuid( string ID )
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
        map<string, LINE_CAM_PARAM>::iterator it;
        for (it = m_camConfigList.begin(); it != m_camConfigList.end(); ++it)
        {
            //获取相机别名， 如果与传入匹配则返回
            string strID = it->second.strCameraID;  
            if (0 == strID.compare(ID) && it->second.bEable)
            {
                strName = it->first;
                break;
            }
        }
    } while (0);

    return strName;
}

int CLineCamManagerPlugin::GetCameraNameList( vector<string> &vecName, int iCamType /*= UNKNOWN_CAM*/ )
{
    CISAPUGuard<JMutex> locker(m_lock);
    map<string, LINE_CAM_PARAM>::iterator it = m_camConfigList.begin();
    for (; it != m_camConfigList.end(); ++it)
    {
        vecName.push_back(it->first);
    }
    return vecName.size();
}

bool CLineCamManagerPlugin::GetCameraInParam( string strName, LINE_CAM_PARAM &info )
{
    //RETURN_V_IF(!IsInitFinished(), false);
    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;
    map<string, LINE_CAM_PARAM>::iterator it = m_camConfigList.find(strName);
    if (it != m_camConfigList.end())
    {
        info = it->second;
        bRet = true;
    }
    return bRet;
}

bool CLineCamManagerPlugin::SetCameraInParam( LINE_CAM_PARAM &param )
{
    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;
    map<string, LINE_CAM_PARAM>::iterator it = m_camConfigList.find(param.strCameraName);
    if (it == m_camConfigList.end())
    {
        m_camConfigList.insert(std::pair< const string,LINE_CAM_PARAM >(param.strCameraName, param));
        it = m_camConfigList.find(param.strCameraName);
    }
    //如果找到，且内容不相同， 则设置
    map<string, ILineCamera *>::iterator itCam = m_camList.find(param.strCameraID);
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
    else
    {
        it->second = param;
        SaveConfig();
        LOG_WARN("相机无效，请检查是否有打开相机");
        bRet = true;
    }
    //同步保存， 供外部其他插件获取
    IFileVariable *pFile = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME);// getCameraSetCfgFileVariable();
    if (NULL != pFile)
    {
        pFile->setValue(CAM_ATTRIBUTE_WIDTH, param.width, false);
        pFile->setValue(CAM_ATTRIBUTE_HEIGHT, param.snapheight, false);
        pFile->setValue(CAM_ATTRIBUTE_BLOCKSIZE, param.blockSize, false);
        pFile->setValue(CAM_ATTRIBUTE_EXPOSURE, param.exposure, false);
        pFile->setValue(CAM_ATTRIBUTE_GAIN, param.gain, false);
    }
    //if (it != m_camConfigList.end() && param != it->second)
    //{
    //}
    return bRet;
}

bool CLineCamManagerPlugin::CreateCameraConfig( string strName, LINE_CAM_PARAM &info )
{
    RETURN_V_IF(!IsInitFinished(), false);
    CISAPUGuard<JMutex> locker(m_lock);
    m_camConfigList.insert(std::pair<const string,LINE_CAM_PARAM>(strName, info));
    map<string, ILineCamera *>::iterator it = m_camList.find(info.strCameraID);
    if (it != m_camList.end())
    {
        it->second->SetCameraInParam(info);
    }
    SaveConfig();
    return true;
}

bool CLineCamManagerPlugin::DeleteCameraConfig( string strName )
{
    RETURN_V_IF(!IsInitFinished(), false);
    CISAPUGuard<JMutex> locker(m_lock);
    map<string, LINE_CAM_PARAM>::iterator it = m_camConfigList.find(strName);
    if (it != m_camConfigList.end())
    {
        m_camConfigList.erase(it);
        SaveConfig();
        return true;
    }
    return false;
}

bool CLineCamManagerPlugin::UpdateCameraName(LINE_CAM_PARAM &info, string strOldName )
{
    RETURN_V_IF(!IsInitFinished(), false);
    CISAPUGuard<JMutex> locker(m_lock);
    bool bRet = false;
    do 
    {
        //查找相机名称是否有效
        map<string, LINE_CAM_PARAM>::iterator it = m_camConfigList.find(strOldName);
        if (it == m_camConfigList.end())
        {
            break;
        }

        //更新原有属性
        LINE_CAM_PARAM cameraInfo = it->second;
        m_camConfigList.erase(it);
        cameraInfo.strCameraName = info.strCameraName;
        m_camConfigList.insert(pair<const string,LINE_CAM_PARAM>(info.strCameraName, cameraInfo));

        //更新相机内参数
        map<string, ILineCamera *>::iterator itC = m_camList.find(info.strCameraID);
        LINE_CAM_PARAM cameraCurrent;
        if (itC != m_camList.end())
        {
            itC->second->GetCameraInParam(cameraCurrent);
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

bool CLineCamManagerPlugin::ReadConfig()
{
    bool bRet = false;
    IFileVariable *pFile = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME);
    bool bIsInit = false;
    if (NULL != pFile)
    {
        string camName;
        m_camConfigList.clear();
        for (int index = 0; index < CAMERA_NUMBER; index++)
        {
            camName = getCamNameWithID(index);
            LINE_CAM_PARAM param;
            param.strCameraID = pFile->getValue(camName, "sn").toCString();
			param.strCameraSerialNumber = pFile->getValue(camName, "csn").toCString();
            param.bEable = pFile->getValue(camName, "eable", 0).toBool();
            param.channel = pFile->getValue(camName, "channel").toInt();
            param.height = pFile->getValue(camName, "height").toInt();
            param.snapheight = pFile->getValue(camName, "snapheight").toInt();
            param.width = pFile->getValue(camName, "width").toInt();
            param.realheight = pFile->getValue(camName, "realheight").toInt();
            param.blockSize = pFile->getValue(camName, "blocksize").toInt();
            param.exposure = pFile->getValue(camName, "exposure").toInt();
            param.gain = pFile->getValue(camName, "gain").toInt();
            param.grabFreq = pFile->getValue(camName, "freq").toInt();
            param.bExtTrigger = pFile->getValue(camName, "trigger").toBool();
            param.strCamConfigPath = pFile->getValue(camName, "path").toCString();
            param.bIsInvert = pFile->getValue(camName, "isinvert").toBool();
            param.strCamDes = pFile->getValue(camName, "des").toCString();
            param.strCardPluginID = pFile->getValue(camName, "dalsalinecamera").toCString();
            param.strCamPluginID = pFile->getValue(camName, "camplugin").toCString();
            //param.xoffset = pFile->getValue(camName, "xoffset").toInt();

			param.bIsTDI = pFile->getValue(camName, "tdi", 0).toBool();
            param.strCameraName = camName;
            if (!bIsInit)
            {
                bIsInit = true;
                bRet = true;
                pFile->setValue(CAM_ATTRIBUTE_WIDTH, param.width, false);
                pFile->setValue(CAM_ATTRIBUTE_HEIGHT, param.snapheight, false);
                pFile->setValue(CAM_ATTRIBUTE_BLOCKSIZE, param.blockSize, false);
                pFile->setValue(CAM_ATTRIBUTE_EXPOSURE, param.exposure, false);
                pFile->setValue(CAM_ATTRIBUTE_GAIN, param.gain, false);
            }

            m_camConfigList[camName] = param;
			//m_camConfigList.insert(make_pair(camName, param));
        }
    }
    return bRet;
}

//写配置文件
bool CLineCamManagerPlugin::SaveConfig()
{
    bool bRet = false;
    IFileVariable *pFile = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME);
    bool bIsInit = false;
    if (NULL != pFile)
    {
        map<string, LINE_CAM_PARAM>::iterator it = m_camConfigList.begin();
        for (; it != m_camConfigList.end(); ++it)
        {
            if (!bIsInit)
            {
                bIsInit = true;
                pFile->setValue(CAM_ATTRIBUTE_WIDTH, it->second.width, true);
                pFile->setValue(CAM_ATTRIBUTE_HEIGHT, it->second.snapheight, true);
                pFile->setValue(CAM_ATTRIBUTE_BLOCKSIZE, it->second.blockSize, true);
                pFile->setValue(CAM_ATTRIBUTE_EXPOSURE, it->second.exposure, true);
                pFile->setValue(CAM_ATTRIBUTE_GAIN, it->second.gain, true);
                bRet = true;
            }

            pFile->setValue(it->first, "name", it->second.strCameraName, true); 
            pFile->setValue(it->first, "eable", it->second.bEable, true);
            pFile->setValue(it->first, "des", it->second.strCamDes, true);
            pFile->setValue(it->first, "sn", it->second.strCameraID, true);
			pFile->setValue(it->first, "csn", it->second.strCameraSerialNumber, true);
            pFile->setValue(it->first, "channel", it->second.channel, true);
            pFile->setValue(it->first, "height", it->second.height, true);
            pFile->setValue(it->first, "snapheight", it->second.snapheight, true);
            pFile->setValue(it->first, "width", it->second.width, true);
            pFile->setValue(it->first, "realheight", it->second.realheight, true);
            pFile->setValue(it->first, "blocksize", it->second.blockSize, true);
            pFile->setValue(it->first, "exposure", it->second.exposure, true);
            pFile->setValue(it->first, "gain", it->second.gain, true);
            pFile->setValue(it->first, "freq", it->second.grabFreq, true);
            pFile->setValue(it->first, "trigger", it->second.bExtTrigger, true);
            pFile->setValue(it->first, "path", it->second.strCamConfigPath.c_str(), true);
            pFile->setValue(it->first, "isinvert", it->second.bIsInvert, true);
            pFile->setValue(it->first, "dalsalinecamera", it->second.strCardPluginID, true);
            pFile->setValue(it->first, "camplugin", it->second.strCamPluginID, true);
            //pFile->setValue(it->first, "xoffset", it->second.xoffset, true);
        }
    }
    return bRet;
}

void CLineCamManagerPlugin::ThreadInitCam( void* param )
{
    //CISAPUGuard<JMutex> lockerInit(m_lockInit);
    //先初始化个类型相机插件， 防止baumer相机占用其他相机的资源
    bool bFinished = true;
    do 
    {
        for (int iCount = 0; iCount < ILineCameraFactoryManage::LINE_CAM_TYPE_COUNT; iCount++)
        {
            if (NULL != m_pCamFactory[iCount] && !m_pCamFactory[iCount]->isCameraInitFinished())
            {
                bFinished = false;
                break;
            }
            else
            {
                bFinished = true;
            }
        }
        Sleep(10);
    } while (!bFinished && m_bIsRunning);
    RefreshValidCameraList();
    LOG_INFO("===============[Cam manager]CamManager init end===============");
    CISAPUGuard<JMutex> locker(m_lock);
    m_bIsInit = true;
}

int CLineCamManagerPlugin::GetValidCameraList( map<string, ILineCamera *> &mapCamera )
{
    RETURN_V_IF(!IsInitFinished(), 0);
    CISAPUGuard<JMutex> locker(m_lock);
    mapCamera = m_camList;
    return m_camList.size();
}

bool CLineCamManagerPlugin::CameraAcqImage(const char *szJobName, const char *szItemName, void *param/* = NULL*/)
{
    RETURN_V_IF(!IsInitFinished(), false);
    if (NULL == szJobName || NULL == szItemName)
    {
        return false;
    }

    ISCWorkFlow *pISCWorkFlow      = (ISCWorkFlow*)sdkcore::queryPriInterface("vsworkflowplugin", ISCWorkFlow::IID_SC_WORKFLOW);
    if (NULL == pISCWorkFlow)
    {
        return false;
    }

    ILineCamera *pCamera = NULL;
    pCamera = GetCameraInstance(szJobName);
    if (NULL == pCamera)
    {
        return false;
    }
    //LINE_CAM_PARAM inCameraParam;
    //pCamera->GetCameraInfo(inCameraParam);
    //inCameraParam.exposure = stImageParam.cameraParamInfo.ExposureTime;
    //inCameraParam.gain     = stImageParam.cameraParamInfo.Gain;
    ////设置曝光增益
    //if (!pCamera->SetCameraInParam(inCameraParam))
    //{
    //    return false;
    //}
    if (!pCamera->SingleAcq())
    {
        return false;
    }

    return true;
}

bool CLineCamManagerPlugin::RefreshValidCameraList()
{
    bool bRet = true;
    ILineCamera *pCamera = NULL;
    vector<string> vecGuid;
    vector<string>::iterator itID;
    //ILineCameraFactory *pCamFactory = NULL;
    LOG_INFO("===============[Cam manager]refresh Camera begin.===============");
    for (int iCount = 0; iCount < ILineCameraFactoryManage::LINE_CAM_TYPE_COUNT; iCount++)
    {
        RETURN_V_IF(!m_bIsRunning, false);
        string strInfo = g_linecamTypeName[iCount];
        if (NULL != m_pCamFactory[iCount])
        {
            //遍历， 创建相机实例
            vecGuid.clear();
            m_pCamFactory[iCount]->GetCamList(vecGuid, 16); //获取相机SN列表
            for (itID = vecGuid.begin(); itID != vecGuid.end(); ++itID)
            {
                RETURN_V_IF(!m_bIsRunning, false);
                LINE_CAM_PARAM info;
				info.strCameraID = *itID;
                string camOutName = GetCameraNameWithGuid(info.strCameraID);
				if (camOutName.length() == 0)
				{
					continue;
				}
				GetCameraInParam(camOutName, info);
                map<string, LINE_CAM_PARAM>::iterator itConfig;
                itConfig = m_camConfigList.find(camOutName);
                if (itConfig != m_camConfigList.end())
                {
                    info = itConfig->second;
                }
                map<string, ILineCamera *>::iterator itCam = m_camList.find(info.strCameraID);
                if (itCam != m_camList.end())
                {
                    pCamera = itCam->second;
                    if (NULL != pCamera && itConfig != m_camConfigList.end())
                    {
                        if (!pCamera->SetCameraInParam(info))
                        {
                            bRet = false;
                        }
                    }
                }
                else
                {
                    pCamera = m_pCamFactory[iCount]->CreateCamera(&info);
                    m_camList.insert(make_pair(info.strCameraID, pCamera));
                }
                if (NULL == pCamera)
                {
                    m_lastError = VISION_CAMERAMANAGER_EXECUTE_CREATECAMERA_ERROR;
                    bRet = false;
                }
            }
            LOG_INFO("===============Refresh %s Camera end.===============", strInfo.c_str());
            //m_vecCameraFactory[iCount] = pCamFactory;
        }
    }
    //bool bRunning = false;
    //do
    //{
    //    map<string, ILineCamera *>::iterator it;
    //    for (it = m_camList.begin(); it != m_camList.end(); ++it)
    //    {
    //        if (NULL != it->second)
    //        {
    //            if (!it->second->IsInitFinished())
    //            {
    //                bRunning = true;
    //                break;
    //            }
    //        }
    //        bRunning = false;
    //    }
    //} while (bRunning && m_bIsRunning);
    LOG_INFO("===============[Cam manager]refresh camera end.===============");
    return bRet;
}

bool CLineCamManagerPlugin::IsInitFinished()
{
    CISAPUGuard<JMutex> locker(m_lock);
    return m_bIsInit;
}

//bool CLineCamManagerPlugin::getSequenceImageBuffer(stImageInfo *img, int chid, int index /*= 0*/)
//{
//	if (NULL == img || chid > MAX_CHANNEL_COUNT || chid < 0 || index < 0)
//    {
//        return false;
//    }
//
//    int width = 0;
//    int height = 0;
//    int iSize = 0;
//    unsigned char *pImageBuffer = NULL;
//
//    try
//    {
//        ILineCamera *pCurrentCamera = getSequenceImageInstance(chid, index); 
//        RETURN_V_IF(NULL == pCurrentCamera, false);
//        RETURN_V_IF(!pCurrentCamera->GetImageSize(width, height), false);
//
//        iSize = width * height;
//		int ic = getSequenceInCamIndex(chid);
//
//        pImageBuffer = (unsigned char *)pCurrentCamera->GetImage(ic, -1);
//        RETURN_V_IF(NULL == pImageBuffer, false);
//
//        if (NULL != img->pixelR && (width != img->width || height != img->height))
//        {
//            delete img->pixelR;
//            img->pixelR = NULL;
//        }
//        img->width = width;
//        img->height = height;
//        if (img->pixelR == NULL)
//        {
//            img->pixelR = new unsigned char[iSize];
//        }
//        memcpy(img->pixelR, pImageBuffer, sizeof(unsigned char) * iSize);
//        //if (pCurrentCamera->IsImplemented())
//        //{
//        //    if (NULL == img->pixelG)
//        //    {
//        //        img->pixelG = new unsigned char[iSize];
//        //    }
//        //    if (NULL == img->pixelB)
//        //    {
//        //        img->pixelB = new unsigned char[iSize];
//        //    }
//        //}
//    }
//    catch (...)
//    {
//        return false;
//    }
//    return true;
//}
//
//bool CLineCamManagerPlugin::getSequenceImageBuffer(stImageInfo *img, string chName, int index /*= 0*/)
//{
//    if (NULL == img)
//    {
//        return false;
//    }
//    int chid = 0;
//    chid = getSeqIndexWithName(chName);
//    RETURN_V_IF(!getSequenceImageBuffer(img, chid), false);
//    return true;
//}
//
//ILineCamera * CLineCamManagerPlugin::getSequenceImageInstance(int chid, int index /*= 0*/)
//{
//    if (chid >= SEQUE_NUMBER || index >= CAMERA_NUMBER)
//    {
//        return false;
//    }
//
//    int width = 0;
//    int height = 0;
//    int iSize = 0;
//    unsigned char *pImageBuffer = NULL;
//    string camList;
//    string camName;
//    ILineCamera *pCurrentCamera = NULL;
//    
//    camList = getCurrentProgramCamNameList();
//    size_t posBegin = 0;
//    size_t pos = 0;
//    int    camCount = 0;
//
//    iSize = count(camList.begin(), camList.end(), ',') + 1;
//    for (int i = 0; i < iSize && i < CAMERA_NUMBER; i++)
//    {
//        pos = camList.find(',', posBegin);
//        int iType;
//        int iCamIndex = 0;
//        getSqueLightTypeAndCameraIndex(getSeqNameWithID(chid), iType, iCamIndex);
//        if (iCamIndex-1 == i)
//        {
//            if (index >= camCount)
//            {
//                if (pos == string::npos)
//                {
//                    pos = camList.size();
//                }
//                camName = camList.substr(posBegin, pos - posBegin);
//                break;
//            }
//            camCount++;
//        }
//        posBegin = pos + 1;
//    }
//    pCurrentCamera = GetCameraInstance(camName);
//    return pCurrentCamera;
//}
//
//int CLineCamManagerPlugin::getSequenceInCamIndex(int index)
//{
//	ILineCamera *pCurrentCamera = NULL; 
//	int iCamOn[CAMERA_NUMBER] = { 0 };
//
//	int iCamIndex = 0; //相机索引
//	int iChIndex = -1;  //图像在当前相机中的索引
//	LINE_CAM_PARAM info;
//	pCurrentCamera = getSequenceImageInstance(index);
//	if (NULL == pCurrentCamera)
//	{
//		return 0;
//	}
//	pCurrentCamera->GetCameraInParam(info);
//	iCamIndex = getCamIndexWithName(info.strCameraName);
//	for (int i = 0; i <= index; i++)
//    {
//        int iType;
//        int iCamEableIndex = 0;
//        getSqueLightTypeAndCameraIndex(getSeqNameWithID(i), iType, iCamEableIndex);
//        if (iCamEableIndex > 0 && iCamIndex == iCamEableIndex-1)
//		{
//			iChIndex++;
//		}
//	}
//	return iChIndex;
//}

void CLineCamManagerPlugin::SaveCameraInParam()
{
    SaveConfig();
}

int CLineCamManagerPlugin::GetCamStatus(const char *szCamName/* = NULL*/)
{
    int iRet = ILineCamera::STATUS_UNINIT;//未初始化
    RETURN_V_IF(!m_bIsInit, iRet);
    if (NULL != szCamName)
    {
        if (m_camConfigList.end() != m_camConfigList.find(szCamName))
        {
            ILineCamera *pCamInstance = GetCameraInstance(szCamName);
            if (NULL != pCamInstance) //找不到有效相机， 则返回未初始化
            {
                iRet = pCamInstance->GetCamStatus();
            }
        }
    }
    else
    {
        map<string, LINE_CAM_PARAM>::iterator itCam = m_camConfigList.begin();
        for (; itCam != m_camConfigList.end(); ++itCam)
        {
            if (!itCam->second.bEable)
            {
                continue;
            }
            ILineCamera *pCamInstance = GetCameraInstance(itCam->first);
            if (NULL == pCamInstance) //找不到有效相机， 则返回未初始化
            {
                iRet = ILineCamera::STATUS_UNINIT;   //未初始化
                break;
            }
            iRet = pCamInstance->GetCamStatus();
            if (iRet != 0)
            {
                break;
            }
        }
    }

    return iRet;
}

//Demo
//bool CCameraManagerPluginPlugin::motoron( int i1,int i2 )
//{
//	return true;
//}



