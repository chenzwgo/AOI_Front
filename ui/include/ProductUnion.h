#pragma once
#include <QList>
#include <vector>

using namespace std;

#include "maindefine.h"
#include "g12globalenv.h"
#include "jmutex.h"
#include "../../dlapi/include/classification_interface.h"
//#include "classification_interface.h"
#include <QTime>
#include <QPolygon>
static int Statusbit = 0;
enum enProStatus
{
	en_error = 0,							// ����
	en_idle = 1 << 0,						// ����
	en_preGrap = 1 << 1,					// ��ͼǰ
	en_graping = 1 << 4,				// ��ͼ��
	en_grapfinish = 1 << 5,			// ��ͼ���
	en_prProcess = 1 << 6,					// Ԥ����ʼ
	en_prProcessfinish = 1 << 7,			// Ԥ�������
	en_multiChannel = 1 << 8,				// ��ͨ����ȡ
	en_multiChannelfinish = 1 << 9,			// ��ͨ����ȡ����
    en_extracting =1<<12,					// �㷨ȱ����ȡ
    en_extractfinish =1<<13,				// �㷨ȱ����ȡ���
	en_defectleveling = 1 << 14,            // ȱ�ݷ�ͨ����ȡͬʱ�ֵȼ�
	en_defectlevelfinish = 1 << 15,         // ȱ�ݷ�ͨ����ȡͬʱ�ֵȼ����
    en_updateui=1<<16,						// ����UI
    en_updateuifinish=1<<17,				// ����U���
    en_saveResImage=1<<18,					// ������ͼƬ
    en_saveResImagefinish = 1<<19,			// ������ͼƬ���
    en_processfinish =1<<20					// �������
};

enum enImgBufferFlag
{
    en_img_Grap=0,               // ��ͼ��
    en_img_StaticShading,        // ��̬
    en_img_DynamicShading,       // ��̬
    en_img_SignalFilter,         // �ź��˲�
	en_img_Threshold  ,         // ��ֵ�ָ�
    en_img_Inner  ,             // Inner
};


struct ImageBuffer 
{
	stResImage rawData;
	stResImage staticData;
	unsigned int m_iWidth;
	unsigned int m_iHeight;
	ImageBuffer(unsigned int iWidth,unsigned int iHeight)
	{
		if (iWidth != 0 && iHeight != 0)
		{
			m_iWidth = iWidth;
			m_iHeight = iHeight;
			mallocMem();
		}
	}
	void mallocMem()
	{
		rawData.img_data = new unsigned char[m_iWidth*m_iHeight];
		rawData.img_height = m_iHeight;
		rawData.img_width = m_iWidth;

	}

	void clearMem()
	{
		rawData.clearMem();
		staticData.clearMem();
	}
	void resetMem(unsigned int iWidth, unsigned int iHeight)
	{
		if (iWidth == m_iWidth && iHeight == m_iHeight)
		{
			return;
		}
		releaseMem();

		m_iWidth = iWidth;
		m_iHeight = iHeight;

		mallocMem();
	}
	
	void releaseMem()
	{
		rawData.resetMem();
		staticData.resetMem();
	}

	~ImageBuffer()
	{
		releaseMem();
	}
};

class CImageProduct
{
public:

    CImageProduct(unsigned long long  ullSn);
    ~CImageProduct();

    bool isProductOneOnStatus(enProStatus status, int index = 0);
    bool isProductAllOnStatus(enProStatus status, int index = 0);
	bool isChannelOneOnStatus(QString strChannel, enProStatus status, int index = 0);
	bool isChannelAllOnStatus(QString strChannel, enProStatus status, int index = 0);
	void setChannelAllStatus(QString strChannel, enProStatus status, int index = IMAGE_STATION_MAX_CNT);
    void setProductAllStatus(enProStatus status, int index = IMAGE_STATION_MAX_CNT);


	void setProductImageSize(unsigned int iWidth, unsigned int iHeight, int index = 0);

    unsigned int getProductWidth(int index = 0);
    unsigned int getProductHeight(int index = 0);

	void setStartGrapImageTime();
	void setEndGrapImageTime();
	unsigned int grapImageElapsedTime();

    void setStartProcessTime();
	void setEndProcessTime();
    unsigned int processElapsedTime();

	void setUpdateUIFinish(bool bFlag);
	bool getUpdateUIFlag();

    unsigned long long getProductSn();
	void setProductSn(unsigned long long  ullSn);

	ImageBuffer* getImageBuffer(QString strChannelName,int index = 0);

	bool isFree();
	void setFree(bool bFree);
	void resetStatus();
	void resetFlagStatus();
	void resetMemStatus();

	// �жϴ����Ƿ����
	eProcessErroeCode getProcessError();
	void setProcessErrCode(eProcessErroeCode bflag);
	// ȱ�ݽ��
	vector<stDefectFeature*>*   pDefectStrutVec;
	stProductSummary     m_stProductSummary;

    QRect m_cutRoiInfo[2];
    QPolygon m_curPolygon[2][2];

	bool m_bSetProcessStartTimeflag;
	bool m_bSetProcessEndTimeflag;
	bool m_bSetGrapImageStartTimeflag;
	bool m_bSetGrapImageEndTimeflag;

	void SetZipPath(const QString& path);
	QString GetZipPath();
	//
	unsigned int m_iWidth[IMAGE_STATION_MAX_CNT];
	unsigned int m_iHeight[IMAGE_STATION_MAX_CNT];
	eProcessErroeCode m_ProcessErrorCode;
	QMap<QString, ImageBuffer*>  m_mapImageBuffer[IMAGE_STATION_MAX_CNT];							// ��ͨ���ģ�ͼƬbuffer
	QMap<QString, enProStatus>	m_mapProStatus[IMAGE_STATION_MAX_CNT];								// ��ͨ���ģ�ͼƬ�����״̬
	unsigned long long  m_proSn;                                            // ��ƷSn ,�����������϶����Ʒ   
	string m_str2Dcode;
	map<string, double> m_glassSize;
	stProductDataRes m_MarkFrontRes;//��־���ε�ǰ��Ʒ�����//add 2021/2/24
private:
    JMutex m_mutexStatus;
	JMutex m_mutexTime;                              

    QTime m_startTimeProcess;
    QTime m_endTimeProcess;

	QTime m_startTimeGrapImage;
	QTime m_endTimeGrapImage;

	bool m_bUpdateUI;
	bool m_bIsFree;
	bool m_bSetProductImageSize[IMAGE_STATION_MAX_CNT];

	QString m_zipPath;
};

class CProductUnion
{

public:
    static CProductUnion *getInstance();
    static void releaseInstance();
     
	void initProductBuffer(unsigned int isize);
    // ����/���ٲ�Ʒ�ṹ��
    CImageProduct* createProduct();
    void releaseProduct(CImageProduct* pProduct);
    void releaseAllProduct();
    //��ȡ��ǰ��Ʒ����
    int getProductCnt();
	bool isProductExist(CImageProduct* pProduct);

    //��ѯָ����Ʒ
    CImageProduct* getProduct(int index);
    CImageProduct* getProductBySn(unsigned long long  proSn);

	CImageProduct* getChannelOneOnStatus(QString strChannel,enProStatus status);
	CImageProduct* getChannelAllOnStatus(QString strChannel, enProStatus status);

    CImageProduct* getProductOneOnStatus(enProStatus status,int index = IMAGE_STATION_MAX_CNT);
    CImageProduct* getProductAllOnStatus(enProStatus status,int index = IMAGE_STATION_MAX_CNT);

	CImageProduct* getProductGrapFinish(int index = 0);

    unsigned int readCurrentDayPrSn();
    void writeCurrentDayPrSn(unsigned int ullSn);
private:

    static CProductUnion *m_pInstance;
    CProductUnion(void);
    ~CProductUnion(void);

    static unsigned long long Sn; 
    QList<CImageProduct*> m_listProduct;
    JMutex m_mutexProductList;
	QString m_strMarkDay;
	
};

