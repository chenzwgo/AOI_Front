#ifndef IMAGESTATION_H
#define IMAGESTATION_H
#include <vector>
#include "businesscommon.h"
#include "jthreadt.h"
#include "jmutex.h"
#include "jtime.h"
#include <QList>
using namespace std;

class IImageLogicManager;
class lightcontrol_new;
class motioncontrol_new;
class ICardContInterp;
class IStation;
class CImageProduct;

class CImageStation
{
public:
    CImageStation(int index);
    ~CImageStation();

    IImageLogicManager* getImageLogicManager();
    lightcontrol_new* getLightcontrol();

    bool isEnable();

    bool startGrapProcess(bool bThread = true);	// 开始采集流程

    bool stopGrapProcess();		//

    bool startContinueGrap();

    bool stopContinueGrap();

    bool resetStation();

    bool saveRawImage(CImageProduct* pProduct);

    void pushBackProduct(CImageProduct* pProduct);

    CImageProduct* popProduct();

    void removeAllProduct();

    unsigned int getProductBuffSize();

    unsigned int getEnableChannelCnt();

public:
	// 写配方电流
	bool WriteEleAndSqe();

	bool AbortGrabImage();

	void ResetAcq();

	// 清楚光源控制器
	bool ClearLightCrlNumber();

	// 重新设置光源参数
	bool SetCamerCfg();


private:

	bool GrapProcessFun();

	void readStationCfg();

	// 设控制卡触发参数
	bool setControllerParams(bool bGgCard=true);


	// 通知相机开始采集
	bool TriggerGrabImage();

	// 分配内存开始采集
	bool StartGetImage();

	bool IsAcqFinished();

	bool waitTriggerFinish(bool& bRun);

	bool getLightCrlNumber();

	bool StartAcqImage();
	
private:
	bool getCameraPara(CameraParam &cfg);

private:
	bool m_bEnable;
	int m_iIndex;
	IImageLogicManager* m_pImageLogicManager;		//   相机分组
	lightcontrol_new* m_plightcontrol_new;          //   光源控制器
	motioncontrol_new* m_pmotioncontrol;			//   自定义控制器
    int m_iMotionCtl;		//   运动控制器
	int m_itrigNetid;			//   运动控制器 netid
	bool m_bcardgg;
	bool m_bLineCam;
	string m_strTrigIOName;
	ICardContInterp*  m_pCardConInterp;
	IStation* m_pTargetStation;
	bool m_bRunning;
	JThreadT<CImageStation, bool>* m_pGrapthread;
    QList<CImageProduct*> m_productStack;
    JMutex m_stackMutex;
	uint64 m_startLastGrapTime;
	int m_iProNumber;
};


class  CImageStationManager
{
public:
	static CImageStationManager* GetInstance();
	void   ReleaseInstance();
	CImageStation*  getImageStation(int index);
	void releaseImageStation(int index);

private:
	CImageStationManager();
	~CImageStationManager();
	static CImageStationManager* m_pManagerInstance;
	CImageStation* ImageStations[8];


};
#endif // IMAGESTATION_H
