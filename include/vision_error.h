#ifndef    _VISION_ERROR_H_
#define    _VISION_ERROR_H_

#ifndef INVALID_VALUE
#define  INVALID_VALUE -1
#endif

enum EM_ERROR_VISION_RESULT
{
    VISION_FAILURE  =-1,
	VISION_SUCCESS  = 0,
	VISION_INVALID_PARAM,
	VISION_TRANSLATEROI_FAILURE,	    // ROI模板转换失败
	VISION_READ_INNER_PARAM_FAILURE,	// 内部参数读取失败
	VISION_SAVE_INNER_PARAM_FAILURE,	// 内部参数保存失败
	VISION_EXEC_FAILURE,				// 执行失败
	VISION_JOB_NOT_EXIST,               // Job 不存在
	VISION_JOB_INIT_FAIL,               // Job 初始化失败
	VISION_JOB_GET_RESULT_FAIL,         // Job 获取结果失败
	VISION_GLOBAL_IO_PARAM_INVALID,     // 全局IO参数实例获取失败
    VISION_ACTION_TIME_OUT,             // 操作超时
    VISION_ACTION_RUNNING,              // 操作执行中

	/* 每个模块100 个错误code */
	
    //VISION_FIND_CIRCLE                            = 200, //找圆
    //VISION_FIND_LINE                              = 300, //找线
    //VISION_CUSTOM_HALCON                          = 400, //自定义halcon过程
    //VISION_AFFINETRANSPOINT                       = 500, //多点标定加载（vsaffinetranspoint）
    //VISION_VSCALIBMULTIPOINT                      = 600, //多点标定（vscalibmultipoint）
    //VISION_VSCALIBRATIOFACTORY                    = 700, //比例标定(vscalibratiofactory)
    //VISION_VSDATAASSEMBLED                        = 800, //数据拼装(vsdataassembled)
    //VISION_VSMODELSHAPEFACTORY                    = 900, //模板匹配(vsmodelshapefactory)
    //VISION_VSRESULTCOUNTFACTORY                   = 1000,//结果计算(vsresultcountfactory)
    //VISION_JOBCAMERA                              = 1100,//图片采集(jobcamera)
    //VISION_JOBIMAGEFILE                           = 1200,//文件加载(jobimagefile)
    //VISION_VSIMAGESHOW                            = 1300,//图片显示(vsimageshow)
    //VISION_VSGENCONTOURPOLYGON                    = 1400,//多边形拟合(vsgencontourpolygon)
    //VISION_CIRCLEFITTINGITEM                      = 1500,//圆形拟合(circlefittingitem)
    //VISION_LINEFITTINGITEM                        = 1600,//直线拟合(linefittingitem)
    //VISION_DISTANCELLJOBITEM                      = 1700,//测量两条线距离(distancelljobitem)
    //VISION_DISTANCEPPJOBITEM                      = 1800,//测量两点距离(distanceppjobitem)
    //VISION_SAVEDATA                               = 1900,//数据保存(vssavedatatofile)
    //VISION_SAVEIMAGE                              = 2000,//图片保存(vssaveimagetofile)
    //VISION_VSFINDRECTCENTER                       = 2100,//找矩形中心(vsfindrectcenter)
    //VISION_VSFINDINTERSECTIONLL                   = 2200,//找十字叉交点(vsfindintersectionll)
    //VISION_VSDATAINTERACTIVE                      = 2300,//数据交互（vsdatainteractive）
    //VISION_VSFINDDATACODE2D                       = 2400,//查找二维码（vsfinddatacode2d）
    //VISION_VSFINDIRREGULARSHAPE                   = 2500,//查找不规则图形重心（vsfindirregularshape）
    //VISION_COMM_UNDEFINE_CMD                      = 2600,//通信错误信息
    //VISION_VSGETMULTIPOINTFACTORY                 = 2700,//查找多点（vsgetmultipointfactory）
    //VISION_VSGET9POINTSFACTORY                    = 2800,//查找9点（vsget9pointsfactory）
    //VISION_VSGETCENTERBY3POINTS                   = 2900,//3点定园（vsgetcenterby3points）
    //VISION_DISTANCEPLJOBITEM                      = 3000,//测量点线距离(distanceppjobitem)
    //VISION_INTERSECTIONCCJOBITEM                  = 3100,//交叉工具（intersectionccjobitem）
    //VISION_INTERSECTIONLCJOBITEM                  = 3200,//交叉工具（intersectionlcjobitem）
    //VISION_INTERSECTIONLEJOBITEM                  = 3300,//交叉工具（intersectionlejobitem）
    //VISION_INTERSECTIONLLJOBITEM                  = 3400,//交叉工具（intersectionlljobitem）
    //VISION_FITCIRCLEPP                            = 3500,//两点拟合圆（fitcircleppjobitem）
    //VISION_VSDATAFORMATITEM                       = 3600,//数据格式化(vsdataformatitem)
    //VISION_IMAGE_AREA_SIZE_TOO_B_ERROR            = 3700,//图像处理(阈值面积查找)
    //VISION_VSDATAINPUTITEM_INPUTPARAM_NULL_ERROR  = 3800,//数据输入
    //VISION_VSDATAOUTPUTITEM_INPUTPARAM_NULL_ERROR = 3900,//数据输出
	//VISION_VISIONDEBUG                            = 4000,//调试模块
    //VISION_GENCOORDINATESJOBITEM                  = 4200,//点和基准线拟合坐标系(gencoordinatesjobitem)
    //VISION_CAMERAMANAGER                          = 4300,//相机管理(cameramanager)

    VISION_USER_ERROR = 50000,                                                   //自定义错误起始位置
};


#endif     //_VISION_ERROR_H_
