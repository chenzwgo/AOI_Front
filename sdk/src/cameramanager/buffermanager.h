#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H
#include "g12globalenv.h"
#include "vscameratools/camera/ilinecamera.h"
#include "vscameratools/cameramanager/icamerabuffermanager.h"
#include "jevent.h"
#include "jthreadt.h"
#include "common_data.h"
#include "imageProcess.h"
#include <string>
#include <map>
using namespace std;

#define MAX_IMAGE_COUNT   8
#define CAM_GROUP_COUNT   CAMERA_NUMBER     //����������
#define CAM_INDEX_COUNT   CAMERA_NUMBER     //ÿ�������������
	
class ILineCameraFactoryManage;

class CBufferManager : public IBufferManager
{
public:
    CBufferManager(string SceneName);
    ~CBufferManager();

    bool init(); //����������ͼ��
    //ͨ������ͼ�񳤡����ص�X�����ص�Y��
    //bool setBufferParam(); ֱ�ӹ�������
    //bool updateSetting();

    virtual bool startProcess();
    virtual void getBufferSize(int &width, int &height);
    virtual const unsigned char *getBuffer(int iChannel, int lineOffset = 0, int iLineNumb = -1);
    virtual const unsigned char *getBufferAdress(int iChannel);

    virtual bool continuations(int channel, FnDataRecvFunction fnTriggerFunction, void *pInParam); //block == 1, ȡ�����
    virtual bool stopContinuations(int channel);

    virtual bool reset();
    virtual bool abort();

    virtual bool isBusying();
    virtual bool isInitFinished();

    virtual bool updateImageBuffer();
    virtual bool stopInit();

    virtual int  getCamStatus(int iGroupIndex);

    static CBufferManager *getInstance();
    static void releaseInstance();
private:
    //static void CallbackGrapFunction(void *, int, unsigned char **, int, int, int);

    void ThreadRealTimePro(void* param);
	void loadConfig();
private:
    FnDataRecvFunction    m_fnTriggerFunction;
    void                 *m_pInParam;

    int                   m_camWidth;
    int                   m_camHeight;
    int                   m_bufferWidth;
    int                   m_bufferHeight;
    int                   m_bufferOverlapX;
    int                   m_bufferOverlapY;

    stImageInfo           m_imageInfo[SEQUE_NUMBER];
    bool                  m_bIsEnable[SEQUE_NUMBER];
    
    CImageProcess        *m_ImageProInstance[CAM_GROUP_COUNT][CAM_INDEX_COUNT]; //TBM
    string                m_strCamName[CAM_GROUP_COUNT][CAM_INDEX_COUNT];

    //FnLineTriggerFunction m_fnTriggerFunction;
    //void                 *m_pInParam;
    JThreadTP<CBufferManager, void*, void>*    m_pRealTimeThread;
    int                   m_currentChannel;
    bool                  m_bIsContinuations;
	bool                  m_bIsFast;

    static CBufferManager *m_pInstance;
    JMutex                 m_proLock;
	string m_SceneName;
	bool m_bIsEnableChannel[CAM_GROUP_COUNT][SEQUE_NUMBER];		// 4���������8��ͨ���У���Щͨ��ʹ��
	int m_iEnableChannelCnt[CAM_GROUP_COUNT];					// 4�����,ÿһ�����ͨ����ʹ�ܼ���������1���� A,B,C ʹ�á���ô��������3
	int m_iChannelEnableCamIndex[SEQUE_NUMBER];					// 8��ͨ����ÿ��ͨ�����ĸ�һ�������ʹ�ܡ���¼������ʹ��
};

class BufferManagerFactory:public IBufferManagerFactory
{
public:

	BufferManagerFactory();
	~BufferManagerFactory();
	static BufferManagerFactory *getInstance();
	static void releaseInstance();

	IBufferManager* createBufferManager(string SceneName);
	void relealseBufferManager(IBufferManager* pBufferManager);
	void relealseBufferManager(string SceneName);
private:
	static BufferManagerFactory* m_pBufferManagerFactory;
	map<string, CBufferManager*> m_mapBufferManager;
	JMutex m_mapMutex;
};


#endif // BUFFERMANAGER_H
