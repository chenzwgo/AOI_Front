#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
#include "g12globalenv.h"
#include "vscameratools/camera/ilinecamera.h"
#include "vscameratools/cameramanager/icamerabuffermanager.h"
#include "jevent.h"
#include "jthreadt.h"
#include "common_data.h"
#include <string>
using namespace std;

#define MAX_IMAGE_COUNT   8
#define CAM_GROUP_COUNT   CAMERA_NUMBER     //相机分组个数
#define CAM_INDEX_COUNT   CAMERA_NUMBER     //每组中相机最大个数

class ILineCameraFactoryManage;

class CImageProcess
{
public:
    CImageProcess(string strCamName);
    ~CImageProcess();

    void setCamName(string strCamName);
    void setParam(int xOffset, int yOffset = 0,int ibufferWidth=0,int ibufferHeight=0);
    bool setBufferInfo(int iCount, stImageInfo **pBuffer);

    bool startProcess();

    bool continuations(/*FnLineTriggerFunction fnTriggerFunction, void *pInParam*/); //block == 1, 取完就清
    bool stopContinuations();
    bool isRecvDataFinished();
    void resetFinishedStatus();

    bool reset();
    bool abort();

    bool isBusying();
    bool isGrabFinished(int iChannel, int index = -1);
    bool isInitFinished();
    bool stopInit();

    int getCamStatus();
private:
    static void CallbackGrapFunction(void *, unsigned char *, int, int);

    void insertBuffer(const unsigned char *, int, int);

    void init();
    bool waitForInitFinished();

    void ThreadInitPro(void* param);
private:
    //合成图像大小
    int                  m_bufferWidth;     
    int                  m_bufferHeight;
    int                  m_iChannelCount;   //相机通道数
    int                  m_camXOffset;
    int                  m_camYOffset;
    stImageInfo         *m_imageInfo[SEQUE_NUMBER];
    int                  m_curRecvIndex[SEQUE_NUMBER];
    int                  m_currentChannalIndex;

    bool                 m_bIsRealTime;   //是否为实时采图
    string               m_strCamName;
    ILineCameraFactoryManage *m_pCameraManager;
    ILineCamera         *m_pCameraInstance;
    JEvent               m_eventDataRecv;
    JEvent               m_eventGetCamInstance;
    JThreadTP<CImageProcess, void*, void>*    m_pInitThread;
    bool                 m_bIsInitRunning;

    JMutex               m_proLocker;
	bool				m_bMirror;
};

#endif // IMAGE_PROCESS_H
