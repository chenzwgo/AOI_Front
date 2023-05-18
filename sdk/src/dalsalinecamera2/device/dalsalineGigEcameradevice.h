#ifndef DALSALINE_GIGE_CAMERADEVICE_H
#define DALSALINE_GIGE_CAMERADEVICE_H

#include "sapclassbasic.h"
#include "vscameratools/camera/ilinecameradevice.h"

class ILineCameraDevice;

#include <string>
using namespace std;

class dalsalineGigecameradevice : public ILineCameraDevice
{
public:
    dalsalineGigecameradevice();

    bool init(const char *szDeviceName);
    bool openDevice(SapAcqDevice    *pAcqDevice = NULL);
	bool openDevice();
    bool closeDevice();

    bool IsOpen();

    bool setTDIFeature(bool open); 
    bool SetGain(double GainValue);
    bool SetExposureTime(double ExpTimeValue);
    bool SetCameraFrequency(unsigned int Freq);
    bool SetTriggerMode(bool triger);
    bool setCamPluginId(string strCamPluginID);
private:
    //bool readConfig();
private:
    SapAcqDevice    *m_AcqDevice;
    SapFeature      *m_pFeature;
    ILineCameraDevice *m_pOtherCamDevice;
    string           m_strDeviceName;
    string           m_strCamPluginName;
    bool             m_bIsUnknown;  //未知相机不设置相机参数
};

#endif // DALSALINECAMERADEVICE_H
