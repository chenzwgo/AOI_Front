#ifndef ICAMERA_H
#define ICAMERA_H
#include <string>
using namespace std;

enum CameraTriggerMode {
	mode_unknow = -1, mode_off = 0, mode_line, mode_software
};

enum CameraTriggerActivation {
    rising_edge, falling_edge
};

typedef struct VS_CAMERA_INFO
{
    unsigned int            gain;           //相机增益
    unsigned int            exposure;       //相机曝光时间
    unsigned long           width;          //图像宽度
    unsigned long           height;         //图像高度
    unsigned long           acqTimeOut;     //外触发采像超时时间
    CameraTriggerMode       TriggerMode;    //触发模式 -- 软触发 or 外触发
    CameraTriggerActivation Activation;     //触发极性 -- 上升沿触发 or 下降沿
    string                  SerialNumber;   //相机全局ID， 在相机上可找到对应字符
    string                  strOutName;     //使用时定义的名称
    string                  ServerName;     //
    string                  strConfigName;  //相机配置文件
    VS_CAMERA_INFO ()
    {
        gain        = 0;       
        exposure    = 100;   
        width       = 800;      
        height      = 600;    
        acqTimeOut  = 2000;
        TriggerMode = mode_unknow;
        Activation  = rising_edge; 
        SerialNumber= "";    
        strOutName  = ""; 
        ServerName = "";
        strConfigName = "";
    }
    VS_CAMERA_INFO (const VS_CAMERA_INFO &other)
    {
        gain        = other.gain;       
        exposure    = other.exposure;   
/*        width       = other.width;      
        height      = other.height; */    
        acqTimeOut  = other.acqTimeOut;
        TriggerMode = other.TriggerMode;
        Activation  = other.Activation; 
        SerialNumber= other.SerialNumber;    
        strOutName  = other.strOutName; 
        ServerName  = other.ServerName;
        strConfigName  = other.strConfigName;
    }
    VS_CAMERA_INFO operator=(const VS_CAMERA_INFO &other)
    {
        gain        = other.gain;       
        exposure    = other.exposure;   
/*        width       = other.width;      
height      = other.height; */      
        acqTimeOut  = other.acqTimeOut; 
        TriggerMode = other.TriggerMode;
        Activation  = other.Activation; 
        SerialNumber= other.SerialNumber;    
        strOutName  = other.strOutName; 
        ServerName  = other.ServerName;
        strConfigName  = other.strConfigName;
        return *this;
    }
    void reset()
    {
        gain        = 0;       
        exposure    = 100;   
        width       = 800;      
        height      = 600;      
        acqTimeOut  = 2000;  
        TriggerMode = mode_unknow;
        Activation  = rising_edge; 
        SerialNumber= "";    
        strOutName  = ""; 
        ServerName = "";
        strConfigName  = "";
    }
    bool operator==(const VS_CAMERA_INFO &other)
    {
        if (gain           == other.gain        
            && exposure    == other.exposure  
            && acqTimeOut  == other.acqTimeOut   
            && TriggerMode == other.TriggerMode
            && Activation  == other.Activation 
            && SerialNumber== other.SerialNumber    
            && strOutName  == other.strOutName
            && ServerName  == other.ServerName
            && strConfigName  == other.strConfigName)
        {
            return true;
        }
        return false;
    }
    bool operator!=(const VS_CAMERA_INFO &other)
    {
        if ((*this == other))
        {
            return false;
        }
        return true;
    }
};

#define MAX_IMAGES_COUNT    16
class STImageBufferArray
{
public:
    STImageBufferArray()
    {
        for (int i = 0; i < MAX_IMAGES_COUNT; i++)
        {
            m_szBufArray[i] = NULL;
            m_iLengthArray[i] = 0;
        }
        m_iUsed = 0;
        m_iWriteIndex = 0;
        m_validBuffer.clear();
    }

    ~STImageBufferArray()
    {
        reset();
    }
    void addBuffer(unsigned char *szBuf, unsigned long iLenght)
    {
        if (NULL == szBuf || iLenght <= 0)
        {
            return;
        }
        //判断是否需要增加缓存
        if (m_validBuffer.size() >= m_iUsed && m_validBuffer.size() < MAX_IMAGES_COUNT)
        {
            //从最后一个添加的数据后开始，将原有的数据后移，防止覆盖未使用的数据
            bool bIsMove = false;
            for (int iC = m_iUsed; iC > m_iWriteIndex && iC > 0; iC--)
            {
                m_iLengthArray[iC] = m_iLengthArray[iC-1];
                m_szBufArray[iC] = m_szBufArray[iC-1];
                bIsMove = true;
            }
            if (bIsMove)
            {
                m_iLengthArray[m_iWriteIndex] = 0;
                m_szBufArray[m_iWriteIndex] = NULL;
            }
            m_iUsed++;
        }
        //Buffer满了后覆盖原有的
        if (m_iWriteIndex > m_iUsed-1)
        {
            m_iWriteIndex = 0;
        }
        //缓存大小不对时重新分配
        if (m_iLengthArray[m_iWriteIndex] != iLenght && NULL != m_szBufArray[m_iWriteIndex])
        {
            delete []m_szBufArray[m_iWriteIndex];
            m_szBufArray[m_iWriteIndex] = NULL;
            m_iLengthArray[m_iWriteIndex] = 0;
        }
        //如果缓存指针为空， 则重新分配
        if (NULL == m_szBufArray[m_iWriteIndex])
        {
            m_iLengthArray[m_iWriteIndex] = iLenght;
            m_szBufArray[m_iWriteIndex] = new unsigned char[m_iLengthArray[m_iWriteIndex]];
            memset(m_szBufArray[m_iWriteIndex], 0 , m_iLengthArray[m_iWriteIndex]);
        }
        //索引有效，且Buffer不为空， 则拷贝内存
        if (m_iWriteIndex < m_iUsed && NULL != m_szBufArray[m_iWriteIndex])
        {
            memset(m_szBufArray[m_iWriteIndex], 0, m_iLengthArray[m_iWriteIndex]);
            memcpy(m_szBufArray[m_iWriteIndex], szBuf, m_iLengthArray[m_iWriteIndex]);
            if (MAX_IMAGES_COUNT == m_validBuffer.size())
            {
                m_validBuffer.pop_front();
            }
            m_validBuffer.push_back(m_szBufArray[m_iWriteIndex]);
            m_iWriteIndex++; //添加数据成功后指针后移
        }
    }
    unsigned char *getBuffer()
    {
        if (m_validBuffer.size() > 0)
        {
            unsigned char *pBuffer = m_validBuffer.front();
            m_validBuffer.pop_front();
            return pBuffer;
        }
        else
        {
            return NULL;
        }
    }
    int getVaildDataCount() {return m_validBuffer.size();}
    void reset()
    {
        for (int i = 0; i < MAX_IMAGES_COUNT; i++)
        {
            if (NULL != m_szBufArray[i])
            {
                delete []m_szBufArray[i];
                m_szBufArray[i] = NULL;
            }
            m_iLengthArray[i] = 0;
        }
        m_iUsed = 0;
        m_iWriteIndex = 0;
        m_validBuffer.clear();
    }
    void clear()
    {
        m_validBuffer.clear();
    }
private:
    unsigned char *m_szBufArray[MAX_IMAGES_COUNT];  //最多允许16张缓存同时存在
    unsigned int m_iLengthArray[MAX_IMAGES_COUNT];
    list<unsigned char *>   m_validBuffer;
    int  m_iUsed;
    int  m_iWriteIndex;
    int  m_iValidSize;
};

class stImageInfo;
typedef void(*FnTriggerFunction)(void *, stImageInfo*);

//typedef void (*FnTriggerFunction)(void *);

class ICamera
{
public:
	enum
	{
		IID_CAMERA = 0x100
	};
	virtual ~ICamera(){}
    virtual void AddJobNameToList(const char *)  = 0;  //为即将采集的图像分类
    virtual bool SingleAcq( const char *szJobName = 0) = 0;
    virtual bool GetImage(void *pR, void *pG, void *pB, const char *szJobName = 0, bool bSetParam=true, bool acq = true) = 0;
	virtual bool GetImageSize(int &w, int &h) = 0;
    virtual void Reset() = 0;
    virtual bool IsImplemented() {return false;};   //是否是彩色相机
    virtual bool Continuations(bool bOpen) = 0;
    virtual bool IsContinuations(){return false;}          //否在实时采图

	virtual bool Open() = 0;
    virtual bool Close() = 0;
    virtual bool IsOpen() = 0;
    virtual void SetTriggerFunction(FnTriggerFunction, void *) { }
    virtual int  GetImageListSize() { return 0; }
    virtual void GetCameraInfo(VS_CAMERA_INFO &info) {info = m_CameraInfo;}  //读取相机参数
    virtual bool SetCameraInParam(VS_CAMERA_INFO &param)  = 0;//设置相机参数

    virtual void ClearBuf() {};
    //是否取像完成
    virtual bool IsGetImageFinished() = 0;
    //等待取像完成
    virtual bool WaitGetImageFinished(int iTimeOut = -1) = 0;
    //重置取像完成状态
    virtual bool ResetFinishState() = 0;

    virtual int GetLastError(char *errInfo = NULL, int *iSize = NULL) = 0;
protected:
    VS_CAMERA_INFO   m_CameraInfo;
};

#endif // ICAMERA_H
