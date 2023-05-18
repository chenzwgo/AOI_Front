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
    void init();												// ��ʼ�������ɺ��ٳ�ʼ��
	void uninit();

    bool setProductPtr(CImageProduct *product);					// ����Ʒ��Ϣ������
    bool setImageParam(int iMinLength = 12800);
	bool startGetImage();
	bool isAcqFinished();
	bool resetImageProcess();

    bool triggerGrabImage();
	bool abortGrabImage();
	bool updateImageBuffer();									// ���»���
	bool GetSetExporeTime(int expore, double &time);		        // ���������ع�ʱ��

private:
	VS_CAMERA_INFO ReadConfig();								// ��ȡ�������
	static void CallbackGrapFunction(void *, stImageInfo*);		// �ص�����
private:
	CHkImageLogic*	m_ImageProInstance[SEQUE_NUMBER];
	CImageProduct*	m_product;
	string          m_strSeqName[SEQUE_NUMBER];
	int             m_iChannelCount;							// ���ͨ����
	int				m_bufferWidth;								// ͼƬ��
	int				m_bufferHeight;								// ͼƬ��
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
	bool setBufferInfo(int iCount, stImageInfo *pBuffer);			// ���û�����Ϣ
	void pushImageBuffer(stImageInfo* pstImageInfo);
private:
	void ThreadImageProcess(void* param);							// ��ȡͼƬ�߳�
	void ImageCorrection(const char *channelName, int index);		// �ɼ�״̬����
	void readConfig();	
	void clearImageBuffer();
	stImageInfo* popImageBuffer();
private:

	JThreadTP<CHkImageLogic, void*, void>*	m_pThreadImagePro;
	int										m_iMaxChannelCount;		// ���ͨ����
	int										m_seqIndex;				// ͨ������
	int										m_iChannelCount;		// ���ͨ����
	
	bool									m_bRunning;
	bool									m_bReset;

	string									m_strSeqName;
	JMutex									m_ImageLock;
	JMutex									m_EventLock;
	JEvent									m_eStartImagePro;
	JEvent									m_eImageReadFinished;
	JEvent									m_eImageReading;
	CImageProduct*							m_product;				//��ǰ��Ʒ��Ϣ
	INotifyService*							m_pNotifyService;
    int				m_index;
    string									m_strScene;
	list<stImageInfo*>						m_imageBuffer;
	JMutex									m_imageBufferMutex;
};


#endif // CHKIMAGELOGIC_H
