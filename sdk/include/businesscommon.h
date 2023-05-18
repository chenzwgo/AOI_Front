#ifndef BUSINISS_COMMON_H
#define BUSINISS_COMMON_H

#include <stdio.h>
#include <string>
#include "jtime.h"
#include "errcode_def.h"

using namespace std;

#define MAX_OPS_BUFF_LEN    256             //��������С
#define BUFF_CNT            20              //����������
#define STEPCIRCLETIME		200             //circle time

enum EM_SYS_WORK_STATE
{//ϵͳ״̬
    EM_SYS_NOINIT = 0,    //δ��ʼ��
    EM_SYS_INIT,          //��ʼ����
    EM_SYS_WAIT_RESET,    //�ȴ���λ
    EM_SYS_ESTOP,         //��ͣ
    EM_SYS_RESET,         //��λ��
    EM_SYS_RESET_FAILED,  //��λʧ��
    EM_SYS_STOP,          //ֹͣ��
    EM_SYS_READY,         //����
    EM_SYS_PAUSE,         //��ͣ
    EM_SYS_WORK,          //������
    EM_SYS_FINISH,        // �������
    EM_SYS_ERROR,         // �����쳣
    EM_SYS_SAFE_DOOR_OPEN, // ��ȫ�ű���
    EM_SYS_POWER_OFF,      // �ϵ�
    EM_SYS_LIMIT_ON,       // ��λ����
};

#define INVALID_VALUE         -1       //��Чֵ
//
#define CCD_DATA_MAX          14        //ccd������󷵻�14������
#define MAX_SN_LENGTH         23       //���SN�볤��
#define MAX_PIC_NAME_LENGTH   256      //���ͼƬ���ֳ���

//��վ��ʾ������
typedef enum E_POLIT_LIGHT
{
    GREEN_LIGHT = 0,        //�̵�
    YELLOW_LIGHT,           //�Ƶ�
    RED_LIGHT,              //���
    CLOSE_ALL_LIGHT,        //�ر�����
}PolitLight; 


enum SYSTEM_MSG_TYPE
{
    EVENT_USUALLY = 0,          //ͨ����Ϣ
    EVENT_SYS_STATUS,           //ϵͳ����״̬
    EVENT_WORK_FLOW_STATUS,     //��������״̬
    EVENT_RECHECK_DATA,         //��������
    EVENT_PILOT_MSG,            //��ʾ��Ϣ
    EVENT_ALARM_MSG             //������Ϣ
};


#define PDCA_PATH_MAX   256
#define PDCA_BUFFER_MAX 64
typedef struct _PdcaConfig
{
    char server[16];                         // ������IP
    int sport;                               // �������˿ں�
    char client[16];                         // ����IP
    int cport;                               // ���ض˿ں�
    char userName[PDCA_BUFFER_MAX];          // �û���
    char userKey[PDCA_BUFFER_MAX];           // ����
    char localDataPath[PDCA_PATH_MAX];       // ��������·��
    char localPicPath[PDCA_PATH_MAX];        // ����ͼƬ·��
    char serverDataPath[PDCA_PATH_MAX];      // ����������·��
    char serverPicPath[PDCA_PATH_MAX];       // ������ͼƬ·��
    char deviceName[PDCA_BUFFER_MAX];        // �豸��

    _PdcaConfig()
    {
        memset( this, 0, sizeof(_PdcaConfig) );
    }
}PdcaConfig;

// ���ڲ���
typedef struct _ComConfig
{    
    string name;
    int baudRate;   
    int parity;          /* Parity  0-4=None,Odd,Even,Mark,Space    */
    int byteSize;        /* ByteSize Number of bits/byte, 4-8       */
    int stopBits;        /* //ֹͣλ///* StopBits 1,3,2 = 1, 1.5, 2  */ 

    _ComConfig()
    {
        baudRate = 0;
        parity = 0;
        byteSize = 0;
        stopBits = 1;
    }
}ComConfig;

// ѹ������ز���
typedef struct _PressureConfig
{    
    char szName[16];
    int baudRate;   
    int parity;          /* Parity  0-4=None,Odd,Even,Mark,Space    */
    int byteSize;        /* ByteSize Number of bits/byte, 4-8       */
    int stopBits;        /* StopBits 0,1,2 = 1, 1.5, 2              */
    double upper;        /* ѹ������                                */
    double lower;        /* ѹ������                                */
    double pressure;     /* ѹ����ֵ                                */
    int stopTime;        /* ��ѹʱ��                                */

    _PressureConfig()
    {
        memset( this, 0, sizeof(_PressureConfig) );
    }
}PressureConfig;

// ���ϲ�������
typedef struct _SeatOffsetConfig
{
    double x;
    double y;
    double z;
    double ang;

    _SeatOffsetConfig()
    {
        memset( this, 0, sizeof(_SeatOffsetConfig) );
    }
}SeatOffsetConfig;

// �����׼
typedef struct _ReviewConfig
{
    double xUp;         // x ����
    double xDown;       // x ����
    double yUp;         // y ����
    double yDown;       // y ����
    double angUp;       // �Ƕ� ����
    double angDown;     // �Ƕ� ����

    _ReviewConfig()
    {
        memset( this, 0, sizeof(_ReviewConfig) );
    }
}ReviewConfig;

//CCD�������ݽṹ
typedef struct ST_CCD_DATA
{
    double data[CCD_DATA_MAX];      //��������
    bool bResult;
    char szSN[64];
    ST_CCD_DATA()
    {
        memset(data, 0,sizeof(data));
        memset(szSN, 0,sizeof(szSN));
        bResult = false;
    }

    ST_CCD_DATA operator &=(ST_CCD_DATA copy)
    {
        for (int i = 0; i < CCD_DATA_MAX; i++)
        {
            data[i] = copy.data[i];
        }
        bResult = copy.bResult;
        strncpy(szSN, copy.szSN, strlen(copy.szSN));
        return *this;
    }

}CCDData;

//�������ݽṹ
struct  RecheckWorkData
{
    double data[CCD_DATA_MAX];
};

typedef struct ST_WORK_TIME
{
    uint64 StartTime;
    uint64 EndTime;

	ST_WORK_TIME()
	{
		StartTime = 0;
		EndTime = 0;
	}

    ST_WORK_TIME &operator=(ST_WORK_TIME copy)
    {
        StartTime = copy.StartTime;
        EndTime = copy.EndTime;
        return *this;
    }
}stWorkTime;

typedef struct ST_WORK_PRODUCE_TIME
{
    JTime StartTime;
    JTime EndTime;

    ST_WORK_PRODUCE_TIME &operator=(ST_WORK_PRODUCE_TIME copy)
    {
        StartTime = copy.StartTime;
        EndTime = copy.EndTime;
        return *this;
    }
}stWorkProduceTime;

//�������ݽṹ
typedef struct WORK_DATA
{
    CCDData SuckCatchAlignData;     //�����������λȡ������
    CCDData SuckFixAlignData;       //�����������λ��������
    CCDData PFAlignData;            //����Ĥ��λ����
    CCDData RecheckData;            //��������
    CCDData CheckAppearanceData;    //��ۼ������
    CCDData HSGAlignData;           //HSG�������
    char szResStr[64];				//NG PASS ��� û�м�⵽����
    stWorkTime WorkTime;            //����ʱ���¼
    char SN[MAX_SN_LENGTH];         //BarCode��ά��
    char FGWIP_SN[MAX_SN_LENGTH];   //PDCA���ص���
    char PicName[MAX_PIC_NAME_LENGTH];  //ͼƬ����
    double dLoadCell;               //���������ϱ�ѹֵ
	double dPressureValue;          //��ѹѹ��ֵ
    int    Color;                   //CG������ɫ
    uint64 CircleTime;              //CTʱ��
    bool Result;                    //������
	int Tossing;					// ��һ��HSG���˼���Ĥ

	WORK_DATA()
	{
		memset(szResStr, 0, sizeof(szResStr));
		memset(SN, 0, sizeof(SN));
		memset(FGWIP_SN, 0, sizeof(FGWIP_SN));
		memset(PicName, 0, sizeof(PicName));
		dLoadCell = 0.0;
		dPressureValue = 0.0;
		Color = 0;
		CircleTime = 0;
		Result = false;
		Tossing = 0;
	}

	WORK_DATA &operator=(WORK_DATA copy)
	{
		SuckCatchAlignData = copy.SuckCatchAlignData;
		SuckFixAlignData = copy.SuckFixAlignData;
		PFAlignData = copy.PFAlignData;
		RecheckData = copy.RecheckData;
        CheckAppearanceData = copy.CheckAppearanceData;
		HSGAlignData = copy.HSGAlignData;
		WorkTime = copy.WorkTime;

		strncpy(szResStr, copy.szResStr, strlen(szResStr));
		strncpy(SN, copy.SN, strlen(SN));
		strncpy(FGWIP_SN, copy.FGWIP_SN, strlen(FGWIP_SN));
		strncpy(PicName, copy.PicName, strlen(PicName));
		dLoadCell = copy.dLoadCell;
		dPressureValue = copy.dPressureValue;
		Color = copy.Color;
		CircleTime = copy.CircleTime;
		Result = copy.Result;
		Tossing = copy.Tossing;
		return *this;
	}
}WorkData;
//��ɨ�������
typedef struct _CAMERA_PARAM
{
    bool    bSaveSourceImage;    //�Ƿ񱣴�ԭʼͼ��
    bool    bEableStaticImage;   //�Ƿ�ʹ�ܾ�̬У׼
    bool    bEableDynamic;       //�Ƿ�ʹ�ܶ�̬У׼
    bool    bEableFilterImage;   //�Ƿ�ʹ���˲�
    bool    bSaveCorrectImage;   //�Ƿ񱣴�У׼ͼ��
	double	dMoveUnit;			 // ���ؾ���(mm)
	double  dGetImageLen;        //��Ұ����(mm)
	double	dPluseWidgh;	     // ������������(ms)
	double	dFstartpos;		     // ��λ��ʱ����(mm)
	int     Fivscount;           //����ͼ��ɼ�����
	int     iSequence;           //ʱ����
	int     iFiv;         //������������  
    int  MoveLenth;           //��С�ɼ�ʱ��---
	double  dSpeed;              //�ƶ��ٶ�
	
	_CAMERA_PARAM()
	{
        bSaveSourceImage = true;
        bEableStaticImage = true;
        bEableDynamic = true;
        bEableFilterImage = true;
        bSaveCorrectImage = true;
		dMoveUnit    = 0.01;
		dGetImageLen = 10;
		dPluseWidgh  =0.001;
		dFstartpos   =0;	
		Fivscount    = 1000;   
		iSequence    = 1;   
		MoveLenth    = 500;   
		dSpeed       = 20;
		iFiv = 0;

	}

	_CAMERA_PARAM& operator=(const _CAMERA_PARAM &other)
	{
		if (this == &other)
		{
			return *this;
		}

        bSaveSourceImage  = other.bSaveSourceImage;
        bEableStaticImage = other.bEableStaticImage;
        bEableDynamic     = other.bEableDynamic;
        bEableFilterImage = other.bEableFilterImage;
        bSaveCorrectImage = other.bSaveCorrectImage;
		dMoveUnit   = other.dMoveUnit;
		dPluseWidgh = other.dPluseWidgh;
		dFstartpos= other.dFstartpos;
		dGetImageLen = other.dGetImageLen;
		Fivscount= other.Fivscount;
        MoveLenth = other.MoveLenth;
		dSpeed = other.dSpeed;
		iFiv = other.iFiv;
		iSequence = other.iSequence;
		return *this;
    }
    bool operator==(const _CAMERA_PARAM &other)
    {
        if (bSaveSourceImage != other.bSaveSourceImage
            || bEableStaticImage != other.bEableStaticImage
            || bEableDynamic     != other.bEableDynamic
            || bEableFilterImage != other.bEableFilterImage
            || bSaveCorrectImage != other.bSaveCorrectImage
            || dMoveUnit   != other.dMoveUnit
            || dPluseWidgh != other.dPluseWidgh
            || dFstartpos != other.dFstartpos
			|| iFiv != other.iFiv
            || dGetImageLen != other.dGetImageLen
            || Fivscount != other.Fivscount
            || MoveLenth != other.MoveLenth
            || dSpeed != other.dSpeed
			||iSequence != other.iSequence)
        {
            return false;
        }
        return true;
    }
}CameraParam;
#endif // BUSINISS_COMMON_H

