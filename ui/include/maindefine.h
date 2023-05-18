#ifndef MAINUIDEFINE
#define MAINUIDEFINE

#include <stdio.h>
#include <string>
#include "jtime.h"
#include "../../dlapi/include/classification_interface.h"
#include "g12globalenv.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

#define Point_Path "Point.ini"
#define SETTING_FILE "Setting.ini"
#define  CONTROLCARD_NAME  "card"
#define  TARGET_STATION     "target"//
using namespace std;


typedef enum { Start, Reset, Stop, Pause} SignalType;
typedef enum { randomClose, randomTwoBin, randomThreeBin,randomOneBin} sendResultMode;
typedef enum { OKResult, RIResult, NGResult } resultForOneResult;
typedef enum { OKRIResult, OKNGResult, RINGResult } resultForTwoResult;

// ϵͳ�ź�
typedef enum _SYS_STATE
{
	SYS_IDLE = 0,		     // �ȴ�״̬
	SYS_START,			     // ������	
	SYS_RESET,			     // ��λ
	SYS_ESTOP,			     // ��ͣ
	SYS_STOP,                // ֹͣ
	SYS_ESTOP_LIMITS,	     // ����λ����ͣ
	SYS_SAFE_DOOR,			 // ��ȫ�ſ���״̬
	SYS_DEVICE_POWER,		 // �豸�ϵ�
	SYS_PAUSE,				 // ��ͣ
	SYS_CONTINUE			 // ����
}E_SYS_STATE;

// ����״̬
typedef enum _MB_WORK_STATE
{
	MAIN_WORK_INIT,       // ��ʼ״̬
	MAIN_WORK_WAIT_RESET, // �ȴ���λ
	MAIN_WORK_RESETING,   // ��λ��
	MAIN_WORK_RESET_FAILED, //��λʧ��
	MAIN_WORK_ESTOP,      // ��ͣ
	MAIN_WORK_SAFEDOOR,   // ��ȫ�ű���
	MAIN_WORK_POWEROFF,   // ����	
	MAIN_WORK_READY,      // ����
	MAIN_WORK_WORK,       // ������
	MAIN_WORK_PAUSE,      // ��ͣ
	MAIN_WORK_STOP,       // ֹͣ(���ϵ�)
    MAIN_WORK_CARD_UNCONNECT,//������ʧ��
	MAIN_WORK_ERROR,      // �����쳣
	MAIN_WORK_AXISALARM,   // �ᱨ��
	MAIN_PLC_NO_READY,		 // PLC ready
	MAIN_CLEAR_MODE		 // ����ģʽ

}E_WORK_STATE;
#define  WORK_STATE_SIZE    15
static const char* szEWorkStateCn[WORK_STATE_SIZE] = { "��ʼ״̬","�ȴ���λ","��λ��","��λʧ��", "��ͣ",
                                                       "��ȫ�ű���","����","����","������","��ͣ", 
	                                                   "ֹͣ", "������ʧ��" ,"�����쳣","�ᱨ��", "PLC ready" };//
#define JSON_FIRST_KEY	"Silkprint"
#define JSON_TIMEOUT_SWITCH_KEY	"bStart_timer"
#define JSON_TIMEOUT_DELAYTIME_KEY	"iDelaytime"
//�������ӵ������ļ�����
#define JSON_CONFIG_FOR_FRONT	0
#define JSON_CONFIG_FOR_BACK_DOWN	1
#define JSON_CONFIG_FOR_BACK_UP	2

#define DEFECT_MAX_SIZE  50

typedef enum
{
	productUntest, 
	productTesting, 
	productPass,
	productNg
} enProductState;

typedef enum
{
	DeepLearnNoNeed, //�������㷨
	DeepLearnInit,    //��ʼ��
	DeepLearnInitOK,  //��ʼ�����
	DeepLearnIintError,//��ʼ��ʧ��
} InitDeepLearnState; //H�㷨��ʼ��״̬

struct RadioSpeed
{
    double radioSpeed;
    RadioSpeed() {radioSpeed = 10;}
    RadioSpeed& operator=(const RadioSpeed &other)
    {
        if (this != &other)
        {
            radioSpeed = other.radioSpeed;
        }

        return *this;
    }
};


struct stDLConfig
{
    string symbol_file ;
    string params_file ;
    int batch_size ;
    int model_width ;
    int model_height;
    int model_channels ;
    int device_type;//1 -> cpu 2 ->gpu
    int device_id  ;

    stDLConfig()
    {
        symbol_file = "./dlapi/sample-symbol.json";
        params_file = "./dlapi/sample-0114.params";
        batch_size = 16;
        model_width = 512;
        model_height = 512;
        model_channels = 1;
        device_type = 2;//1 -> cpu 2 ->gpu
        device_id = 0 ;
    }

};

typedef struct ResImage
{
    unsigned char *img_data; 
    int img_width; 
    int img_height;
    int img_channels;
	int img_offsetstart;
	int img_offsetend;
    int img_blockCnt;       //����ͼƬ�ֳɵĿ���
    int img_blockRow;       //ÿһ�����ݵ�����
    int img_blockIndex;     //�ѽ��ܵ������ݿ���
    ResImage()
    {
        img_data = NULL;
        img_width= 0;
        img_height = 0;
        img_channels = 1;
		img_offsetstart = 0;
		img_offsetend = 0;
        img_blockIndex= 0;
        img_blockCnt = 0;
        img_blockRow = 0;
    }

    ResImage& operator=(const ResImage& other)
    {
        if (&other == this)
        {
            return *this;
        }

        img_data = other.img_data; 
        img_width = other.img_width; 
        img_height = other.img_height;
		img_channels = other.img_channels;
		img_offsetstart = other.img_offsetstart;
		img_offsetend = other.img_offsetend;
        img_blockIndex = other.img_blockIndex;
        img_blockCnt = other.img_blockCnt;
        img_blockRow = other.img_blockRow;
        return *this;
    }
    bool isInValid()
    {
        return img_data == NULL;
    }
    void resetMem()
    {
        if (img_data != NULL)
        {
            delete img_data;
            img_data = NULL;
        }
        img_width= 0;
        img_height = 0;
		img_offsetstart = 0;
		img_offsetend = 0;
        img_channels = 1;
        img_blockIndex = 0;
        img_blockCnt = 0;
        img_blockRow = 0;
    }

	void clearMem()
	{
		if (img_data != NULL)
		{
			memset(img_data, 0, img_width*img_height);
		}
        img_blockIndex = 0;
	}
}stResImage;

struct stDVHandles
{
    RefineScoreHandle refiner[8];
    ClassifierHandle heat_handle;
    ClassifierHandle defect_handle;
    ClassifierHandle heat_handle_d;
    ClassifierHandle defect_handle_d;
    dv::DefectResultInfo defectResult[10] ;
    dv::DefectControler defectController[2];
	dv::DeformationInfo deformationInfo;
	//FilterHandle filterHandle;

    stDVHandles()
    {
		heat_handle = NULL;
		defect_handle = NULL;
        heat_handle_d = NULL;
        defect_handle_d = NULL;
        //filterHandle = NULL;
        memset(refiner, 0, sizeof(refiner));
        memset(defectResult, 0, sizeof(defectResult));
		memset(&deformationInfo, 0, sizeof(deformationInfo));
    }

};
typedef enum
{
    Scene_First = 0, //t��һ����
    Scene_Second,     //�ڶ�
	Scene_Third,	//����
	Scene_Size
}enScene;
struct ImageSrcStrut
{
	unsigned char* imageAddr;//ԭͼ��ַ
	string  strChannel;     //ͨ��
	int      iImageWide;     //ͼ���
	int      iImageHight;    //��
	int      iCam;			 // 0 ǰ�����,1������
	ImageSrcStrut()
	{
		imageAddr   = NULL;
		strChannel  = "";
		iImageWide  = 0;
		iImageHight = 0;
		iCam        = 0;
	}
};

struct MarkStrut
{
    string strMark;  //��־
	string strChannel;//ͨ��
    bool    bRect;  //������//
    int     iMarkIndex; //��־����
    int     iDefectType;//ȱ������--���ڱ���ɫ
	int     iSize;      //��С
    double   dWide;   //ȱ�ݿ�
    double   dHigh;   //ȱ�ݸ�
    double   posx;    //xλ��
    double   posy;    //yλ��
    MarkStrut()
    {
        strMark = "";
		strChannel = "";
        bRect = false;
        iMarkIndex = 0;
        iDefectType = 0;
        dWide = 1;
        dHigh = 1;
		iSize = 1;
        posx= 0.0;
        posy= 0.0;
    }
};
struct UiStatusStrut
{
	bool bCradOnline;     //������
    bool bCameraOnline[4];//�������
    bool bShangOK;        //���ϻ�/���ϻ�
    bool bXiaOK;        //���ϻ�/���ϻ�
	bool bClearMode;      //����ģʽ 
	bool bCleanMoving;    //��ϴ������
	bool bAoiIsConnected;	//��һ����AOI�Ƿ�����
	UiStatusStrut()
	{
		bCradOnline = false;
		memset(bCameraOnline, false, sizeof(bCameraOnline));
        bShangOK       = false;
        bXiaOK     = false;
		bClearMode   = false;
		bCleanMoving = false;
		bool bAoiIsConnected = false;
	}
};


typedef enum
{
    update_number, 
    update_getimage, 
    update_getiamgetime,
    update_deal,
    update_dealtime,
    update_reset
} enImageState;
#define  UPDATE_RES_TYPE_SIZE	    4		//����������-ABCD
//#define  DEFECT_TYPE_SIZE	29		//ȱ���������
#define  DEFECT_LEVEL_SIZE	8		//ȱ�ݵȼ�����
#define  DEFECT_FLAG_SIZE	2		//
#define  DEFECT_SHAPE_SIZE	4		//
#define  DEFECT_REGION_SIZE	21		// λ�÷ֳ�20������

static const char* g_strTableTitle = "ͨ��,����,�ȼ�,��״,Size,����,����,�Աȶ�,λ��,�Ҷ�,�÷�,���,�߶�"; //ȱ���б�ͷ
static const char* g_strFlag[DEFECT_FLAG_SIZE] = {"��Ե","�Ӵ�"};
static const char* g_ShapeFlag[DEFECT_SHAPE_SIZE] = { "δ֪","��" ,"��" ,"��" };
//

#define DEFECTLIST_FILE_NAME "defectlist.csv"

#define  DATE_TIMES	        12
#define  DATE_DATA_ALL	    25
static const char* szDay[2] = { "�װ�","ҹ��" };
static const char* szDayPath[3] = {"All","Day","Night" };//�ļ��а���ҹ���//
static const char* g_timeFlag[DATE_DATA_ALL] = { "0","1","2","3","4","5","6","7","8" ,"9","10","11","12","13","14","15","16","17","18","19","20",
                                                "21","22","23","0"};
#define  UPHCFG_SIZES	27		//�������װ࣬ҹ�࣬24Сʱ
#define  UPHINFO	   "UPHINFO"
#define  DAY_BY_TIME	 8   //ת��ʱ��
#define  DAY_BY_WORK_TIMES	 2   //һ�� ����� ����

#define  DAY_BY_WORK_INDEX	     1   //�װ�
#define  NIGHT_BY_WORK_INDEX	 2   //ҹ��

#define PRODUCT_NORMAL 0	//����Ʒ
#define PRODUCT_RI	1	//RIƷ
//
// ȱ������
typedef enum defectType
{
	en_LightLeak = 1,			// 1 ©��
	en_ExcessLnk,				// 2 ��ī
	en_Discolouration,			// 3 ��ɫ
	en_SawEgdge,				// 4 ���
	en_Smirch,					// 5 ����
	en_AccuInk,					// 6 ��ī�ѻ�
	en_DistHole,				// 7 �ױ���
	en_MisPrint,				// 8 ©ӡ
	en_Scratches,				// 9  ����
	en_Chips,					// 10 ����
	en_Dent,					// 11 ��͹��	
    en_Impress,					// 12 ѹ��
    en_Ripples ,                 // 13 ˮ����
    en_FalseSmirch,				// 14 ģ������
    en_FalseScratches,			// 15 ģ������
    en_PolishingBadness,		// 16 ɨ�ⲻ��
    en_BrightLine,			    // 17 �
	en_MarkNg,		            // 18 mark�߲���
	en_ForeignMatter,			// 19 ����
	en_Hair,			        // 20ë��
	en_Dirtpits,			    // 21����
	en_DecomeDeformed,			// 22���� =
	en_NarrowEdge,			    // 23խ�� =
	en_FalseChips,	//24ģ������						
	en_FalseDent,	//25ģ��dent
	en_FalseLightLeak,	//26ģ��©��
	en_FalseDiscolouration,	//27ģ����ɫ
	en_FalseBrightLine	//28ģ���
	//en_Normal,		24����		
	//en_Normal1,		25����1	
	//en_Backup1,		26����1
	//en_Backup2,		27����2

} endefectType;
// #define  DEFECTTYPENUMBER_TYPE_SIZE	    17		//��ǰ����������
#define  DEFECTTYPENUMBER_TYPE_SIZE	    22	

// ȱ������
typedef enum DVdefectType
{
	en_dv_ok = 0,				// 0 ����
	en_dv_Smirch,				// 1 ����
	en_dv_hair,					// 2 ë��
	en_dv_dirtpits,				// 3 ����
	en_dv_Scratches,			// 5 ����
	en_dv_Chips,				// 6 ����
	en_dv_dent,					// 7 dent��͹��
} enDVdefectType;

typedef enum HeatMapShowType
{
	heatmap_ok = 0,
	heatmap_Smirch,
	heatmap_hair,
	heatmap_dirtpits,
	heatmap_Scratches,
	heatmap_Chips,
	heatmap_Dent,
	heatmap_Impress,
	heatmap_LightLeak,
	heatmap_ExcessLnk,
	heatmap_Discolouration,
	heatmap_BrightLine,
} enHeatMapShowType;

typedef enum DefectLevel
{
	en_level_0 = 0,		
	en_level_1,			
	en_level_2,			
	en_level_3,			
	en_level_4,			
	en_level_5,			
	en_level_6,
	en_level_7,
} enDefectLevel;
#define  GHISTOGRAM_MAX  256
typedef struct gHistogram {
	int gray_value; //�Ҷ�ֵ
	int pixel_num;  //���ظ���
	gHistogram()
	{
		gray_value = 0;
		pixel_num = 0;
	}
	gHistogram(const gHistogram& other)
	{
		gray_value = other.gray_value;
		pixel_num  = other.pixel_num;
	}
	gHistogram& operator=(const gHistogram& other)
	{
		gray_value = other.gray_value;
		pixel_num = other.pixel_num;
		return *this;
	}
};
struct stDefectFeature
{
	string strOriginate;		// bj�㷨ȱ���ַ�����
	string strChannelName;		// ͨ�� ,���ĸ�ͨ������
    string sn;

	int		iChannelType;		// iType 0 ��Ч��1���� ��2 ������ 3 ��͸
	int		iCameraIndex;		// iCameraIndex 0 ��Ч 1���һ��2�������3�������4�����
	int     iMarkIndex;			// ��־����
    int     iIndex;             // ��ʾ����
	int     iStation;           // ��վ����
	//BaseFeature  baseFeat;		//  BaseFeature
	//map<string, double*>		mapbaseFeat;	// 
	// ȱ�����ͣ�1 ©�⣬2 ��ī ��    3 ��ɫ ��  4 ���
	//           5 ���ۣ�6 ��ī�ѻ��� 7 �ױ��� ��8 ©ӡ
	//           9 ���ˣ�10 ���ߣ�   11 ��͹��  ,12 ѹ��
    //           13 ˮ���� 14 ģ������  15 ģ������ 16 ɨ�ⲻ�� 17 �Ӵ�����
    //0��δ����
    int     iDefectType;
	// H�㷨ȱ������, 0 ���� ,1 ���� ��2 ë����3 ���㣬 4 ���ۣ� 5 ���ߣ�6 dent
	int     iDvDefectType;				// H �㷨ȱ������
	string  strDefectName;
	int     iSize;				// ȱ��Size
    int     defectWidth;        // ȱ�ݿ�
    int     defectHeight;       // ȱ�ݳ�
	double  dmm_defectWidth;        // ȱ�ݿ�
	double  dmm_defectHeight;       // ȱ�ݳ�
	double   dWide;				// ȱ����Ӿ��ο�
	double   dHigh;				// ȱ����Ӿ��θ�
	double   posx;				// xλ��
	double   posy;				// yλ��
	double SkeletonLength;      // ȱ�ݳ���
	double DefectBAvgGray;      // ƽ���Ҷ�ֵ
    double Circularity;         // Բ��
    double dSizeArea;           // �����С ��ƽ�����ף�
	int iDefectLevel;			// ȱ�ݵȼ� ��1 - 7 ����
    int iRealDefectLevel;       // ��Ʒ��ʵ��ȱ�ݵȼ�
    float fscore;				// ȱ�ݷ���
    float fdetect_score;				// ȱ�ݷ���
    int contrast;				// ȱ�ݶԱȶ�
    int     iRealType;          // ��������ʵ�����ͣ����ж��Ƿ������
    int iSGray[SEQUE_NUMBER];       //��ͨ���Ҷ�ֵ
    int iSAreaSize[SEQUE_NUMBER];   //��ͨ�����
    int iScontrast[SEQUE_NUMBER];   //��ͨ���Աȶ�
    int ivague_flag;                                     // ģ����־λ��ֻ������ۻ��ˣ�0 ��ģ�� 1 ���ۻ���ģ��
    int iposition;                                       // ȱ�����ĸ�����  0 ��Ե���� 1 �Ӵ����� 
	int idetect_flag;
	gHistogram histogram[GHISTOGRAM_MAX];								// �Ҷ�ֱ��ͼ��Ϣ����ͨ��ȱ����Ч���Զ��һ��ȱ��Ĭ��Ϊ0��
	int histogram_size;
	int ihistogramTotalCnt;         //ȱ�ݻҶ�ֵ ����
	int iH_DefectLevel;			    // H_ȱ�ݵȼ� ��1 - 7 ���������㷨�ȼ�����
	int iShape;                     // ȱ����״ 0->�㣬1->�� ��2->��
	double   absposx;				// ��Բ���ԭ��xλ��
	double   absposy;				// ��Բ���ԭ��yλ��
	int		 regionPos;				// ȱ�ݵ�����λ��
	string strRegion;			    // ȱ������
    stDefectFeature(const stDefectFeature& other)
    {
        strOriginate = other.strOriginate;
        strChannelName = other.strChannelName;
        sn = other.sn;
        iChannelType = other.iChannelType;
        iMarkIndex = other.iMarkIndex;
        iIndex = other.iIndex;
		iStation = other.iStation;
        iCameraIndex = other.iCameraIndex;
		iDefectType = other.iDefectType;
		iDvDefectType = other.iDvDefectType;
		strDefectName = other.strDefectName;
        defectWidth = other.defectWidth;
        defectHeight = other.defectHeight;
		dmm_defectWidth = other.dmm_defectWidth;
		dmm_defectHeight = other.dmm_defectHeight;
        dWide = other.dWide;
        dHigh = other.dHigh;
        iSize = other.iSize;
        posx = other.posx;
        posy = other.posy;
		SkeletonLength = other.SkeletonLength;
		DefectBAvgGray = other.DefectBAvgGray;
        Circularity = other.Circularity;
        dSizeArea = other.dSizeArea;
		iDefectLevel = other.iDefectLevel;
        fscore = other.fscore;
        fdetect_score = other.fdetect_score;
        contrast = other.contrast;
        iRealType = other.iRealType;
        ivague_flag = other.ivague_flag;
        iposition = other.iposition;
		idetect_flag = other.idetect_flag;
		histogram_size = other.histogram_size;;
		ihistogramTotalCnt = other.ihistogramTotalCnt;
		iH_DefectLevel = other.iH_DefectLevel;
		iShape = other.iShape;
		absposx = other.absposx;
		absposy = other.absposy;
		regionPos = other.regionPos;
		
        for (int i = 0; i < SEQUE_NUMBER; i++)
        {
            iSGray[i] = other.iSGray[i];
            iSAreaSize[i] = other.iSAreaSize[i];
            iScontrast[i] = other.iScontrast[i];
        }
		for (int i = 0; i < GHISTOGRAM_MAX; i++)
		{
			histogram[i] = other.histogram[i];
		}
    }
	stDefectFeature()
	{
		strOriginate = "";
		strChannelName = "";
        sn = "";
		iChannelType = 0;
		iMarkIndex = 0;
        iIndex = 0;
		iStation = 0;
		iCameraIndex = 0;
		iDefectType = 0;
		iDvDefectType = 0;
        strDefectName = "";
        defectWidth = 1;
        defectHeight = 1;
		dmm_defectWidth = 0.0126;
		dmm_defectHeight = 0.0126;
		dWide = 1;
		dHigh = 1;
		iSize = 1;
		posx = 0.0;
		posy = 0.0;
		SkeletonLength = 0;
		DefectBAvgGray = 0;
        Circularity = 0; 
        dSizeArea = 1;
        iDefectLevel = 0;
        fscore = 0;
        fdetect_score = 0;
        contrast = 0;
        iRealType = 0;
        memset(iSGray, 0, sizeof(iSGray));
        memset(iSAreaSize, 0, sizeof(iSAreaSize));
        memset(iScontrast, 0, sizeof(iScontrast));
        ivague_flag = 0;
        iposition = 0;
		idetect_flag = -1;
		histogram_size = 0;
		ihistogramTotalCnt = 0;
		iH_DefectLevel = 0;
		iShape = -1;
		absposx = 0;
		absposy = 0;
		regionPos =0;
	}
    stDefectFeature& operator=(const stDefectFeature& other)
    {
        strOriginate = other.strOriginate;
        strChannelName = other.strChannelName;
        sn = other.sn;
        iChannelType = other.iChannelType;
        iMarkIndex = other.iMarkIndex;
        iIndex = other.iIndex;
		iStation = other.iStation;
        iCameraIndex = other.iCameraIndex;
		iDefectType = other.iDefectType;
		iDvDefectType = other.iDvDefectType;
        strDefectName = other.strDefectName;
        defectWidth = other.defectWidth;
        defectHeight = other.defectHeight;
		dmm_defectWidth = other.dmm_defectWidth;
		dmm_defectHeight = other.dmm_defectHeight;
        dWide = other.dWide;
        dHigh = other.dHigh;
        iSize = other.iSize;
        posx = other.posx;
        posy = other.posy;
		SkeletonLength = other.SkeletonLength;
		DefectBAvgGray = other.DefectBAvgGray;
        Circularity = other.Circularity;
        dSizeArea = other.dSizeArea;
        iDefectLevel = other.iDefectLevel;
        fscore = other.fscore;
        fdetect_score = other.fdetect_score;
        contrast = other.contrast;
        iRealType = other.iRealType;
        ivague_flag = other.ivague_flag;
        iposition = other.iposition;
		idetect_flag = other.idetect_flag;
		histogram_size = other.histogram_size;
		ihistogramTotalCnt = other.ihistogramTotalCnt;
		iH_DefectLevel = other.iH_DefectLevel;
		iShape = other.iShape;
		absposx = other.absposx;
		absposy = other.absposy;
		regionPos = other.regionPos;;
		
        for (int i = 0; i < SEQUE_NUMBER; i++)
        {
            iSGray[i] = other.iSGray[i];
            iSAreaSize[i] = other.iSAreaSize[i];
            iScontrast[i] = other.iScontrast[i];
        }
		for (int i = 0; i < GHISTOGRAM_MAX; i++)
		{
			histogram[i] = other.histogram[i];
		}
        return *this;
    }
};

struct  stEnableConfig
{
	stEnableConfig()
	{
        bImageDV = false;
        bImageSYDV = false;
		bNeedCutImage = false;
		bInner = false;
		bEdgeDetect = false;
		bCornerDetect = false;
		bListionHole = false;
		bClassification = false;
		bStaticShading = false;
		bDynamicShading = false;
		bSignalFilter = false;
		bThreshold = false;
		bSaveRawImge = false;
		bSaveDvErRawImge = false;
		bSaveCutcImage = false;
		bSaveStaticImage = false;
		bSaveDynImage = false;
		bSaveFiltImage = false;
		bSavebThresholdImage = false;
		bNeedCutSaveImage = false;
		bNeedCutSaveImage = false;
		bSaveResultFile = false;
		bDefectLevel = false;
        bDvbUnitEF = false;
        bMeasrue = false;
	}

    bool bImageDV;
    bool bImageSYDV;
    bool bDvbUnitEF;
	bool bNeedCutImage;
	bool bInner ;
	bool bEdgeDetect ;
	bool bCornerDetect;
	bool bListionHole ;
	bool bClassification;
	bool bStaticShading ;
	bool bDynamicShading;
	bool bSignalFilter;
	bool bThreshold;

	bool bSaveRawImge ;
	bool bSaveDvErRawImge;
	bool bSaveCutcImage ;
	bool bSaveStaticImage ;
	bool bSaveDynImage;
	bool bSaveFiltImage ;
	bool bSavebThresholdImage;
	bool bNeedCutSaveImage;
	bool bBlob;

	bool bSaveResultFile ;
	bool bDefectLevel;
    bool bMeasrue;
	bool bFirstEnable;
	bool bSecondEnable;
};
//ȱ���ж�
struct stDefectSpecCfg
{
	//int    iType;
	bool   bEnable;
	double dAreaMax;
	double dLenMax;

	double dAreaFirstMin;
	double dAreaFirstMax;
	int    iAreaFirstNumber;
	double dLenFirstMin;
	double dLenFirstMax;
	int    iLenFirstNumber;

	double dAreaSecondMax;
	int    iAreaSecondNumber;
	double dLenSecondMax;
	int    iLenSecondNumber;

	stDefectSpecCfg()
	{
		//iType    = 0 ;
		bEnable = true;
		dAreaMax = 0;
		dLenMax  = 0;

		dAreaFirstMin    = 0;
		dAreaFirstMax    = 0;
		iAreaFirstNumber = 0;
		dLenFirstMin     = 0;
		dLenFirstMax     = 0;
		iLenFirstNumber  = 0;

		dAreaSecondMax    = 0;
		iAreaSecondNumber = 0;
		dLenSecondMax     = 0;
		iLenSecondNumber  = 0;
	}
	stDefectSpecCfg(const stDefectSpecCfg& other)
	{
		//iType = other.iType;
		bEnable = other.bEnable;
		dAreaMax = other.dAreaMax;
		dLenMax = other.dLenMax;

		dAreaFirstMin = other.dAreaFirstMin;
		dAreaFirstMax = other.dAreaFirstMax;
		iAreaFirstNumber = other.iAreaFirstNumber;
		dLenFirstMin = other.dLenFirstMin;
		dLenFirstMax = other.dLenFirstMax;
		iLenFirstNumber = other.iLenFirstNumber;

		dAreaSecondMax = other.dAreaSecondMax;
		iAreaSecondNumber = other.iAreaSecondNumber;
		dLenSecondMax = other.dLenSecondMax;
		iLenSecondNumber = other.iLenSecondNumber;
	}
	stDefectSpecCfg& operator=(const stDefectSpecCfg& other)
	{
		//iType    = other.iType;
		bEnable = other.bEnable;
		dAreaMax = other.dAreaMax;
		dLenMax  = other.dLenMax;

		dAreaFirstMin = other.dAreaFirstMin;
		dAreaFirstMax = other.dAreaFirstMax;
		iAreaFirstNumber = other.iAreaFirstNumber;
		dLenFirstMin = other.dLenFirstMin;
		dLenFirstMax = other.dLenFirstMax;
		iLenFirstNumber = other.iLenFirstNumber;

		dAreaSecondMax = other.dAreaSecondMax;
		iAreaSecondNumber = other.iAreaSecondNumber;
		dLenSecondMax = other.dLenSecondMax;
		iLenSecondNumber = other.iLenSecondNumber;

		return *this;
	}
};

typedef enum productResFlag
{
	res_unknow = -1,
	res_error = 0,
	res_recheck,//Aierror   �㷨��Ӳ������
	res_OK,         //A  
    res_OK2,        //B 
    res_OK3,        //C RI
	res_Ng,         //D NG
	res_MaxNumber,  //ȱ�ݹ���
}enResFlag;
struct FrontAoiResMsg
{
	long lsn;
	enResFlag FrontResFlag;
	unsigned long long ulStartTime;
	FrontAoiResMsg()
	{
		lsn = 0;
		FrontResFlag = res_OK;
		ulStartTime = 0;
	}
	FrontAoiResMsg& operator=(const FrontAoiResMsg &other)
	{
		if (this != &other)
		{
			lsn = other.lsn;
			FrontResFlag = other.FrontResFlag;
			ulStartTime = other.ulStartTime;
		}
		return *this;
	}
};
struct stProductSummary
{
	enResFlag ResultFlag;
    string sn;
	string str2Dcode;
	// ȱ�����ͣ�1 ©�⣬2 ��ī ��    3 ��ɫ ��  4 ���
	//           5 ���ۣ�6 ��ī�ѻ��� 7 �ױ��� ��8 ©ӡ
	//           9 ���ˣ�10 ���ߣ�   11 ��͹��   0��δ����

    int defectSummaryArr[DEFECT_MAX_SIZE];									// ÿһ��ȱ�ݵĸ���,�� 11 ��ȱ��
    //vector<int> defectSummaryVec;
	int defectLevelSummaryArr[DEFECT_LEVEL_SIZE];							// ÿһ�ֵȼ��ĸ���,�� 7���ȼ�
    int defectSingleLevelCntArr[DEFECT_MAX_SIZE][DEFECT_LEVEL_SIZE];		// ÿһ��ȱ��,ÿһ�ֵȼ��ĸ���.
    //vector<vector<int>> defectSingleLevelCntVec;
    int dvdefectSummaryArr[DEFECT_MAX_SIZE];								// H�㷨ȱ�����ͷ��࣬ÿһ��ȱ�ݵĸ���
    //vector<int> dvdefectSummaryVec;
    double dvdefectSummaryAreaArr[DEFECT_MAX_SIZE];						// H�㷨ȱ�����ͷ��࣬ÿһ��ȱ�ݵ������
    //vector<int> dvdefectSummaryAreaVec;
	int  dvdefectRegionCnt[DEFECT_REGION_SIZE];								// ÿ������ȱ�ݵĸ���
	int iMaxAearDefectType;   // �������ȱ������
	int iProductNgDefectType;
	int iProductRiDefectType;
	int iDefectNumber;//ȱ�ݸ���
	int iCT;//��Ʒ����ʱ�� ms
	stProductSummary()
	{
        sn = "";
		str2Dcode = "";
		memset(defectSummaryArr, 0, sizeof(defectSummaryArr));
		memset(defectSingleLevelCntArr, 0, sizeof(defectSingleLevelCntArr));
		memset(dvdefectSummaryArr, 0, sizeof(dvdefectSummaryArr));
		memset(dvdefectSummaryAreaArr, 0, sizeof(dvdefectSummaryAreaArr));

        memset(defectLevelSummaryArr, 0, sizeof(defectLevelSummaryArr));
		memset(dvdefectRegionCnt, 0, sizeof(dvdefectRegionCnt));

		ResultFlag = res_unknow;
		iMaxAearDefectType = 0;
		iProductNgDefectType = 0;
		iProductRiDefectType = 0;
		iDefectNumber = 0;
		iCT = 0;
	};
    stProductSummary& operator=(const stProductSummary& other)
    {

        sn = other.sn;
		str2Dcode = other.str2Dcode;
		memcpy(defectSummaryArr, other.defectSummaryArr, sizeof(defectSummaryArr));
		memcpy(defectSingleLevelCntArr, other.defectSingleLevelCntArr, sizeof(defectSingleLevelCntArr));
		memcpy(dvdefectSummaryArr, other.dvdefectSummaryArr, sizeof(dvdefectSummaryArr));
		memcpy(dvdefectSummaryAreaArr, other.dvdefectSummaryAreaArr, sizeof(dvdefectSummaryAreaArr));

        memcpy(defectLevelSummaryArr, other.defectLevelSummaryArr, sizeof(defectLevelSummaryArr));
		memcpy(dvdefectRegionCnt, other.dvdefectRegionCnt, sizeof(dvdefectRegionCnt));

		ResultFlag = other.ResultFlag;
		iMaxAearDefectType = other.iMaxAearDefectType;
		iProductNgDefectType = other.iProductNgDefectType;
		iDefectNumber = other.iDefectNumber;
		iProductRiDefectType = other.iProductRiDefectType;
		iCT = other.iCT;
        return *this;
    }
	void init()
	{
		ResultFlag = res_unknow;
		iMaxAearDefectType = 0;
		iProductNgDefectType = 0;
		iProductRiDefectType = 0;
		iDefectNumber = 0;
		iCT = 0;
        sn = "";
		str2Dcode = "";
        memset(defectLevelSummaryArr, 0, sizeof(defectLevelSummaryArr));
		memset(dvdefectRegionCnt, 0, sizeof(dvdefectRegionCnt));
		memset(defectSummaryArr, 0, sizeof(defectSummaryArr));
		memset(defectSingleLevelCntArr, 0, sizeof(defectSingleLevelCntArr));
		memset(dvdefectSummaryArr, 0, sizeof(dvdefectSummaryArr));
		memset(dvdefectSummaryAreaArr, 0, sizeof(dvdefectSummaryAreaArr));
	};
};
struct stProductDataRes
{
	enResFlag ResultFlag;
	string sn;
	string str2Dcode;
	int iNgOrRiDefectType;
	stProductDataRes()
	{
		sn = "";
		str2Dcode = "";
		ResultFlag = res_unknow;
		iNgOrRiDefectType = 0;
	};
	stProductDataRes& operator=(const stProductDataRes& other)
	{

		sn = other.sn;
		str2Dcode = other.str2Dcode;
		ResultFlag = other.ResultFlag;
		iNgOrRiDefectType = other.iNgOrRiDefectType;
		return *this;
	}
	void init()
	{
		ResultFlag = res_unknow;
		iNgOrRiDefectType = 0;
		sn = "";
		str2Dcode = "";

	};
	bool IsOkFlag()
	{
		if (res_OK==ResultFlag|| res_OK2== ResultFlag)
		{
			return true;
		}
		return false;
	};
};
struct IniDefectSet
{
	double minArea;
	double maxArea;
	double minRect2DensityBrightPrepR;
	double GeometricLengthBrightPrepR;
	double maxCircleDensityBrightPrepR;
	double maxSub1stCircleDensityBrightPrepR;
    double GrayMeanGvBrightPrepB;
    double GrayMeanGvBrightPrepC;
    double GeometricRect2SlimnessBrightPrepR;
    double ListionHoleSize;
	IniDefectSet()
	{
		minArea = 0.0;
		maxArea = 0.0;
		minRect2DensityBrightPrepR        = 0.0;
		GeometricLengthBrightPrepR = 0.0;
		maxCircleDensityBrightPrepR		  = 0.0;
		maxSub1stCircleDensityBrightPrepR = 0.0;
        GrayMeanGvBrightPrepB = 0.0;
        GrayMeanGvBrightPrepC = 0.0;
        GeometricRect2SlimnessBrightPrepR = 0.0;
        ListionHoleSize = 0.0;
	}
};

enum eProcessErroeCode
{
	er_no_error = 0,					// ����
	er_Instance_null,
	er_Grap,							// ��ͼ
	er_Cutimging ,						// ��ͼ        
	er_StaticShading,					// ��̬
	er_DynamicShading,					// ��̬
	er_SignalFilter,					// �˲�
	er_Threshold,						// ��ֵ�ָ�
    er_ExtractingEdge,                  // ��Ե��ȡ
	er_multiChannel,					// ��ͨ����ȡ
	er_DV ,								// H �㷨
	er_DV_timeout,						// H �㷨��ʱ����
	er_Inner,							// Inner
	er_EdgeDetect,						// EdgeDetect
	er_CornerDetection,					// CornerDetection ,						// ȱ����ȡ
	er_ListionHole,						// ListionHole
	er_Classification,					// Classification
	er_Blob,							// Blob
	er_saveFile,						// ������File
	er_saveImage,						// ������ͼƬ
	er_BUniteEF,						// Bͼ��ϲ�EFʧ��
	er_BEF_null,						// Befͼ��Ϊ��
};

#define  MAX_DEFECT_CLASS_NUMBS    64

enum enInputImageType
{
	IMAGE_NORMAL = 0,     // ��ǰͨ��ԭʼͼƬ
	IMAGE_UNITE,          // �ں�ͼƬ
	IMAGE_OTHER_CHANNEL,  // ����ͨ����ͼƬ
};

typedef struct DvConfig
{
	DvConfig()
    {
        dv_config_path = "./hconfig/config.json";
		dv_product_path = "";
        log_flag = 0xF;
        defect_log_file_path = "./log";
        /*******************TBD*******************/

  //      heat_params_file_d = "./bojie/model/heat_d";
  //      defect_params_file_d = "./bojie/model/defect_d";

		//heat_batch_size = 64;
		//defect_batch_size = 64;
		//model_width = 256;
		//model_height = 256;
		//defect_model_width = 64;
		//defect_model_height = 64;
		//model_channels = 1;
		//defect_model_channels = 3;
		//device_type = 2;//1 -> cpu 2 ->gpu
		//device_id = 0;
		//timeout = 4000;
  //      refiner_numbs = 8;
        class_num = 6;
  //      defect_threshold = 3.0;
        union_index = 4;
        imageNumbs = 4;
		//iDelaytime = 6000;
		bStarttimer = true;
        memset(enable, 0, sizeof(enable));
        memset(defect_index, 0, sizeof(defect_index));
        memset(image_type, 0, sizeof(image_type));
        memset(image_index, 0, sizeof(image_index));

        //memset(class_contrast, 0, sizeof(class_contrast));
        //memset(class_contrast_d, 0, sizeof(class_contrast_d));
        //memset(class_area_size, 0, sizeof(class_area_size));
        //memset(class_area_size_d, 0, sizeof(class_area_size_d));

        //memset(seq_background_gray, 0, sizeof(seq_background_gray));
        /*******************TBD end*******************/
	}

    string dv_config_path;
	string dv_product_path;
    unsigned long log_flag;  //��־����
    string defect_log_file_path;
    /*******************TBD*******************/
 //   string defect_params_file_d;
	//int heat_batch_size ;
	//int defect_batch_size ;
	//int model_width ;
	//int model_height;
	//int defect_model_width ;
	//int defect_model_height ;
	//int model_channels;
	//int defect_model_channels ;
	//int device_type;//1 -> cpu 2 ->gpu
	//int device_id ;
	//int timeout;
 //   int refiner_numbs;  //�������
    int class_num;   //ȱ�ݷ����������--���64-MAX_DEFECT_CLASS_NUMBS
 //   int filter_size;  //���˼������С���С
 //   int cut_image_index[2];  //��¼������ͼͼƬ������ͨ������
    bool enable[MAX_DEFECT_CLASS_NUMBS];
    int defect_index[MAX_DEFECT_CLASS_NUMBS]; //ȱ��
    int union_index;
    int imageNumbs;
	//int iDelaytime;
	bool bStarttimer;

    //int class_contrast[MAX_DEFECT_CLASS_NUMBS]; //ȱ�ݷ���������ĶԱȶ�Ҫ��
    //int class_contrast_d[MAX_DEFECT_CLASS_NUMBS]; //ȱ�ݷ���������ĶԱȶ�Ҫ��
    //int class_area_size[MAX_DEFECT_CLASS_NUMBS]; //ȱ�ݷ�����������������Ҫ��
    //int class_area_size_d[MAX_DEFECT_CLASS_NUMBS]; //ȱ�ݷ�����������������Ҫ��

    //int seq_background_gray[SEQUE_NUMBER]; //ȱ�ݷ�����������������Ҫ��
    float defect_threshold;
    /*******************TBD end*******************/
	enInputImageType image_type[SEQUE_NUMBER];
	int image_index[SEQUE_NUMBER];
}stDvConfig;

//ͼƬ�ںϲ���
#define CATOUT_COUNTS   2
typedef struct CatoutConfig
{
    CatoutConfig()
    {
        memset(thresholdE, 0, sizeof(thresholdE));
        memset(thresholdF, 0, sizeof(thresholdF));
        memset(diliation1, 0, sizeof(diliation1));
        memset(diliation2, 0, sizeof(diliation2));
        memset(addValue, 0, sizeof(addValue));
    }
    int thresholdE[CATOUT_COUNTS];
    int thresholdF[CATOUT_COUNTS];
    int diliation1[CATOUT_COUNTS];
    int diliation2[CATOUT_COUNTS];
    int addValue[CATOUT_COUNTS];
}stCutOutConfig;

typedef struct InterpolationConfig
{
    InterpolationConfig()
    {
        thresholdMax = 0;
        thresholdMin = 0;
    }
    int thresholdMax;
    int thresholdMin;
}stInterpolationConfig;
/*** �ȼ����ֽṹ��***/
#define  FILTER_TYPE_SIZE	3		//�����������
static const char* szFilterTypeEn[FILTER_TYPE_SIZE] = { "Area","Score","Contrast"};
static const char* szFilterTypeCn[FILTER_TYPE_SIZE] = { "���","�÷�","�Աȶ�" };
// ����
typedef enum filterType
{
	en_Area = 0,			// 0 ���
	en_Score,			    // 1 �÷�
	en_Contrast,		    // 2 �Աȶ�
} enfilterType;

typedef struct VarExpression
{
	string varName;
	string varDes;
	string varExp;

	VarExpression()
	{
		varName = "";
		varDes = "";
		varExp = "";

	}

	VarExpression(const VarExpression& other)
	{
		varName = other.varName;
		varDes = other.varDes;
		varExp = other.varExp;
	}

	VarExpression& operator=(const VarExpression& other)
	{
		varName = other.varName;
		varDes = other.varDes;
		varExp = other.varExp;
		return *this;
	}

}stVarExpression;

typedef struct defectlevelItem
{
	string strItemExpression;   //
	bool   bEnable;       //ʹ��
	string strItemDes;
	string strDefectName;
	int iLevel;
	
	defectlevelItem()
	{
		strItemExpression = "";
		bEnable    = false;
		strItemDes = "";
		strDefectName = "";
		iLevel = 0;
	}
	defectlevelItem& operator=(const defectlevelItem& other)
	{
		strItemExpression = other.strItemExpression;
		bEnable    = other.bEnable;
		strItemDes = other.strItemDes;
		strDefectName = other.strDefectName;
		iLevel = other.iLevel;
		return *this;
	}
	void init()
	{
		strItemExpression = "";
		bEnable     = false;
		strItemDes = "";
	}

}stdefectlevelItem;
typedef struct defectlevel
{
	bool   bEnable;       //ʹ��
	string strLevel;      //�ȼ�1~7
	int    iLevel;		  //�ȼ�1~7	
	string strDefectName;
	vector<stdefectlevelItem> defectlevelItemVec;
	defectlevel()
	{
		bEnable = false;
		strLevel = "";
		strDefectName = "";
	}
	defectlevel& operator=(const defectlevel& other)
	{
		bEnable  = other.bEnable;
		strLevel = other.strLevel;
		defectlevelItemVec = other.defectlevelItemVec;
		strDefectName = other.strDefectName;
		return *this;
	}
}stdefectlevel;

typedef struct defectlevelRegion
{
	int iRegion;								// ȱ�����ĸ�����  0 ��Ե���� 1 �Ӵ����� 
	vector<stdefectlevel>* pDefectlevelVec;		//�ȼ�1~7	

	defectlevelRegion()
	{
		iRegion = 0;
		pDefectlevelVec = NULL;
	}

	defectlevelRegion(int iReg , vector<stdefectlevel>* pRegionlVec)
	{
		iRegion = iReg;
		pDefectlevelVec = pRegionlVec;
	}


	defectlevelRegion(const defectlevelRegion& other)
	{
		if (&other == this)
		{
			return;
		}
		iRegion = other.iRegion;
		pDefectlevelVec = other.pDefectlevelVec;
	}

	defectlevelRegion& operator=(const defectlevelRegion& other)
	{
		iRegion = other.iRegion;
		pDefectlevelVec = other.pDefectlevelVec;
	}

}stdefectlevelRegion;


typedef struct defectLevelEnableCfg
{
    //bool defectTypeEnable[DEFECT_TYPE_SIZE];
	bool defectLevel[DEFECT_LEVEL_SIZE];
	bool bOkEnable;
	bool bOk2Enable;
	bool bOk3Enable;
	bool bNgEnable;
	bool bEnableNgAbort;
	bool bEnableCntAbort;
	bool bEnableTimeout;
	int iAbortCnt;
	int iTimeout;
	defectLevelEnableCfg()
	{
        //memset(defectTypeEnable, 0, sizeof(defectTypeEnable));
		memset(defectLevel, 0, sizeof(defectLevel));
		bOkEnable = false;
		bOk2Enable = false;
		bOk3Enable = false;
		bNgEnable = false; 
		bEnableNgAbort = false;
		bEnableCntAbort = false;
		bEnableTimeout = false;
		iAbortCnt = 9999;
		iTimeout = 9999;
	}
}stdefectLevelEnableCfg;


// ����������ṹ��
typedef struct  MutilCamCfg
{
	int iCamResolutionX;	// ����ֱ���X
	int iCamResolutionY;	// ����ֱ���Y
	int iCamCnt;			// �������
	int iOverlapX;			// �����X�����ص�����
	int iOverlapY;			// �����Y�����ص�����
	int iImageWidth;		// ͼ����
	int iImageHeight;		// ͼ�񳤶�
	bool bEnableBlockGrap;	// �ֿ��ͼʹ��
	int iBlockLines;		// �ֿ��ͼ,ÿ������
	int iBlockOverlapLines;	// �ֿ��ͼ,ÿ���ص�����
	int iBlockCnt;			// �ֿ��ͼ,����

	MutilCamCfg()
	{
		iCamResolutionX = 8196;
		iCamResolutionY = 0;
		iCamCnt = 1;
		iOverlapX = 0;
		iOverlapY = 0;
		iImageWidth = 8196;
		iImageHeight = 0;
		bEnableBlockGrap = false;
		iBlockLines = 0;
		iBlockOverlapLines = 0;
		iBlockCnt = 0;

	}

	MutilCamCfg(const MutilCamCfg& other)
	{
		iCamResolutionX = other.iCamResolutionX;
		iCamResolutionY = other.iCamResolutionY;
		iCamCnt = other.iCamCnt;
		iOverlapX = other.iOverlapX;
		iOverlapY = other.iOverlapY;
		iImageWidth = other.iImageWidth;
		iImageHeight = other.iImageHeight;
		bEnableBlockGrap = other.bEnableBlockGrap;;
		iBlockLines = other.iBlockLines;;
		iBlockOverlapLines = other.iBlockOverlapLines;;
		iBlockCnt = other.iBlockCnt;;
	}

	const MutilCamCfg& operator=(const MutilCamCfg& other)
	{
		if (&other == this)
		{
			return *this;
		}
		iCamResolutionX = other.iCamResolutionX;
		iCamResolutionY = other.iCamResolutionY;
		iCamCnt = other.iCamCnt;
		iOverlapX = other.iOverlapX;
		iOverlapY = other.iOverlapY;
		iImageWidth = other.iImageWidth;
		iImageHeight = other.iImageHeight;
		bEnableBlockGrap = other.bEnableBlockGrap;;
		iBlockLines = other.iBlockLines;;
		iBlockOverlapLines = other.iBlockOverlapLines;;
		iBlockCnt = other.iBlockCnt;;
		return *this;
	}

}stMutilCamCfg;

typedef struct  ChannelCamCfg
{
	bool bCam1Enable;
	bool bCam2Enable;
	bool bCam3Enable;
	bool bCam4Enable;

	string strCam1Sn;
	string strCam2Sn;
	string strCam3Sn;
	string strCam4Sn;
	ChannelCamCfg()
	{
		bCam1Enable = false;
		bCam2Enable = false;
		bCam3Enable = false;
		bCam4Enable = false;
		strCam1Sn = "";
		strCam2Sn = "";
		strCam3Sn = "";
		strCam4Sn = "";
	}



	ChannelCamCfg(const ChannelCamCfg& other)
	{
		bCam1Enable = other.bCam1Enable;
		bCam2Enable = other.bCam2Enable;
		bCam3Enable = other.bCam3Enable;
		bCam4Enable = other.bCam4Enable;
		strCam1Sn = other.strCam1Sn;
		strCam2Sn = other.strCam1Sn;
		strCam3Sn = other.strCam1Sn;
		strCam4Sn = other.strCam1Sn;
	}

	const ChannelCamCfg& operator=(const ChannelCamCfg& other)
	{
		if (&other == this)
		{
			return *this;
		}
		bCam1Enable = other.bCam1Enable;
		bCam2Enable = other.bCam2Enable;
		bCam3Enable = other.bCam3Enable;
		bCam4Enable = other.bCam4Enable;
		strCam1Sn = other.strCam1Sn;
		strCam2Sn = other.strCam1Sn;
		strCam3Sn = other.strCam1Sn;
		strCam4Sn = other.strCam1Sn;

		return *this;
	}
};
#define MAX_DEFECT_SIZE 50
typedef struct UPHCfg
{
	unsigned int iTotalCnt;
	unsigned int iOKCnt;  //A -OK
	unsigned int iNGCnt;  //D- NG
	unsigned int iReCheck; // B
	unsigned int iReClean; // C
    //vector<int> NgDefectTypeVec;//NG�ж�Ӧ��ȱ�ݸ���
	int NgDefectType[MAX_DEFECT_SIZE];
	int RiDefectType[MAX_DEFECT_SIZE];
	//
	unsigned int iDefectAvgCnt;   //ȱ��ƽ����
	unsigned int iSmirchAvgCnt;   //����ƽ����
	unsigned int iHairAvgCnt;     //ë��ƽ����
	unsigned int iDirtpitsAvgCnt; //����ƽ����
	double       dSmirchAvgAera;     //�������ƽ��ֵ
	int			iday;	//����0 ��ʱ����л�ͳ��
	int         iAiError;//�㷨��Ӳ������RI

	UPHCfg()
	{
		iTotalCnt = 0;
		iOKCnt = 0;
		iNGCnt = 0;
		iReCheck = 0;
		iReClean = 0;
		iDefectAvgCnt   = 0;
		iSmirchAvgCnt   = 0;
		iHairAvgCnt     = 0;
		iDirtpitsAvgCnt = 0;
		dSmirchAvgAera  = 0.0;
		iAiError = 0;
		memset(NgDefectType, 0, sizeof(NgDefectType));
		memset(RiDefectType, 0, sizeof(RiDefectType));
	}
	UPHCfg& operator=(const UPHCfg& other)
	{

		iTotalCnt = other.iTotalCnt;
		iOKCnt    = other.iOKCnt;
		iNGCnt    = other.iNGCnt;
		iReCheck  = other.iReCheck;
		iReClean  = other.iReClean;
		iDefectAvgCnt = other.iDefectAvgCnt;
		iSmirchAvgCnt = other.iSmirchAvgCnt;
		iHairAvgCnt = other.iHairAvgCnt;
		iDirtpitsAvgCnt = other.iDirtpitsAvgCnt;
		dSmirchAvgAera = other.dSmirchAvgAera;
		iAiError = other.iAiError;
		memcpy(NgDefectType, other.NgDefectType, sizeof(NgDefectType));
		memcpy(RiDefectType, other.RiDefectType, sizeof(RiDefectType));
		return *this;
	}
	void clear()
	{
		iTotalCnt = 0;
		iOKCnt = 0;
		iNGCnt = 0;
		iReCheck = 0;
		iReClean = 0;
		iDefectAvgCnt = 0;
		iSmirchAvgCnt = 0;
		iHairAvgCnt = 0;
		iDirtpitsAvgCnt = 0;
        dSmirchAvgAera = 0.0;
		iAiError = 0;
		memset(NgDefectType, 0, sizeof(NgDefectType));
		memset(RiDefectType, 0, sizeof(RiDefectType));
	}
	~UPHCfg()
	{
		clear();
	}
}stUPHCfg;

#define  DVTOPLIMITCFG_SIZE		3
typedef struct DvToplimitCfg
{
	int idvSmirchCnt;				// 1 ���� ��������
	int idvHairCnt;					// 2 ë�� ��������
	int idvDirtpitsCnt;				// 3 ���� ��������
	double ddvSmirchArea;			// 1 ���� �������
	double ddvHairArea;				// 2 ë�� �������
	double ddvDirtArea;				// 3 ���� �������
	DvToplimitCfg()
	{
		idvSmirchCnt = 0;				// 1 ���� ��������
		idvHairCnt =0;					// 2 ë�� ��������
		idvDirtpitsCnt =0 ;				// 3 ���� ��������
		ddvSmirchArea = 0.0;			// 1 ���� �������
		ddvHairArea = 0.0;				// 2 ë�� �������
		ddvDirtArea = 0.0;				// 3 ���� �������
	}

	DvToplimitCfg(const DvToplimitCfg& other)
	{
		idvSmirchCnt = other.idvSmirchCnt;
		idvHairCnt = other.idvHairCnt;
		idvDirtpitsCnt = other.idvDirtpitsCnt;
		ddvSmirchArea = other.ddvSmirchArea;
		ddvHairArea = other.ddvHairArea;
		ddvDirtArea = other.ddvDirtArea;
	}

	const DvToplimitCfg& operator=(const DvToplimitCfg& other)
	{
		if (&other == this)
		{
			return *this;
		}

		idvSmirchCnt = other.idvSmirchCnt;
		idvHairCnt = other.idvHairCnt;
		idvDirtpitsCnt = other.idvDirtpitsCnt;
		ddvSmirchArea = other.ddvSmirchArea;
		ddvHairArea = other.ddvHairArea;
		ddvDirtArea = other.ddvDirtArea;

		return *this;
	}

}stDvToplimitCfg;

#endif // BUSNIESSCOMMON

