#ifndef  _DH_MER_CAMERA_H_
#define  _DH_MER_CAMERA_H_

#include <string>
#include <list>
#include <map>
#include "jevent.h"
#include "vscameratools/camera/ilinecamera.h"
#include "common_data.h"
#include "sapclassbasic.h"
#include "jthreadt.h"
#include "jtime.h"
using namespace std;

class dalsalinecameradevice;

struct stImageBuffer
{
    int iStartLine;
    int width;
    int height;
    BYTE *pBuffer;
    stImageBuffer()
    {
        iStartLine = 0;
        width = 0;
        height = 0;
        pBuffer = NULL;
    }

    stImageBuffer &operator=(stImageBuffer &other)
    {
        iStartLine = other.iStartLine;
        width = other.width;
        height = other.height;
        pBuffer = other.pBuffer;
        return *this;
    }
};

class CDalsaGigeLineCamera : public ILineCamera
{
public:
    CDalsaGigeLineCamera(const char *pszGUID);
    ~CDalsaGigeLineCamera(void);
    virtual bool InitCam(unsigned int ServerIndex, unsigned int ResourceIndex, const char *szDeviceName, const  LINE_CAM_PARAM *param);
    virtual bool SingleAcq(bool bSoft = false);
    virtual bool StartAcq();   //开始图像采集
    //virtual bool GetImage(void *pR, void *pG, void *pB, const char *szJobName = 0, bool bSetParam=true, bool acq = true);
    virtual const unsigned char *GetImage();
    //virtual const unsigned char *GetImageBufferAdr(int ichannel);
    virtual bool GetImageSize(int &w, int &h);
    virtual bool AbortAcq();              //终止采集
    virtual void Reset();
    virtual bool IsImplemented();   //是否是彩色相机
    virtual bool Continuations(bool bOpen, int iSize = 1);
    virtual bool IsContinuations();          //否在实时采图

    virtual bool Open();
    virtual bool Close();
    virtual bool IsOpen();
    virtual void GetCameraInParam(LINE_CAM_PARAM &info) {info = m_CameraParam;}  //读取相机参数
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param);//设置相机参数

    virtual void ClearBuf();
    //是否取像完成
    virtual bool IsSnapFinished();
    //等待取像完成
    virtual bool WaitGetImageFinished(int iTimeOut = -1);
    //重置取像完成状态
    virtual bool ResetFinishState();

    virtual int GetLastError(char *errInfo = NULL, int *iSize = NULL);
    virtual int GetCamStatus();

    virtual void SetTriggerFunction(FnLineTriggerFunction fnTriggerFunction, void *pInParam );
    //是否初始化完成
    virtual bool IsInitFinished();
private:
    //void handleException(const GenericException &e);
    bool init();
    void ProcessOffline();
    void TransformToRGB(void *pBuffer, void *pR, void *pG, void *pB);

    //static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
    /// 掉线回调函数
    static void __stdcall OnDeviceOfflineCallbackFun(void* pUserParam);
    virtual bool SetGain(double GainValue);
    virtual bool SetExposureTime(double ExpTimeValue);
	bool SetCameraFrequency(unsigned int Freq);
	bool SetTriggerMode(bool triger);
    bool CreateImageBuffer();
    void ClearRecvBuffer();
    bool InsertBuffer(void *pData, int width, int height = 1);
    //void SaveImage(int index);
    //void SaveImage1();
    void ThreadInitCam(void* param);
	void setTriggerHeight(int iHeight);
public:
    static void BufferCallback(SapXferCallbackInfo *);
    void setCamStatus(unsigned long status);
    //void CleanCamStatus(unsigned long status);
private:
    int  m_errorCode;
    unsigned int  m_CamStatus;
    string m_errInfo;
    JMutex m_lock;
    JMutex m_imageLock;
    JMutex m_camStatusLock;
    JEvent m_AcqImageEvent;
    JEvent m_GrabFinishedEvent;
    JEvent m_ReadImageEvent;
    void             *m_pInParam;
    bool             m_bImageArrived;
    bool             m_bIsOpen;    //是否打开相机
    bool             m_bIsOffLine;  //是否掉线
    bool             m_bIsColorFilter;  //是否为彩色
    unsigned int     m_ServerIndex;
    unsigned int     m_ResourceIndex;
    bool             m_bIsRunning;

    SapAcquisition  *m_Acq;
    SapAcqDevice    *m_AcqDevice;
    SapBuffer       *m_Buffer;
    SapTransfer     *m_Xfer;
    BYTE            *m_pImageBuf;
    BYTE            *m_pBufferChannel[MAX_CHANNEL_COUNT];
    BYTE            *m_pImageRecvBuf;
    stImageInfo      m_pImageInfo;
    //BYTE            *m_pImageRecvBufFrame[512];
    unsigned long    m_iRecvCount;
    unsigned long    m_iCurrentIndex;
    LINE_CAM_PARAM   m_CameraParam;
	string           m_strDeviceName;
    bool             m_bIsGrab;     //是否为实时采集模式
    FnLineTriggerFunction m_fnTriggerFunction;
    unsigned long    m_iCurrentWidth;
    unsigned long    m_iCurrentHeight;
    int              m_iCurrentCount; //当前开启的通道数

    dalsalinecameradevice   *m_camDevice;
	uint64                   m_latestTime;
public:
    JThreadTP<CDalsaGigeLineCamera, void*, void>*    m_pThreadOpenCam;
};

#endif
