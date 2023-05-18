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
	en_error = 0,							// 错误
	en_idle = 1 << 0,						// 空闲
	en_preGrap = 1 << 1,					// 采图前
	en_graping = 1 << 4,				// 采图中
	en_grapfinish = 1 << 5,			// 采图完成
	en_prProcess = 1 << 6,					// 预处理开始
	en_prProcessfinish = 1 << 7,			// 预处理结束
	en_multiChannel = 1 << 8,				// 多通道提取
	en_multiChannelfinish = 1 << 9,			// 多通道提取结束
    en_extracting =1<<12,					// 算法缺陷提取
    en_extractfinish =1<<13,				// 算法缺陷提取完成
	en_defectleveling = 1 << 14,            // 缺陷分通道提取同时分等级
	en_defectlevelfinish = 1 << 15,         // 缺陷分通道提取同时分等级完成
    en_updateui=1<<16,						// 更新UI
    en_updateuifinish=1<<17,				// 更新U完成
    en_saveResImage=1<<18,					// 保存结果图片
    en_saveResImagefinish = 1<<19,			// 保存结果图片完成
    en_processfinish =1<<20					// 处理完成
};

enum enImgBufferFlag
{
    en_img_Grap=0,               // 采图中
    en_img_StaticShading,        // 静态
    en_img_DynamicShading,       // 动态
    en_img_SignalFilter,         // 信号滤波
	en_img_Threshold  ,         // 阈值分割
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

	// 判断处理是否错误
	eProcessErroeCode getProcessError();
	void setProcessErrCode(eProcessErroeCode bflag);
	// 缺陷结果
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
	QMap<QString, ImageBuffer*>  m_mapImageBuffer[IMAGE_STATION_MAX_CNT];							// 多通道的，图片buffer
	QMap<QString, enProStatus>	m_mapProStatus[IMAGE_STATION_MAX_CNT];								// 多通道的，图片处理的状态
	unsigned long long  m_proSn;                                            // 产品Sn ,用于区分线上多个产品   
	string m_str2Dcode;
	map<string, double> m_glassSize;
	stProductDataRes m_MarkFrontRes;//标志上游当前产品检测结果//add 2021/2/24
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
    // 创建/销毁产品结构体
    CImageProduct* createProduct();
    void releaseProduct(CImageProduct* pProduct);
    void releaseAllProduct();
    //获取当前产品数量
    int getProductCnt();
	bool isProductExist(CImageProduct* pProduct);

    //查询指定产品
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

