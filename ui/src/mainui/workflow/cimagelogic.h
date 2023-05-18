#ifndef CIMAGE_LOGIC_H
#define CIMAGE_LOGIC_H

#include "jthreadt.h"
#include "jevent.h"
#include "businesscommon.h"
#include "g12globalenv.h"
#include "common_data.h"
#include "ProductUnion.h"
#include <map>
#include <string>
#include <QString>
#include <QMap>
#include <QTime>
using namespace std;

class IFileVariable;
struct stImageInfo;
struct CImageProduct; 
class INotifyService;
class CImageLogic;
class IBufferManager;


#define MAX_IMAGE_COUNT   8
#define CAM_GROUP_COUNT   CAMERA_NUMBER     //相机分组个数
#define CAM_INDEX_COUNT   CAMERA_NUMBER     //每组中相机最大个数

class CImageLogic
{
public:
    CImageLogic(IBufferManager* pCameraBufferManager);
    ~CImageLogic();

    void setImageParam(string strSeqName, int index, string strSceneName = "");
    void setEnable(bool bEnable);

    //void SetImageInfo(/*bool *bIsEnableChannel, */int iCount, string *seq);
    void BeginReadImage(bool bOnLine = true);
    bool bIsReadFinished();
	void SetProductPtr(CImageProduct *product, int index );
	void Reset();

    //bool triggerGrabImage();
    bool abortGrabImage();
private:
    void ThreadImageProcess(void* param);
    void ImageCorrection(const char *channelName, int index);

    void readConfig();
private:
    IFileVariable*                        m_fileVariable;
    //bool                                  m_bIsEnableChannel[MAX_IMAGE_COUNT];   //对应通道是否使能
    int                                   m_iMaxChannelCount;   //最大通道数
    string                                m_seqName[SEQUE_NUMBER];
	JMutex                                m_ImageLock;
	JMutex                                m_EventLock;
    string                                m_strSeqName;
    int                                   m_seqIndex;

    JThreadTP<CImageLogic,void*,void>*    m_pThreadImagePro;
    JEvent                                m_eStartImagePro;
	JEvent                                m_eImageReadFinished;
	JEvent                                m_eImageReading;
    bool                                  m_bRunning;
    bool                                  m_bReset;
    CImageProduct                        *m_product; //当前产品信息

    INotifyService                       *m_pNotifyService;
    int                                   m_imageXOffset;
    IBufferManager                       *m_pCameraBufferManager;

    int                                   m_bufferWidth;
    int                                   m_bufferHeight;
    int                                   m_blockSize;
    int                                   m_bufferOverlapY;
    bool                                  m_bProWhole;
    bool                                  m_bIsEnable;
    bool                                  m_bIsFast;
	string								  m_strScene;
	int			m_index;
};

#endif // CIMAGE_LOGIC_H
