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
    // 切换产品重新写入配方与电流
    void   Reinit();
	
	bool   Crlmove(int imove = 0);
	bool   JobMoveForClearMode();
private:
    WorkFlowFun(void);
    ~WorkFlowFun(void);

public:
    void registerMainFlowCallBackFun();

    static int callFunMoveForCheck(void* parent, string param);//设滚轮移动

    static int callFunMoveJobForCheck(void* parent, string param);      //设滚轮连续运行
    static int callFunSetRun(void* parent, string param);               //设运行标志 run 

    static int callFunUSleep(void* parent, string param);                //调用深度学习接口

    //流水线采集图像接口
    static int callFunPieceByPiecework(void* parent, string param);        //采集流程   

	//流水线采集图像接口//
	//
    static int callFunWriteEleAndSqe(void* parent, string param);////写电流&& 配方
   
	//
	static int callFunResetCam(void* parent, string param);     //复位相机
    static int callFunResetLight(void* parent, string param);   //清空光源控制器计数

	static int callFunCheckInProductOnTime(void* parent, string param);// 入料指示灯操作//

	// 测试自动触发(按行数)
	static int callFunAutoTriggerByLines(void* parent, string param);

	static int callFunDeepLearnEnv(void* parent, string param);//H算法初始化

public:
    static int callFunSetCamerCfg(void* parent, string param); //设相机参数

	static int queryBackPcStatus(int& iStatus);							//查询下台PC的状态  0 查询成功 1 未使能前后道讯  2.连接失败，3 查询失败
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
    //int m_iCameraOn[CAMERA_NUMBER];       // 1111//对应的4位4个相机使用
    bool                                  m_bRunning;
   
    //int m_iSeque[SEQUE_NUMBER][CAMERA_NUMBER];   //每个时序中的相机数
    int m_iSequeNumber;   //获取实时的时序数
    QString m_dateTime;

    bool m_bRun;
    string m_seq[SEQUE_NUMBER];//时序名

    bool m_bInitCfg;//写入光源//
    QFile* m_csvFile;

};

#endif
