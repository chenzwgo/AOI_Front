#ifndef  _IKAP_LINE_CAMERA_MANAGER_H_
#define  _IKAP_LINE_CAMERA_MANAGER_H_

#include <map>
#include <string>
#include <vector>
#include "jthreadt.h"
#include "vscameratools/camera/icamerafactory.h"
#include "IKapBoard.h"

using namespace std;

struct cameraDeviceInfo
{
	int serverIndex;
	int resourceIndex;
	string strServeName;
    string strSerialNumber;
	cameraDeviceInfo()
	{
		serverIndex = 0;
		resourceIndex = 0;
		strServeName = "";
		strSerialNumber = "";
	}
};
struct cameraAcqInfo
{
	int serverIndex;
	int resourceIndex;
    string strCameraName;
	string strDeviceName;
	ILineCamera *pCam;
	cameraAcqInfo()
	{
		serverIndex = 0;
		resourceIndex = 0;
        strCameraName = "";
		strDeviceName = "";
		pCam = NULL;
    }
    cameraAcqInfo(cameraAcqInfo &other)
    {
        serverIndex   = other.serverIndex;
        resourceIndex = other.resourceIndex;
        strCameraName = other.strCameraName;
        strDeviceName  = other.strDeviceName;
        pCam          = other.pCam;
    }
    cameraAcqInfo &operator=(cameraAcqInfo &other)
    {
        serverIndex   = other.serverIndex;
        resourceIndex = other.resourceIndex;
        strCameraName = other.strCameraName;
        strDeviceName  = other.strDeviceName;
        pCam          = other.pCam;
        return *this;
    }
    bool operator==(cameraAcqInfo &other)
    {
        if (serverIndex   == other.serverIndex
        && resourceIndex == other.resourceIndex
        && strCameraName == other.strCameraName
        && strDeviceName  == other.strDeviceName
        && pCam          == other.pCam)
        {
            return true;
        }
        return false;
    }
    bool operator!=(cameraAcqInfo &other)
    {
        return !(*this == other);
    }
};

class CIKAPCameraManager : public ILineCameraFactory
{
    typedef std::map<std::string, ICamera*> CameraList;
    typedef std::map<std::string, ICamera*>::iterator CameraIterator;
public:
    CIKAPCameraManager(void);
    ~CIKAPCameraManager(void);

public:
    static CIKAPCameraManager *getInstance();
    static void releaseInstance();

    bool init();
    bool deinit();
	bool FindCamera();

    virtual bool GetCamList(std::vector<std::string> &vecGUID,unsigned long MaxCnt);
    virtual ILineCamera *CreateCamera(LINE_CAM_PARAM *param);

    virtual bool isCameraInitFinished();
private:
	bool GetCameraParam(const char *szSerialNumb, string &configFile);


    void ThreadInitCam(void* param);
    void ThreadGetCam(void* param);
private:
    static CIKAPCameraManager *m_pInstance;

    JThreadTP<CIKAPCameraManager, void*, void>*    m_pThreadInit;

    CameraList m_cameraList;
    JMutex m_lock;

    int m_errorCode;
    bool m_bIsStartUp;
	
	vector<string> m_vecServerName;
	map<string, cameraDeviceInfo *> m_camDeviceList;
	map<string, cameraAcqInfo *> m_camAcqList;
    std::vector<std::string>  m_vecCamList;
    bool m_bIsInit;
    bool m_bIsRunning;
    JThreadTP<CIKAPCameraManager, void*, void>*    m_pThreadGetCamServerName[8];
    map<string, string> mapDeviceName;
	bool m_bCallInit;
};

#endif
