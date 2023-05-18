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
    virtual bool StartAcq();   //��ʼͼ��ɼ�
    //virtual bool GetImage(void *pR, void *pG, void *pB, const char *szJobName = 0, bool bSetParam=true, bool acq = true);
    virtual const unsigned char *GetImage();
    //virtual const unsigned char *GetImageBufferAdr(int ichannel);
    virtual bool GetImageSize(int &w, int &h);
    virtual bool AbortAcq();              //��ֹ�ɼ�
    virtual void Reset();
    virtual bool IsImplemented();   //�Ƿ��ǲ�ɫ���
    virtual bool Continuations(bool bOpen, int iSize = 1);
    virtual bool IsContinuations();          //����ʵʱ��ͼ

    virtual bool Open();
    virtual bool Close();
    virtual bool IsOpen();
    virtual void GetCameraInParam(LINE_CAM_PARAM &info) {info = m_CameraParam;}  //��ȡ�������
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param);//�����������

    virtual void ClearBuf();
    //�Ƿ�ȡ�����
    virtual bool IsSnapFinished();
    //�ȴ�ȡ�����
    virtual bool WaitGetImageFinished(int iTimeOut = -1);
    //����ȡ�����״̬
    virtual bool ResetFinishState();

    virtual int GetLastError(char *errInfo = NULL, int *iSize = NULL);
    virtual int GetCamStatus();

    virtual void SetTriggerFunction(FnLineTriggerFunction fnTriggerFunction, void *pInParam );
    //�Ƿ��ʼ�����
    virtual bool IsInitFinished();
private:
    //void handleException(const GenericException &e);
    bool init();
    void ProcessOffline();
    void TransformToRGB(void *pBuffer, void *pR, void *pG, void *pB);

    //static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
    /// ���߻ص�����
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
    bool             m_bIsOpen;    //�Ƿ�����
    bool             m_bIsOffLine;  //�Ƿ����
    bool             m_bIsColorFilter;  //�Ƿ�Ϊ��ɫ
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
    bool             m_bIsGrab;     //�Ƿ�Ϊʵʱ�ɼ�ģʽ
    FnLineTriggerFunction m_fnTriggerFunction;
    unsigned long    m_iCurrentWidth;
    unsigned long    m_iCurrentHeight;
    int              m_iCurrentCount; //��ǰ������ͨ����

    dalsalinecameradevice   *m_camDevice;
	uint64                   m_latestTime;
public:
    JThreadTP<CDalsaGigeLineCamera, void*, void>*    m_pThreadOpenCam;
};

#endif
