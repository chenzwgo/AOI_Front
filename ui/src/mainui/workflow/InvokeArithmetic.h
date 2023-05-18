#pragma once

#include "ProductUnion.h"
#include "jthreadt.h"
#include "jevent.h"
#include <map>
#include <string>
#include <jevent.h>
#include "defect/idefectqualification.h"
#include "tinyxml.h"
#include <stack>
using namespace std;

class  QFile;
class  IArithmeticStaticMgr;
class  IDefectQualification;
class  CDefectLevelThreads;
class IDlArithmetic;
struct ImageBufferAndChannel
{
	ImageBuffer* imgBuff;
	unsigned int iStation;
	unsigned int iChannel;
	int iHOffset;
	ImageBufferAndChannel()
	{
		imgBuff = NULL;
		iStation = 0;
		iChannel = 0;
		iHOffset = 0;
	}
};
class CInvokeArithmetic
{
public:
    CInvokeArithmetic(void);
    ~CInvokeArithmetic(void);
    
    static CInvokeArithmetic *getInstance();
    static void releaseInstance();

    bool init();
    bool unInit();

    void startDeepLearnProcess();

	void startGrapLocalImage();

	void startPocess();

    void resetProcess();

	bool isResetFinish();

	int reProcessSpecialProduct(unsigned long long sn);

    uint64 getDvElapsedTime();

	void startAotoTest(int imax);
	void setAotoTestFlag(bool bFinish = true);
	void setStopAotoTest();
    //IDlArithmetic* getDlArithmetic();
	void SetUnInitDeepLearn();
	InitDeepLearnState GetDeepLearnEnvState();
private:
    static CInvokeArithmetic *m_pInstance;

    void startImageProcess();

	void startSaveImage();

	void startUpdataToUI();

	void startRecycling();

    int updataResultToUI(CImageProduct* pProduct);

    void SaveProductResult(unsigned long long proSN, stDefectFeature *mark,QString &data);
	void SaveOneProductResult(unsigned long long proSN,QString &data);
	void SaveProductSummaryResult(unsigned long long proSN, stProductSummary *pProductSummary);
	QString GetSaveResultSavePath(QString strPath = "");//strPath 为增加的文件夹
    static void saveProcessImgage(unsigned char* pImageData,unsigned int iwidth,unsigned iHeight,string name);
	static void saveRawImgage(unsigned char* pImageData,unsigned int iwidth,unsigned iHeight,string name);

	void ThreadGrapLocalImage(void* param);			// 图片预处理线程

    void ThreadImageProcess(int param);			// 图片预处理线程
    void ThreadImageDeepLearnProcess(void* param);  // 深度学习处理现场
    void ThreadUpdateResult(void* param);           // 结果更新线程
	void ThreadSaveImage(int param);				 // 图片预处理线程
    void ThreadRecyclingProcess(void* param);       // 保存各种处理后的效果图
	void ThreadLocalImage(void* param,string strChannel, int Image_station=0);			//  按通道数加载离线图片

    void ThreadAotoTestProcess(void* param);       // 
	stResImage getPreProcessImage(string& strChannel,CImageProduct* pProduct);

    void readEnableConfig();
    void readOtherConifg();

	void defectLevelAndSaveFile(CImageProduct* pProduct);

	void pushDvElapsedTime(uint64 uiElapsedTime);
	void clearDvElapsedTime();
	void writeClassSizeCsv(const QString &strData);
	void productFillWithHistoryData(CImageProduct* pProduct);

	void readClassSizeCsv(CImageProduct* pProduct);		// 读取玻璃尺寸
	void readDefectLevel(CImageProduct* pProduct);		// 读取产品缺陷等级信息
	void readProductSummary(CImageProduct* pProduct);	// 读取产品的统计信息
	int DefectPosToRegion(int iwidth, int height, int posX, int posY);
	void repairGigeImage(CImageProduct* pProduct);
	QString resultFlagToStr(enResFlag flag);

	void ImageForAi(CImageProduct* pProduct,int iStation);
	void ThreadDAiImgeProcess(ImageBufferAndChannel* imgBuffer);private:


    JThreadTP<CInvokeArithmetic, int, void>* m_pProcessThreads[IMAGE_STATION_MAX_CNT];
	JThreadTP<CInvokeArithmetic, int, void>* m_mapSaveImageThreads[IMAGE_STATION_MAX_CNT];
    JThreadTP<CInvokeArithmetic,void*,void>* m_pDeepLearnThread;
    JThreadTP<CInvokeArithmetic,void*,void>* m_pUpdateResultThread;
    JThreadTP<CInvokeArithmetic,void*,void>* m_pRecyclingThread;
	JThreadTP<CInvokeArithmetic, void*, void>* m_pGrapLocalImageThread;
	JThreadTP<CInvokeArithmetic, void*, void>* m_pAotoTesthread;
	JThreadTP<CInvokeArithmetic, void*, void>* m_pWaitGrapFinishthread;

	JThreadTP<CInvokeArithmetic, int, void>* m_pImgeThreads[IMAGE_STATION_MAX_CNT];

	map<string, JThreadTP<CInvokeArithmetic, ImageBufferAndChannel*, void>* > m_mapAiImgeThreads;

    bool m_bPreProcessThread;
	bool m_bProcessThread;
    bool m_bDeepLearnThread;
    bool m_bUpdateResultThread;
    bool m_bSaveImageThread;
	bool m_bRecyclingThread;
    bool m_bIsFastGrab;
    bool m_bAotoTesthread;//
	bool m_bWaitGrapFinishthread;

    JEvent  m_SaveImageEvent;
	QFile* m_csvFile;
	stEnableConfig m_enableConfig;

    IDefectQualification *m_pIDefectQualification;

	CDefectLevelThreads* m_pCDefectLevelThreads;	//多线程处理等级分类
    stCutOutConfig  m_catoutParam;
    stInterpolationConfig  m_InterpolationParam[SEQUE_NUMBER];
	stack<uint64> m_DVElapsedTimestack;
    JMutex  m_ElapsedTimeLock;
    uint64 m_processStartTime;    // 处理开始时间
    void SaveCuseImage(CImageProduct* pProduct);
	bool m_bFinishFlag;
    int m_iAotoTestMax;
	IDlArithmetic* m_pIDlArithmetic0;
    IDlArithmetic* m_pIDlArithmetic;
	int m_iIndexDlArithmetic;
	int m_iIndexDlArithmetic2;
	int m_iIndexDlArithmetic0;

	vector<string> m_defectNameVec;
	vector<string> m_defectEnNameVec;
	vector<pair<string,bool> > m_defectEnableVec;

};
