#ifndef _HIKSOURCECAMERA_MANAGER_PLUGIN_H_
#define _HIKSOURCECAMERA_MANAGER_PLUGIN_H_

#include "jmutex.h"
#include "vscameratools/camera/icamerafactory.h"
#include "hiksourcecamera.h"

#include <map>
#include <string>
using namespace std;

class CHIKSourceCameraManagerPlugin : public ICameraFactory
{
    typedef std::map<std::string, ICamera*> CameraList;
    typedef std::map<std::string, ICamera*>::iterator CameraIterator;
public:
    static CHIKSourceCameraManagerPlugin *getInstance();
    static void releaseInstance();

    bool init();
    bool deinit();

    virtual bool GetCamList(std::vector<std::string> &vecGUID,unsigned long MaxCnt);
    virtual ICamera *CreateCamera(const char *pszGUID, unsigned int index);

private:
    CHIKSourceCameraManagerPlugin(void);
    virtual ~CHIKSourceCameraManagerPlugin(void);
    static CHIKSourceCameraManagerPlugin *m_pInstance;

    CameraList m_cameraList;
    MV_CC_DEVICE_INFO_LIST m_mvDevList;
    JMutex m_lock;

    int m_errorCode;
};


#endif _HIKSOURCECAMERA_MANAGER_PLUGIN_H_

