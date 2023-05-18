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

// 系统信号
typedef enum _SYS_STATE
{
	SYS_IDLE = 0,		     // 等待状态
	SYS_START,			     // 单启动	
	SYS_RESET,			     // 复位
	SYS_ESTOP,			     // 急停
	SYS_STOP,                // 停止
	SYS_ESTOP_LIMITS,	     // 因负限位而急停
	SYS_SAFE_DOOR,			 // 安全门开启状态
	SYS_DEVICE_POWER,		 // 设备断电
	SYS_PAUSE,				 // 暂停
	SYS_CONTINUE			 // 继续
}E_SYS_STATE;

// 主控状态
typedef enum _MB_WORK_STATE
{
	MAIN_WORK_INIT,       // 初始状态
	MAIN_WORK_WAIT_RESET, // 等待复位
	MAIN_WORK_RESETING,   // 复位中
	MAIN_WORK_RESET_FAILED, //复位失败
	MAIN_WORK_ESTOP,      // 急停
	MAIN_WORK_SAFEDOOR,   // 安全门被打开
	MAIN_WORK_POWEROFF,   // 掉电	
	MAIN_WORK_READY,      // 就绪
	MAIN_WORK_WORK,       // 工作中
	MAIN_WORK_PAUSE,      // 暂停
	MAIN_WORK_STOP,       // 停止(不断电)
    MAIN_WORK_CARD_UNCONNECT,//卡连接失败
	MAIN_WORK_ERROR,      // 出现异常
	MAIN_WORK_AXISALARM,   // 轴报警
	MAIN_PLC_NO_READY,		 // PLC ready
	MAIN_CLEAR_MODE		 // 清料模式

}E_WORK_STATE;
#define  WORK_STATE_SIZE    15
static const char* szEWorkStateCn[WORK_STATE_SIZE] = { "初始状态","等待复位","复位中","复位失败", "急停",
                                                       "安全门被打开","掉电","就绪","工作中","暂停", 
	                                                   "停止", "卡连接失败" ,"出现异常","轴报警", "PLC ready" };//
#define JSON_FIRST_KEY	"Silkprint"
#define JSON_TIMEOUT_SWITCH_KEY	"bStart_timer"
#define JSON_TIMEOUT_DELAYTIME_KEY	"iDelaytime"
//按照深视的配置文件定义
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
	DeepLearnNoNeed, //不启用算法
	DeepLearnInit,    //初始化
	DeepLearnInitOK,  //初始化完成
	DeepLearnIintError,//初始化失败
} InitDeepLearnState; //H算法初始化状态

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
    int img_blockCnt;       //整张图片分成的块数
    int img_blockRow;       //每一块数据的行数
    int img_blockIndex;     //已接受到的数据块数
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
    Scene_First = 0, //t第一场景
    Scene_Second,     //第二
	Scene_Third,	//第三
	Scene_Size
}enScene;
struct ImageSrcStrut
{
	unsigned char* imageAddr;//原图地址
	string  strChannel;     //通道
	int      iImageWide;     //图像宽
	int      iImageHight;    //高
	int      iCam;			 // 0 前端相机,1后端相机
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
    string strMark;  //标志
	string strChannel;//通道
    bool    bRect;  //画矩形//
    int     iMarkIndex; //标志索引
    int     iDefectType;//缺陷类型--用于标颜色
	int     iSize;      //大小
    double   dWide;   //缺陷宽
    double   dHigh;   //缺陷高
    double   posx;    //x位置
    double   posy;    //y位置
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
	bool bCradOnline;     //卡在线
    bool bCameraOnline[4];//相机在线
    bool bShangOK;        //上料机/下料机
    bool bXiaOK;        //上料机/下料机
	bool bClearMode;      //清料模式 
	bool bCleanMoving;    //清洗机运行
	bool bAoiIsConnected;	//另一条线AOI是否在线
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
#define  UPDATE_RES_TYPE_SIZE	    4		//结果分类个数-ABCD
//#define  DEFECT_TYPE_SIZE	29		//缺陷种类个数
#define  DEFECT_LEVEL_SIZE	8		//缺陷等级个数
#define  DEFECT_FLAG_SIZE	2		//
#define  DEFECT_SHAPE_SIZE	4		//
#define  DEFECT_REGION_SIZE	21		// 位置分成20个区域

static const char* g_strTableTitle = "通道,序列,等级,形状,Size,类型,区域,对比度,位置,灰度,得分,宽度,高度"; //缺陷列表头
static const char* g_strFlag[DEFECT_FLAG_SIZE] = {"边缘","视窗"};
static const char* g_ShapeFlag[DEFECT_SHAPE_SIZE] = { "未知","点" ,"线" ,"面" };
//

#define DEFECTLIST_FILE_NAME "defectlist.csv"

#define  DATE_TIMES	        12
#define  DATE_DATA_ALL	    25
static const char* szDay[2] = { "白班","夜班" };
static const char* szDayPath[3] = {"All","Day","Night" };//文件夹按白夜班分//
static const char* g_timeFlag[DATE_DATA_ALL] = { "0","1","2","3","4","5","6","7","8" ,"9","10","11","12","13","14","15","16","17","18","19","20",
                                                "21","22","23","0"};
#define  UPHCFG_SIZES	27		//总数，白班，夜班，24小时
#define  UPHINFO	   "UPHINFO"
#define  DAY_BY_TIME	 8   //转班时间
#define  DAY_BY_WORK_TIMES	 2   //一天 班次数 两班

#define  DAY_BY_WORK_INDEX	     1   //白班
#define  NIGHT_BY_WORK_INDEX	 2   //夜班

#define PRODUCT_NORMAL 0	//正常品
#define PRODUCT_RI	1	//RI品
//
// 缺陷类型
typedef enum defectType
{
	en_LightLeak = 1,			// 1 漏光
	en_ExcessLnk,				// 2 溢墨
	en_Discolouration,			// 3 异色
	en_SawEgdge,				// 4 锯齿
	en_Smirch,					// 5 脏污
	en_AccuInk,					// 6 油墨堆积
	en_DistHole,				// 7 孔变形
	en_MisPrint,				// 8 漏印
	en_Scratches,				// 9  刮伤
	en_Chips,					// 10 崩边
	en_Dent,					// 11 凹凸点	
    en_Impress,					// 12 压痕
    en_Ripples ,                 // 13 水波纹
    en_FalseSmirch,				// 14 模糊脏污
    en_FalseScratches,			// 15 模糊刮伤
    en_PolishingBadness,		// 16 扫光不良
    en_BrightLine,			    // 17 深划
	en_MarkNg,		            // 18 mark线不良
	en_ForeignMatter,			// 19 异物
	en_Hair,			        // 20毛发
	en_Dirtpits,			    // 21尘点
	en_DecomeDeformed,			// 22变形 =
	en_NarrowEdge,			    // 23窄边 =
	en_FalseChips,	//24模糊崩边						
	en_FalseDent,	//25模糊dent
	en_FalseLightLeak,	//26模糊漏光
	en_FalseDiscolouration,	//27模糊异色
	en_FalseBrightLine	//28模糊深划
	//en_Normal,		24正常		
	//en_Normal1,		25正常1	
	//en_Backup1,		26备份1
	//en_Backup2,		27备份2

} endefectType;
// #define  DEFECTTYPENUMBER_TYPE_SIZE	    17		//当前排序检测类型
#define  DEFECTTYPENUMBER_TYPE_SIZE	    22	

// 缺陷类型
typedef enum DVdefectType
{
	en_dv_ok = 0,				// 0 正常
	en_dv_Smirch,				// 1 脏污
	en_dv_hair,					// 2 毛发
	en_dv_dirtpits,				// 3 尘点
	en_dv_Scratches,			// 5 划痕
	en_dv_Chips,				// 6 崩边
	en_dv_dent,					// 7 dent凹凸点
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
	int gray_value; //灰度值
	int pixel_num;  //像素个数
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
	string strOriginate;		// bj算法缺陷字符名称
	string strChannelName;		// 通道 ,从哪个通道检测的
    string sn;

	int		iChannelType;		// iType 0 无效，1亮场 ，2 暗场， 3 穿透
	int		iCameraIndex;		// iCameraIndex 0 无效 1相机一，2相机二，3相机三，4相机四
	int     iMarkIndex;			// 标志索引
    int     iIndex;             // 显示索引
	int     iStation;           // 工站索引
	//BaseFeature  baseFeat;		//  BaseFeature
	//map<string, double*>		mapbaseFeat;	// 
	// 缺陷类型，1 漏光，2 溢墨 ，    3 异色 ，  4 锯齿
	//           5 脏污，6 油墨堆积， 7 孔变形 ，8 漏印
	//           9 刮伤，10 崩边，   11 凹凸点  ,12 压痕
    //           13 水波纹 14 模糊脏污  15 模糊刮伤 16 扫光不良 17 视窗白线
    //0，未定义
    int     iDefectType;
	// H算法缺陷类型, 0 正常 ,1 脏污 ，2 毛发，3 尘点， 4 划痕， 5 崩边，6 dent
	int     iDvDefectType;				// H 算法缺陷类型
	string  strDefectName;
	int     iSize;				// 缺陷Size
    int     defectWidth;        // 缺陷宽
    int     defectHeight;       // 缺陷长
	double  dmm_defectWidth;        // 缺陷宽
	double  dmm_defectHeight;       // 缺陷长
	double   dWide;				// 缺陷外接矩形宽
	double   dHigh;				// 缺陷外接矩形高
	double   posx;				// x位置
	double   posy;				// y位置
	double SkeletonLength;      // 缺陷长度
	double DefectBAvgGray;      // 平均灰度值
    double Circularity;         // 圆度
    double dSizeArea;           // 面积大小 （平方毫米）
	int iDefectLevel;			// 缺陷等级 （1 - 7 级）
    int iRealDefectLevel;       // 产品上实际缺陷等级
    float fscore;				// 缺陷分数
    float fdetect_score;				// 缺陷分数
    int contrast;				// 缺陷对比度
    int     iRealType;          // 玻璃上真实的类型，来判断是否检测错误
    int iSGray[SEQUE_NUMBER];       //单通道灰度值
    int iSAreaSize[SEQUE_NUMBER];   //单通道面积
    int iScontrast[SEQUE_NUMBER];   //单通道对比度
    int ivague_flag;                                     // 模糊标志位。只针对脏污划伤，0 不模糊 1 脏污划伤模糊
    int iposition;                                       // 缺陷在哪个区域  0 边缘区域 1 视窗区域 
	int idetect_flag;
	gHistogram histogram[GHISTOGRAM_MAX];								// 灰度直方图信息，单通道缺陷有效，对多合一的缺陷默认为0。
	int histogram_size;
	int ihistogramTotalCnt;         //缺陷灰度值 总数
	int iH_DefectLevel;			    // H_缺陷等级 （1 - 7 级）深视算法等级计算
	int iShape;                     // 缺陷形状 0->点，1->线 ，2->面
	double   absposx;				// 相对玻璃原点x位置
	double   absposy;				// 相对玻璃原点y位置
	int		 regionPos;				// 缺陷的区域位置
	string strRegion;			    // 缺陷名称
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
//缺陷判定
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
	res_recheck,//Aierror   算法或硬件错误
	res_OK,         //A  
    res_OK2,        //B 
    res_OK3,        //C RI
	res_Ng,         //D NG
	res_MaxNumber,  //缺陷过多
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
	// 缺陷类型，1 漏光，2 溢墨 ，    3 异色 ，  4 锯齿
	//           5 脏污，6 油墨堆积， 7 孔变形 ，8 漏印
	//           9 刮伤，10 崩边，   11 凹凸点   0，未定义

    int defectSummaryArr[DEFECT_MAX_SIZE];									// 每一种缺陷的个数,共 11 种缺陷
    //vector<int> defectSummaryVec;
	int defectLevelSummaryArr[DEFECT_LEVEL_SIZE];							// 每一种等级的个数,共 7个等级
    int defectSingleLevelCntArr[DEFECT_MAX_SIZE][DEFECT_LEVEL_SIZE];		// 每一种缺陷,每一种等级的个数.
    //vector<vector<int>> defectSingleLevelCntVec;
    int dvdefectSummaryArr[DEFECT_MAX_SIZE];								// H算法缺陷类型分类，每一种缺陷的个数
    //vector<int> dvdefectSummaryVec;
    double dvdefectSummaryAreaArr[DEFECT_MAX_SIZE];						// H算法缺陷类型分类，每一种缺陷的面积和
    //vector<int> dvdefectSummaryAreaVec;
	int  dvdefectRegionCnt[DEFECT_REGION_SIZE];								// 每个区域缺陷的个数
	int iMaxAearDefectType;   // 面积最大的缺陷类型
	int iProductNgDefectType;
	int iProductRiDefectType;
	int iDefectNumber;//缺陷个数
	int iCT;//产品测试时间 ms
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
	er_no_error = 0,					// 无误
	er_Instance_null,
	er_Grap,							// 采图
	er_Cutimging ,						// 切图        
	er_StaticShading,					// 静态
	er_DynamicShading,					// 动态
	er_SignalFilter,					// 滤波
	er_Threshold,						// 阈值分割
    er_ExtractingEdge,                  // 边缘提取
	er_multiChannel,					// 多通道提取
	er_DV ,								// H 算法
	er_DV_timeout,						// H 算法超时返回
	er_Inner,							// Inner
	er_EdgeDetect,						// EdgeDetect
	er_CornerDetection,					// CornerDetection ,						// 缺陷提取
	er_ListionHole,						// ListionHole
	er_Classification,					// Classification
	er_Blob,							// Blob
	er_saveFile,						// 保存结果File
	er_saveImage,						// 保存结果图片
	er_BUniteEF,						// B图像合并EF失败
	er_BEF_null,						// Bef图像为空
};

#define  MAX_DEFECT_CLASS_NUMBS    64

enum enInputImageType
{
	IMAGE_NORMAL = 0,     // 当前通道原始图片
	IMAGE_UNITE,          // 融合图片
	IMAGE_OTHER_CHANNEL,  // 其他通道的图片
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
    unsigned long log_flag;  //日志开关
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
 //   int refiner_numbs;  //句柄个数
    int class_num;   //缺陷分类种类个数--最大64-MAX_DEFECT_CLASS_NUMBS
 //   int filter_size;  //过滤检测区域小块大小
 //   int cut_image_index[2];  //记录用于切图图片的两个通道索引
    bool enable[MAX_DEFECT_CLASS_NUMBS];
    int defect_index[MAX_DEFECT_CLASS_NUMBS]; //缺陷
    int union_index;
    int imageNumbs;
	//int iDelaytime;
	bool bStarttimer;

    //int class_contrast[MAX_DEFECT_CLASS_NUMBS]; //缺陷分类各项类别的对比度要求
    //int class_contrast_d[MAX_DEFECT_CLASS_NUMBS]; //缺陷分类各项类别的对比度要求
    //int class_area_size[MAX_DEFECT_CLASS_NUMBS]; //缺陷分类各项类别的面积过滤要求
    //int class_area_size_d[MAX_DEFECT_CLASS_NUMBS]; //缺陷分类各项类别的面积过滤要求

    //int seq_background_gray[SEQUE_NUMBER]; //缺陷分类各项类别的面积过滤要求
    float defect_threshold;
    /*******************TBD end*******************/
	enInputImageType image_type[SEQUE_NUMBER];
	int image_index[SEQUE_NUMBER];
}stDvConfig;

//图片融合参数
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
/*** 等级划分结构体***/
#define  FILTER_TYPE_SIZE	3		//过滤种类个数
static const char* szFilterTypeEn[FILTER_TYPE_SIZE] = { "Area","Score","Contrast"};
static const char* szFilterTypeCn[FILTER_TYPE_SIZE] = { "面积","得分","对比度" };
// 过滤
typedef enum filterType
{
	en_Area = 0,			// 0 面积
	en_Score,			    // 1 得分
	en_Contrast,		    // 2 对比度
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
	bool   bEnable;       //使能
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
	bool   bEnable;       //使能
	string strLevel;      //等级1~7
	int    iLevel;		  //等级1~7	
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
	int iRegion;								// 缺陷在哪个区域  0 边缘区域 1 视窗区域 
	vector<stdefectlevel>* pDefectlevelVec;		//等级1~7	

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


// 多相机参数结构体
typedef struct  MutilCamCfg
{
	int iCamResolutionX;	// 相机分辨率X
	int iCamResolutionY;	// 相机分辨率Y
	int iCamCnt;			// 相机个数
	int iOverlapX;			// 多相机X方向重叠像素
	int iOverlapY;			// 多相机Y方向重叠像素
	int iImageWidth;		// 图像宽度
	int iImageHeight;		// 图像长度
	bool bEnableBlockGrap;	// 分块采图使能
	int iBlockLines;		// 分块采图,每块行数
	int iBlockOverlapLines;	// 分块采图,每行重叠行数
	int iBlockCnt;			// 分块采图,块数

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
    //vector<int> NgDefectTypeVec;//NG中对应各缺陷个数
	int NgDefectType[MAX_DEFECT_SIZE];
	int RiDefectType[MAX_DEFECT_SIZE];
	//
	unsigned int iDefectAvgCnt;   //缺陷平均数
	unsigned int iSmirchAvgCnt;   //脏污平均数
	unsigned int iHairAvgCnt;     //毛发平均数
	unsigned int iDirtpitsAvgCnt; //尘点平均数
	double       dSmirchAvgAera;     //脏污面积平均值
	int			iday;	//用于0 点时天的切换统计
	int         iAiError;//算法或硬件导致RI

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
	int idvSmirchCnt;				// 1 脏污 个数上限
	int idvHairCnt;					// 2 毛发 个数上限
	int idvDirtpitsCnt;				// 3 尘点 个数上限
	double ddvSmirchArea;			// 1 脏污 面积上限
	double ddvHairArea;				// 2 毛发 面积上限
	double ddvDirtArea;				// 3 尘点 面积上限
	DvToplimitCfg()
	{
		idvSmirchCnt = 0;				// 1 脏污 个数上限
		idvHairCnt =0;					// 2 毛发 个数上限
		idvDirtpitsCnt =0 ;				// 3 尘点 个数上限
		ddvSmirchArea = 0.0;			// 1 脏污 面积上限
		ddvHairArea = 0.0;				// 2 毛发 面积上限
		ddvDirtArea = 0.0;				// 3 尘点 面积上限
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

