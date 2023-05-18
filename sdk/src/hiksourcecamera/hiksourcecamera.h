#ifndef _HIKSOURCECAMERA_PLUGIN_H_
#define _HIKSOURCECAMERA_PLUGIN_H_

#include <string>
#include <list>
#include <map>
#include "jthreadt.h"
#include "jevent.h"
#include "vscameratools/camera/icamera.h"
#include "mycamera.h"
#include "common_data.h"

using namespace std;

class CHIKSourceCameraPlugin;

//#define MAX_IMAGES_COUNT    16
//class STImageBufferArray
//{
//public:
//    STImageBufferArray()
//    {
//        for (int i = 0; i < MAX_IMAGES_COUNT; i++)
//        {
//            m_szBufArray[i] = NULL;
//            m_iLengthArray[i] = 0;
//        }
//        m_iUsed = 0;
//        m_iWriteIndex = 0;
//        m_validBuffer.clear();
//    }
//
//    ~STImageBufferArray()
//    {
//        reset();
//    }
//    void addBuffer(unsigned char *szBuf, unsigned long iLenght)
//    {
//        if (NULL == szBuf || iLenght <= 0)
//        {
//            return;
//        }
//        //判断是否需要增加缓存
//        if (m_validBuffer.size() >= m_iUsed && m_validBuffer.size() < MAX_IMAGES_COUNT)
//        {
//            //从最后一个添加的数据后开始，将原有的数据后移，防止覆盖未使用的数据
//            bool bIsMove = false;
//            for (int iC = m_iUsed; iC > m_iWriteIndex && iC > 0; iC--)
//            {
//                m_iLengthArray[iC] = m_iLengthArray[iC-1];
//                m_szBufArray[iC] = m_szBufArray[iC-1];
//                bIsMove = true;
//            }
//            if (bIsMove)
//            {
//                m_iLengthArray[m_iWriteIndex] = 0;
//                m_szBufArray[m_iWriteIndex] = NULL;
//            }
//            m_iUsed++;
//        }
//        //Buffer满了后覆盖原有的
//        if (m_iWriteIndex > m_iUsed-1)
//        {
//            m_iWriteIndex = 0;
//        }
//        //缓存大小不对时重新分配
//        if (m_iLengthArray[m_iWriteIndex] != iLenght && NULL != m_szBufArray[m_iWriteIndex])
//        {
//            delete []m_szBufArray[m_iWriteIndex];
//            m_szBufArray[m_iWriteIndex] = NULL;
//            m_iLengthArray[m_iWriteIndex] = 0;
//        }
//        //如果缓存指针为空， 则重新分配
//        if (NULL == m_szBufArray[m_iWriteIndex])
//        {
//            m_iLengthArray[m_iWriteIndex] = iLenght;
//            m_szBufArray[m_iWriteIndex] = new unsigned char[m_iLengthArray[m_iWriteIndex]];
//            memset(m_szBufArray[m_iWriteIndex], 0 , m_iLengthArray[m_iWriteIndex]);
//        }
//        //索引有效，且Buffer不为空， 则拷贝内存
//        if (m_iWriteIndex < m_iUsed && NULL != m_szBufArray[m_iWriteIndex])
//        {
//            memset(m_szBufArray[m_iWriteIndex], 0, m_iLengthArray[m_iWriteIndex]);
//            memcpy(m_szBufArray[m_iWriteIndex], szBuf, m_iLengthArray[m_iWriteIndex]);
//            if (MAX_IMAGES_COUNT == m_validBuffer.size())
//            {
//                m_validBuffer.pop_front();
//            }
//            m_validBuffer.push_back(m_szBufArray[m_iWriteIndex]);
//            m_iWriteIndex++; //添加数据成功后指针后移
//        }
//    }
//    unsigned char *getBuffer()
//    {
//        if (m_validBuffer.size() > 0)
//        {
//            unsigned char *pBuffer = m_validBuffer.front();
//            m_validBuffer.pop_front();
//            return pBuffer;
//        }
//        else
//        {
//            return NULL;
//        }
//    }
//    int getVaildDataCount() {return m_validBuffer.size();}
//    void reset()
//    {
//        for (int i = 0; i < MAX_IMAGES_COUNT; i++)
//        {
//            if (NULL != m_szBufArray[i])
//            {
//                delete []m_szBufArray[i];
//                m_szBufArray[i] = NULL;
//            }
//            m_iLengthArray[i] = 0;
//        }
//        m_iUsed = 0;
//        m_iWriteIndex = 0;
//        m_validBuffer.clear();
//    }
//    void clear()
//    {
//        m_validBuffer.clear();
//    }
//private:
//    unsigned char *m_szBufArray[MAX_IMAGES_COUNT];  //最多允许16张缓存同时存在
//    unsigned int m_iLengthArray[MAX_IMAGES_COUNT];
//    list<unsigned char *>   m_validBuffer;
//    int  m_iUsed;
//    int  m_iWriteIndex;
//    int  m_iValidSize;
//};

class CHIKSourceCameraPlugin :public ICamera
{
public:
    CHIKSourceCameraPlugin(const char *pszGUID);
    virtual ~CHIKSourceCameraPlugin(void);

    virtual bool Open();
    virtual bool Close();
    virtual bool IsOpen();
    virtual bool GetImage(void *pR, void *pG, void *pB, const char *szJobName = 0, bool bSetParam=true, bool acq = true);
    virtual bool GetImageSize(int &w, int &h);
    virtual void SetTriggerFunction(FnTriggerFunction fnTriggerFunction, void *pInParam );
    virtual int  GetImageListSize() { return m_ImageArray.getVaildDataCount(); }
    virtual void Reset();
    virtual void AddJobNameToList( const char * szJobName);
    virtual bool SingleAcq( const char *szJobName = 0); //软触发

    virtual bool Continuations(bool bOpen);  //实时采像
    virtual bool IsContinuations();          //否在实时采图
    virtual bool SetCameraInParam(VS_CAMERA_INFO &param);
    virtual int GetLastError(char *errInfo = NULL, int *iSize = NULL);
    virtual void ClearBuf();
    //是否取像完成
    virtual bool IsGetImageFinished();
    //等待取像完成
    virtual bool WaitGetImageFinished(int iTimeOut = -1);
    //重置取像完成状态
    virtual bool ResetFinishState();
public:
    bool PushBackImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo);
    bool SetMCDeviveInfo(MV_CC_DEVICE_INFO* pDeviceInfo);
    /*相机抓图回调*/
    static void   __stdcall ImageCallBack(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
    /*相机异常回掉*/
    static void   __stdcall ExceptionCallBack(unsigned int nMsgType, void* pUser);
private:
    virtual bool SetGain(double GainValue);
    virtual bool SetExposureTime(double ExpTimeValue);
    virtual bool SetTriggerMode(CameraTriggerMode iMode);
    bool SetTriggerPolarity(CameraTriggerActivation iMode);
    virtual CameraTriggerMode GetTriggerMode() { return (CameraTriggerMode)m_CameraInfo.TriggerMode;}

    int RotationAngle(long Angle);
private:
    int  m_errorCode;
    JMutex m_lock;

    JMutex m_imageLock;
    JEvent m_ReadImageEvent;
    bool                             m_bIsRunning;
    std::list<void*> m_Images;
    STImageBufferArray  m_ImageArray;
    FnTriggerFunction m_fnTriggerFunction;
    void             *m_pInParam;
	

    bool             m_bIsColorFilter;  //是否为彩色
    //friend void BGAPI2CALL BufferHandler( void * callBackOwner, Buffer * pBufferFilled );
    bool m_bIsContinuations;
    MV_CC_DEVICE_INFO* m_pDeviceInfo;
    CMyCamera* m_pHikCamera;

    MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      // 保存图像格式
    unsigned char*  m_pBufForSaveImage;         // 用于保存图像的缓存
    unsigned int    m_nBufSizeForSaveImage;
	bool                m_bOpen;
	stImageInfo   m_pImageInfo;
};


#endif _HIKSOURCECAMERA_PLUGIN_H_

