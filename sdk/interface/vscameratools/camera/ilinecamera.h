#ifndef ILINE_CAMERA_H
#define ILINE_CAMERA_H
#include <string>
using namespace std;

#define MAX_CHANNEL_COUNT    8

enum LINE_TRIGGER_TYPE
{
    LINE_TG_NONE = 0,
    LINE_TG_INTERNAL,
    LINE_TG_EXTERNAL,
    LINE_TG_SHAF_ENCODER
};

enum LINE_TRIGGER_DETECTION
{
    TG_ACTIVE_LOW = 0,
    TG_ACTIVE_HIGH,
    TG_RISING_EDGE,
    TG_FALLING_EDGE
};

struct LINE_CAM_PARAM
{
    bool                    bEable;
    bool                    bExtTrigger;
    bool                    bIsInvert;
	bool                    bIsTDI;
    int                     channel;  //
    unsigned long           width;
    unsigned long           height;     //��Ч����
    unsigned long           snapheight;     //��֡�ɼ�ʱ����
    unsigned long           realheight;     //ʵʱ��ʾʱ����
    unsigned long           exposure;   //�ع�ֵ
    unsigned long           gain;   //�ع�ֵ
    unsigned long           grabFreq;   //�ڲ��������������Ƶ ��ÿ��
    unsigned long           blockSize;   //��С���С,ÿ�δ������С����
    LINE_TRIGGER_TYPE       tgType;
    LINE_TRIGGER_DETECTION  tgDetection;
    string                  strCameraName;    //�������
    string                  strCameraID;      //���ʶ���
	string                  strCameraSerialNumber;//�����̖
    string                  strCamConfigPath;  //�������·��
    string                  strCamDes;         //�������
    string                  strCardPluginID;    //�ɼ������ID
    string                  strCamPluginID;    //������ID

    LINE_CAM_PARAM()
    {
        bEable         = true;
        bExtTrigger    = false;
        bIsInvert      = false;
		bIsTDI         = false;
        channel        = 1;
        width          = 8192;
        snapheight     = 100;
        realheight     = 100;
        height         = snapheight;
        exposure       = 20;
        gain           = 0;
        grabFreq       = 10000;
        blockSize      = 512;
        tgType         = LINE_TG_NONE;
        tgDetection    = TG_FALLING_EDGE;
        strCameraID    = "";
        strCameraName  = "";
        strCamConfigPath = "";
        strCamDes = "";
        strCardPluginID = "dalsalinecamera";
        strCamPluginID = "dalsalinecamera";
		strCameraSerialNumber = "";
    }
    LINE_CAM_PARAM(LINE_CAM_PARAM &other)
    {
        if (other.channel >= MAX_CHANNEL_COUNT)
        {
            channel = 8;
        }
        else
        {
            channel        = other.channel;
        }
        bEable         = other.bEable;
        bExtTrigger    = other.bExtTrigger;
        bIsInvert      = other.bIsInvert;
		bIsTDI         = other.bIsTDI;
        width          = other.width;
        height         = other.height;
        snapheight     = other.snapheight;
        realheight     = other.realheight;
        exposure       = other.exposure;
        gain           = other.gain;
        grabFreq       = other.grabFreq;
        blockSize      = other.blockSize;
        tgType         = other.tgType;
        tgDetection    = other.tgDetection;
        strCameraID    = other.strCameraID;
        strCameraName  = other.strCameraName;
        strCamConfigPath = other.strCamConfigPath;
        strCamDes = other.strCamDes;
        strCardPluginID = other.strCardPluginID;
        strCamPluginID = other.strCamPluginID;
		strCameraSerialNumber = other.strCameraSerialNumber;
    }
    LINE_CAM_PARAM operator=(const LINE_CAM_PARAM &other)
    {
        if (other.channel >= MAX_CHANNEL_COUNT)
        {
            channel = 8;
        }
        else
        {
            channel        = other.channel;
        }
        bEable         = other.bEable;
        bExtTrigger    = other.bExtTrigger;
        bIsInvert      = other.bIsInvert;
		bIsTDI         = other.bIsTDI;
        width          = other.width;
        height         = other.height;
        snapheight     = other.snapheight;
        realheight     = other.realheight;
        exposure       = other.exposure;
        gain           = other.gain;
        grabFreq       = other.grabFreq;
        blockSize      = other.blockSize;
        tgType         = other.tgType;
        tgDetection    = other.tgDetection;
        strCameraID    = other.strCameraID;
        strCameraName  = other.strCameraName;
        strCamConfigPath = other.strCamConfigPath;
        strCamDes = other.strCamDes;
        strCardPluginID  = other.strCardPluginID;
        strCamPluginID   = other.strCamPluginID;
		strCameraSerialNumber = other.strCameraSerialNumber;
        return *this;
    }
    bool operator==(const LINE_CAM_PARAM &other)
    {
        if (channel        == other.channel
            && bEable         == other.bEable
            && bExtTrigger    == other.bExtTrigger
            && bIsInvert      == other.bIsInvert
			&& bIsTDI         == other.bIsTDI
            && width          == other.width
            && height         == other.height
            && snapheight     == other.snapheight
            && realheight     == other.realheight
            && exposure       == other.exposure
            && gain           == other.gain
            && grabFreq       == other.grabFreq
            && blockSize      == other.blockSize
            && tgType         == other.tgType
            && tgDetection    == other.tgDetection
            && strCameraID    == other.strCameraID
            && strCameraName  == other.strCameraName
            && strCamConfigPath == other.strCamConfigPath
            && strCamDes == other.strCamDes
            && strCardPluginID == other.strCardPluginID
            && strCamPluginID == other.strCamPluginID
			&& strCameraSerialNumber == other.strCameraSerialNumber)
        {
            return true;
        }
        return false;
    }
    bool operator!=(const LINE_CAM_PARAM &other)
    {
        return !(*this == other);
    }
};

typedef void (*FnLineTriggerFunction)(void *, unsigned char *, int , int);

//namespace BJ_CAM_INFO
//{
//#define STATUS_UNINIT                0x01       //δ��ʼ��
//#define STATUS_CAM_OPEN_FAILED       0x02       //�����ʧ��
//#define STATUS_SET_PARAM_FAILED      0x04       //���ò���ʧ��
//#define STATUS_GRAB_FAILED           0x08       //ͼ��ɼ�ʧ��
//#define STATUS_CAM_NOT_FOUND         0x10       //û�ҵ����
//#define STATUS_CAM_OFF_LINE          0x80       //�������
//
//#define STATUS_CARD_UNINIT           0x0100       //δ��ʼ��
//#define STATUS_CARD_OPEN_FAILED      0x0200       //�򿪿��ƿ�ʧ��
//}

class ILineCamera
{
public:
    enum X_LINE_CAM_STATUS
    {
        STATUS_UNINIT = 0,             //δ��ʼ��
        STATUS_CAM_OPEN_FAILED,        //�����ʧ��
        //STATUS_SET_PARAM_FAILED,       //���ò���ʧ��
        STATUS_GRAB_FAILED,            //ͼ��ɼ�ʧ��
        STATUS_CAM_NOT_FOUND,          //û�ҵ����
        STATUS_CAM_OFF_LINE,           //�������
        //����״̬
        STATUS_CAM_IDLE,               //�������
        STATUS_CAM_READ_ABORT,         //�����ͼ�ж�
        STATUS_CAM_READING,            //�����ͼ��
        STATUS_CAM_READ_END,           //�����ͼ���
    };
    enum X_ERROR_CODE
    {
        ERR_UNKNOW = -1,      //δ֪�쳣
        ERR_OK = 0,
        ERR_OPEN_FAILED,      //�����ʧ��
        ERR_ACQ_FAILED,       //��ͼʧ��
        ERR_GRAP_RUNNING,     //��ͼδ���
        ERR_SET_PARAM_FAILED,       //��������ʧ��
        ERR_GET_SIZE_FAILED,       //��ȡͼ���С�쳣
        ERR_ABORT_FAILED,       //��ֹ�쳣
        ERR_GRAB_FAILED,       //�����ɼ�ʧ��
        ERR_PARAM_INVALID,    //����쳣
    };

	enum
	{
		IID_CAMERA = 0x100
	};
    virtual ~ILineCamera(){}
    virtual bool InitCam(unsigned int ServerIndex, unsigned int ResourceIndex, const char *szDeviceName, const LINE_CAM_PARAM *param) = 0;
    virtual bool SingleAcq(bool bSoft = false) = 0;  //���ⴥ��ģʽ�£� ���ô������� �ⴥ��ģʽ�£� ����bSoftΪtrue, ģ���ⴥ��
    virtual bool StartAcq() = 0;
    //virtual bool GetImage(void *pR, void *pG, void *pB, const char *szJobName = 0, bool bSetParam=true, bool acq = true) = 0;
    //ichannel Ϊͨ��������  index=-1ʱ��ȡ����ͼ�񣬷���Ϊ����index�е��׵�ַ, iLineNumbΪҪȡ����������-����Ҫ��֤����Ѿ����������ͼƬ
    virtual const unsigned char *GetImage() = 0; 
    //virtual const unsigned char *GetImageBufferAdr(int ichannel) = 0;
	virtual bool GetImageSize(int &w, int &h) = 0;
    virtual bool AbortAcq() = 0;
    virtual void Reset() = 0;
    virtual bool IsImplemented() { return false; };   //�Ƿ��ǲ�ɫ���
    virtual bool Continuations(bool bOpen, int iSize = 1) = 0;
    virtual bool IsContinuations(){return false;}          //����ʵʱ��ͼ

	virtual bool Open() = 0;
    virtual bool Close() = 0;
    virtual bool IsOpen() = 0;
    virtual void GetCameraInParam(LINE_CAM_PARAM &info) {info = m_CameraParam;}  //��ȡ�������
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param)  = 0;//�����������

    virtual void ClearBuf() {};
    //�Ƿ�ȡ�����
    virtual bool IsSnapFinished() = 0;
    //�ȴ�ȡ�����
    virtual bool WaitGetImageFinished(int iTimeOut = -1) = 0;
    //����ȡ�����״̬
    virtual bool ResetFinishState() = 0;

    virtual int GetLastError(char *errInfo = NULL, int *iSize = NULL) = 0;
    virtual int GetCamStatus() = 0;
    virtual void SetTriggerFunction(FnLineTriggerFunction fnTriggerFunction, void *pInParam) = 0;
    virtual bool IsInitFinished() = 0;
protected:
    LINE_CAM_PARAM   m_CameraParam;
};

#endif // ICAMERA_H