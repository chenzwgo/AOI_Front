#ifndef CHKIMAGELOGIC_H
#define CHKIMAGELOGIC_H
#include "iimagelogicmanager.h"
#include "jthreadt.h"
#include "jevent.h"
#include "businesscommon.h"
#include "g12globalenv.h"
#include "common_data.h"
#include <map>
#include <string>
#include <QString>
#include <QMap>
#include <QTime>
#include "vscameratools/camera/icamera.h"
#include "vscameratools/camera/icamerafactory.h"
#include "ProductUnion.h"
using namespace std;

class IFileVariable;
struct stImageInfo;
struct CImageProduct;
class INotifyService;
class CHkImageLogic;

class CHkImageLogicManager: public IImageLogicManager
{
public:
    CHkImageLogicManager(int index = 0);
	~CHkImageLogicManager();
    void init();												// 初始化相机完成后再初始化
	void uninit();

    bool setProductPtr(CImageProduct *product);					// 将产品信息传进来
    bool setImageParam(int iMinLength = 12800);
	bool startGetImage();
	bool isAcqFinished();
	bool resetImageProcess();

    bool triggerGrabImage();
	bool abortGrabImage();
	bool updateImageBuffer();									// 更新缓存
	bool GetSetExporeTime(int expore, double &time);		        // 测试设置曝光时间

private:
	VS_CAMERA_INFO ReadConfig();								// 读取相机参数
	static void CallbackGrapFunction(void *, stImageInfo*);		// 回调函数
private:
	CHkImageLogic*	m_ImageProInstance[SEQUE_NUMBER];
	CImageProduct*	m_product;
	string          m_strSeqName[SEQUE_NUMBER];
	int             m_iChannelCount;							// 相机通道数
	int				m_bufferWidth;								// 图片宽
	int				m_bufferHeight;								// 图片高
	bool			m_bRunning;			
	QTime			m_startGrabTime;

	ICameraFactory*	m_pCameraFactory;
	ICamera*		m_pCameraInstance;
	VS_CAMERA_INFO	m_pCameraInfo;
	stImageInfo		m_ImageInfo[SEQUE_NUMBER];
    string m_strScene;
    int m_iIndex;
	int m_iGetImageIndex;
	int m_iAreaiChanelCount;
	int m_iOverSize;
};

class CHkImageLogic
{
public:
	CHkImageLogic(string strSeqName, int indexSeq);
	~CHkImageLogic();
    void setImageParam(string strSeqName, int index, string strSceneName = "");
	void BeginReadImage(bool bOnLine = true);
	bool bIsReadFinished();
    void SetProductPtr(CImageProduct *product ,int index );
	void Reset();
	bool setBufferInfo(int iCount, stImageInfo *pBuffer);			// 设置缓存信息
	void pushImageBuffer(stImageInfo* pstImageInfo);
private:
	void ThreadImageProcess(void* param);							// 提取图片线程
	void ImageCorrection(const char *channelName, int index);		// 采集状态设置
	void readConfig();	
	void clearImageBuffer();
	stImageInfo* popImageBuffer();
private:

	JThreadTP<CHkImageLogic, void*, void>*	m_pThreadImagePro;
	int										m_iMaxChannelCount;		// 最大通道数
	int										m_seqIndex;				// 通道索引
	int										m_iChannelCount;		// 相机通道数
	
	bool									m_bRunning;
	bool									m_bReset;

	string									m_strSeqName;
	JMutex									m_ImageLock;
	JMutex									m_EventLock;
	JEvent									m_eStartImagePro;
	JEvent									m_eImageReadFinished;
	JEvent									m_eImageReading;
	CImageProduct*							m_product;				//当前产品信息
	INotifyService*							m_pNotifyService;
    int				m_index;
    string									m_strScene;
	list<stImageInfo*>						m_imageBuffer;
	JMutex									m_imageBufferMutex;
};


#endif // CHKIMAGELOGIC_H
