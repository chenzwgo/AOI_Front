#ifndef WORKFLOW_FUN_H
#define WORKFLOW_FUN_H
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "station/istation.h"
#include "businesscommon.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/camera/ilinecamera.h"
#include "QList"
#include "g12common.h"
#include "g12globalenv.h"
#include "jthreadt.h"
#include "jevent.h"
#include "common_data.h"
#include "maindefine.h"
#include "qstring.h"
#include "cimagelogic.h"
#include "cimagelogicmanager.h"
#include "imagestation.h"
#include <string>
#include <QMutex>
#include <QMap>
#include <QStack>
using namespace std;
class QImage;
class ISocketproxy;

class WorkFlowFun
{
public:
    static WorkFlowFun *getInstance();
    static void releaseInstance();
    
    void   Reset();
    void   Start();
	void   Stop();
    // �л���Ʒ����д���䷽�����
    void   Reinit();
	
	bool   Crlmove(int imove = 0);
	bool   JobMoveForClearMode();
private:
    WorkFlowFun(void);
    ~WorkFlowFun(void);

public:
    void registerMainFlowCallBackFun();

    static int callFunMoveForCheck(void* parent, string param);//������ƶ�

    static int callFunMoveJobForCheck(void* parent, string param);      //�������������
    static int callFunSetRun(void* parent, string param);               //�����б�־ run 

    static int callFunUSleep(void* parent, string param);                //�������ѧϰ�ӿ�

    //��ˮ�߲ɼ�ͼ��ӿ�
    static int callFunPieceByPiecework(void* parent, string param);        //�ɼ�����   

	//��ˮ�߲ɼ�ͼ��ӿ�//
	//
    static int callFunWriteEleAndSqe(void* parent, string param);////д����&& �䷽
   
	//
	static int callFunResetCam(void* parent, string param);     //��λ���
    static int callFunResetLight(void* parent, string param);   //��չ�Դ����������

	static int callFunCheckInProductOnTime(void* parent, string param);// ����ָʾ�Ʋ���//

	// �����Զ�����(������)
	static int callFunAutoTriggerByLines(void* parent, string param);

	static int callFunDeepLearnEnv(void* parent, string param);//H�㷨��ʼ��

public:
    static int callFunSetCamerCfg(void* parent, string param); //���������

	static int queryBackPcStatus(int& iStatus);							//��ѯ��̨PC��״̬  0 ��ѯ�ɹ� 1 δʹ��ǰ���Ѷ  2.����ʧ�ܣ�3 ��ѯʧ��
private:
    bool getCameraPara(CameraParam &cfg);

    void init();

private:
    static WorkFlowFun *m_pInstance;
	iMotion*          m_pMotion;
	ICard *           m_pCard;
	ICardContInterp*  m_pCardConInterp;
 
    IStation* m_pTargetStation;
	IStation* m_pCodecStation;

	vector<CImageStation*> m_ImageStations;
    //int m_iCameraOn[CAMERA_NUMBER];       // 1111//��Ӧ��4λ4�����ʹ��
    bool                                  m_bRunning;
   
    //int m_iSeque[SEQUE_NUMBER][CAMERA_NUMBER];   //ÿ��ʱ���е������
    int m_iSequeNumber;   //��ȡʵʱ��ʱ����
    QString m_dateTime;

    bool m_bRun;
    string m_seq[SEQUE_NUMBER];//ʱ����

    bool m_bInitCfg;//д���Դ//
    QFile* m_csvFile;

};

#endif
