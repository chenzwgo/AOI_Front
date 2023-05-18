#ifndef I_LINE_CAMERA_DEVICE_H
#define I_LINE_CAMERA_DEVICE_H
#include <string>
using namespace std;

class ILineCameraDevice
{
public:
    enum
    {
        IID_LINE_CAMERA_DEVICE = 0x102,//
    };
    virtual ~ILineCameraDevice(){}
    virtual bool init(const char *szDeviceName) = 0;

	virtual bool openDevice() = 0;
    virtual bool closeDevice() = 0;
    virtual bool IsOpen() = 0;

    virtual bool setTDIFeature(bool open) = 0;
    virtual bool SetGain(double GainValue) = 0;
    virtual bool SetExposureTime(double ExpTimeValue) = 0;
    virtual bool SetCameraFrequency(unsigned int Freq) = 0;
    virtual bool SetTriggerMode(bool triger) = 0;
protected:
    string           m_strDeviceName;
};

#endif // ICAMERA_H