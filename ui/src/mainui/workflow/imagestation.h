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

    bool startGrapProcess(bool bThread = true);	// ��ʼ�ɼ�����

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
	// д�䷽����
	bool WriteEleAndSqe();

	bool AbortGrabImage();

	void ResetAcq();

	// �����Դ������
	bool ClearLightCrlNumber();

	// �������ù�Դ����
	bool SetCamerCfg();


private:

	bool GrapProcessFun();

	void readStationCfg();

	// ����ƿ���������
	bool setControllerParams(bool bGgCard=true);


	// ֪ͨ�����ʼ�ɼ�
	bool TriggerGrabImage();

	// �����ڴ濪ʼ�ɼ�
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
	IImageLogicManager* m_pImageLogicManager;		//   �������
	lightcontrol_new* m_plightcontrol_new;          //   ��Դ������
	motioncontrol_new* m_pmotioncontrol;			//   �Զ��������
    int m_iMotionCtl;		//   �˶�������
	int m_itrigNetid;			//   �˶������� netid
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
