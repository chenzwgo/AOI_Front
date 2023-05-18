#ifndef   GLOBAL_ENV_H_
#define   GLOBAL_ENV_H_

#include <string.h>
#define  CAMERA_NUMBER  4     //相机最大数
#define  SEQUE_NUMBER   8     //时序最大数

//光源地址
#define  LIGHT_SV_ADDR_ON_A  2017
#define  LIGHT_SV_ADDR_ON_B  2018
#define  LIGHT_SV_ADDR_ON_C  2019
#define  LIGHT_SV_ADDR_ON_D  2020

#define  LIGHT_STROBOFLASH_ON_ADDR   2025   //使能频闪
#define LIGHT_CLEAR_ADDR   2026               //清计数
#define  LIGHT_MODE_ADDR   2027                //模式
#define LIGHT_MAIN_ADDR   2028                //上下光源
#define  LIGHT_SAVE_SET  2029

#define LIGHT_A_ON_ADDR   2021     //A开关
#define LIGHT_B_ON_ADDR   2022     //B开关
#define LIGHT_C_ON_ADDR   2023     //C开关
#define LIGHT_D_ON_ADDR   2024     //D开关

//上下光源
enum eModbusComMgr {
	UP_LIGHT_COM = 1,//上光源串口
	DOWN_LIGHT_COM,
    MODBUS_CARD,    // modbus控制运动
};

/******************************插件名称********************************/
#define PLUGIN_FILE_VARIABLE_NAME      "filevariable"   //配置保存插件名称
#define PLUGIN_NOTIFY_CENTER_NAME      "vssdkcommon"   //配置保存插件名称
#define PLUGIN_MANUAL_VALVE_UI_NAME    "dispensertoolui"     //手动点胶窗体插件名称
#define PLUGIN_MANUAL_VISION_UI_NAME    "dispensesetui"    //视觉参数窗体插件名称
#define PLUGIN_MANUAL_CAMERA_UI_NAME    "camerasettingui"    //相机设置窗体插件名称
#define PLUGIN_VISION_PROCESS_PLUGIN_NAME    "vssecondpackage"    //视觉处理插件名称
#define PLUGIN_MOTION_PLUGIN_NAME            "motion"    //运动控制处理插件名称
#define PLUGIN_STATION_PLUGIN_NAME            "station"    //工站插件名称
#define PLUGIN_DEVICES_PLUGIN_NAME            "devices"    //外设插件名称
#define PLUGIN_FILE_DISPENSE_NAME             "dispenseimp"     //点胶业务插件名称

// 保持全局参数信息
/********************* 全局参数文件名 **********************/
#define GLOBAL_FILE   "./products/globalenv.ini"    //全局参数文件名
#define MOTION_FILE   "./products/motion.ini"       //运动参数文件名
#define PRODUCT_INFO   "productinfo.ini"           //产品信息文件


/****************************** 静态校正配置项 ********************************/
#define STATICCORRECT_FILE          "staticcorrect.ini"           // 静态校准文件
#define STATICCORRECT_DISABLE       "staticdisable"
#define STATICCORRECT_GREY         "staticgrey"
#define STATICCORRECT_START_OFFSET  "startoffset"
#define STATICCORRECT_END_OFFSET    "endoffset"

/****************************** 动态校正配置项 ********************************/
#define DYNAMICCORRECT_FILE   "dynamiccorrect.ini"         // 动态校准文件

#define STANDARDGRAY            "standardGray"
#define MAXDEVIATION            "maxDeviation"
#define AFTERFREEZESPEED        "afterFreezeSpeed"
#define MINTHRES                "minThres"
#define MAXTHRES                "maxThres"
#define MAXSPEED                "maxSpeed"
#define SPEEDLEN               "SpeedLen"
#define EXPGARY                "expGary"
#define DYNDISENABLE            "dynDisenable"
#define EDGEHANDLEFLG            "edgeHandleFlg"
#define MINEDGELIMIT            "minEdgeLimit"

/****************************** SignalFilter 配置项 ********************************/
#define SIGNALFILTER_FILE   "signalfilter.ini"         // SignalFilter文件

#define GAIN            "gain"
#define MEAN            "mean"
#define SIGNALDISABLE   "signalDisable"

/****************************** Threshold 配置项 ********************************/
#define THRESHOLD_FILE   "Threshold.ini"         // Threshold 文件
#define  THRESHOLDENABLE                "threshold_enable"

#define  THRESHOLD_ROI_X1               "threshold_roi_x1"
#define  THRESHOLD_ROI_Y1               "threshold_roi_y1"
#define  THRESHOLD_ROI_X2               "threshold_roi_x2"
#define  THRESHOLD_ROI_Y2               "threshold_roi_y2"
#define  THRESHOLD_W                    "threshold_w"
#define  THRESHOLD_H                    "threshold_h"
#define  THRESHOLD_INNERMINTHRES        "threshold_innerminthres"
#define  THRESHOLD_INNERMINLEN          "threshold_innerminlen"
#define  THRESHOLD_STARTMINLEN          "threshold_startminlen"
#define  THRESHOLD_MINDEFECTGRAY        "threshold_mindefectgray"
#define  THRESHOLD_MAXDEFECTGRAY        "threshold_maxdefectgray"
#define  THRESHOLD_EDGEMAXTHRE          "threshold_edgemaxthre"
#define  THRESHOLD_EDGEMINLEN           "threshold_edgeminlen"


#define  CUTEDGE_ROI_X1                 "cutedge_roi_x1"
#define  CUTEDGE_ROI_Y1                 "cutedge_roi_y1"
#define  CUTEDGE_ROIWIDTH               "cutedge_roiwidth"
#define  CUTEDGE_ROIHEIGHT              "cutedge_roiheight"
#define  CUTEDGE_HORIZONTALWITH         "cutedge_horizontalwith"
#define  CUTEDGE_VERTICALWITH           "cutedge_verticalwith"
#define  CUTEDGE_LINEWIDTH				"cutedge_linewidth"
#define  CUTEDGE_CORNERWIDTH			"cutedge_cornerwidth"
#define  CUTEDGE_CORNERHEIGHT           "cutedge_cornerheight"


#define COMPOUND 								"Compound"
#define COMPOUND_ROIX1							"Compound_roix1"
#define COMPOUND_ROIY1							"Compound_roiy1"
#define COMPOUND_ROIX2							"Compound_roix2"
#define COMPOUND_ROIY2							"Compound_roiy2"
#define COMPOUND_BRIGHTTHREB					"Compound_brightThreB"
#define COMPOUND_BRIGHTTHREE					"Compound_brightThreE"
#define COMPOUND_BRIGHTTHREF					"Compound_brightThreF"
#define COMPOUND_BMINDEFECTGRAY					"Compound_bMinDefectGray"
#define COMPOUND_BMINNBELTGRADIENT				"Compound_bMinNbeltGradient"
#define COMPOUND_BMINOBELTGRADIENT				"Compound_bMinOBeltGradient"
#define COMPOUND_BBACKGROUNDGRAY				"Compound_bBackGroundGray"
#define COMPOUND_BEDGELEN						"Compound_bEdgeLen"
#define COMPOUND_BENDDEFECTGRAY					"Compound_bEndDefectGray"
#define COMPOUND_BDEFECTCLOSEW					"Compound_bDefectCloseW"
												
#define COMPOUND_EMINDEFECTGRAY					"Compound_eMinDefectGray"
#define COMPOUND_EMINNBELTGRADIENT				"Compound_eMinNbeltGradient"
#define COMPOUND_EMINOBELTGRADIENT				"Compound_eMinOBeltGradient"
#define COMPOUND_EBACKGROUNDGRAY				"Compound_eBackGroundGray"
#define COMPOUND_EEDGELEN						"Compound_eEdgeLen"
#define COMPOUND_EENDDEFECTGRAY					"Compound_eEndDefectGray"
#define COMPOUND_EDEFECTCLOSEW					"Compound_eDefectCloseW"
#define COMPOUND_EADDVAL						"Compound_eAddVal"
												
#define COMPOUND_FMINDEFECTGRAY					"Compound_fMinDefectGray"
#define COMPOUND_FMINNBELTGRADIENT				"Compound_fMinNbeltGradient"
#define COMPOUND_FMINOBELTGRADIENT				"Compound_fMinOBeltGradient"
#define COMPOUND_FBACKGROUNDGRAY				"Compound_fBackGroundGray"
#define COMPOUND_FEDGELEN						"Compound_fEdgeLen"
#define COMPOUND_FENDDEFECTGRAY					"Compound_fEndDefectGray"
#define COMPOUND_FDEFECTCLOSEW					"Compound_fDefectCloseW"
#define COMPOUND_FADDVAL						"Compound_fAddVal"
#define COMPOUND_MEAN							"Compound_mean"
#define COMPOUND_GAIN							"Compound_gain"
#define COMPOUND_MINDEFECTTHRESHOLD				"Compound_minDefectThreshold"
#define COMPOUND_TOPBOTTOMFLG					"Compound_topBottomFlg"



/****************************** 切图 配置项 ********************************/
#define  ACQUISITION_FILE	"acquisition.ini"
#define  ACQUISITION_CUTIMG_TYPE        "cut_type"
#define  ACQUISITION_CUTIMG_MINTHRE     "minThre"
#define  ACQUISITION_CUTIMG_MAXTHRE     "maxThre"
#define  ACQUISITION_CUTIMG_TOP         "Top"
#define  ACQUISITION_CUTIMG_BOTTOM      "Bottom"
#define  ACQUISITION_CUTIMG_LEFT        "Left"
#define  ACQUISITION_CUTIMG_RIGHT       "Right"

#define ACQUISITION_IMAGECONFIG			"ImageConfig"
#define ACQUISITION_CAM_RESOLUTIONX		"resolutionX"			// 相机分辨率X
#define ACQUISITION_CAM_RESOLUTIONY		"resolutionY"			// 相机分辨率Y
#define ACQUISITION_CAM_CNT				"CamCnt"				// 相机个数
#define ACQUISITION_OVERLAPX			"overlapX"				// 多相机X方向重叠像素
#define ACQUISITION_OVERLAPY			"overlapY"				// 多相机Y方向重叠像素
#define ACQUISITION_IMAGEWIDTH			"ImageWidth"			// 图像宽度
#define ACQUISITION_IMAGEHEIGHT			"ImageHeight"			// 图像长度

#define ACQUISITION_BLOCKENABLE			"BlockEnable"			// 分块采图使能
#define ACQUISITION_BLOCKLINES			"BlockLines"			// 分块采图,每块行数
#define ACQUISITION_BLOCKOVERLAPLINES	"BlockOverlapLines"		// 分块采图,每行重叠行数
#define ACQUISITION_BLOCKCNT			"BlockCnt"				// 分块采图,块数

#define  ACQUISITION_UPCAM1_ENABLE		"upcam1_enable"			// 多相机,UP相机1使能
#define  ACQUISITION_UPCAM1_SN			"upcam1_sn"				// 多相机,UP相机1对于相机SN
#define  ACQUISITION_UPCAM2_ENABLE		"upcam2_enable"			// 多相机,UP相机2使能
#define  ACQUISITION_UPCAM2_SN			"upcam2_sn"				// 多相机,UP相机2对于相机SN
#define  ACQUISITION_UPCAM3_ENABLE		"upcam3_enable"			// 多相机,UP相机3使能
#define  ACQUISITION_UPCAM3_SN			"upcam3_sn"				// 多相机,UP相机3对于相机SN
#define  ACQUISITION_UPCAM4_ENABLE		"upcam4_enable"			// 多相机,UP相机4使能
#define  ACQUISITION_UPCAM4_SN			"upcam4_sn"				// 多相机,UP相机4对于相机SN
#define  ACQUISITION_DOWNCAM1_ENABLE	"downcam1_enable"		// 多相机,Down相机1使能
#define  ACQUISITION_DOWNCAM1_SN		"downcam1_sn"			// 多相机,Down相机1对于相机SN
#define  ACQUISITION_DOWNCAM2_ENABLE	"downcam2_enable"		// 多相机,Down相机2使能
#define  ACQUISITION_DOWNCAM2_SN		"downcam2_sn"			// 多相机,Down相机2对于相机SN
#define  ACQUISITION_DOWNCAM3_ENABLE	"downcam3_enable"		// 多相机,Down相机3使能
#define  ACQUISITION_DOWNCAM3_SN		"downcam3_sn"			// 多相机,Down相机3对于相机SN
#define  ACQUISITION_DOWNCAM4_ENABLE	"downcam4_enable"		// 多相机,Down相机4使能
#define  ACQUISITION_DOWNCAM4_SN		"downcam4_sn"			// 多相机,Down相机4对于相机SN

/****************************** Inner 配置项 ********************************/
#define  INNER_ENABLE_CH                "enableInner"
#define  INNER_FILE                     "Inner.ini"
#define  INNER_INNERMINTHRES            "innerMinThres"
#define  INNER_INNERMAXTHRES            "innerMaxThres"
#define  INNER_INNERAREA                "innerArea"
#define  INNER_INNERAREAFLUCTUATE       "innerAreaFluctuate"
#define  INNER_MININNERRATE             "minInnerRate"
#define  INNER_DARKTHRES                "darkThres"
#define  INNER_DARKWIDTH                "darkWidth"
#define  INNER_BRIGHTTHRES              "brightThres"
#define  INNER_BRIGHTWIDTH              "brightWidth"
#define  INNER_CDEW                     "cdEW"
#define  INNER_MDEW                     "mdEW"
#define  INNER_MINAREA                  "minArea"
#define  INNER_MAXAREA                  "maxArea"
#define  INNER_CDCW                     "cdCW"
#define  INNER_MDCW                     "mdCW"
#define  INNER_CLOSEW                   "closeW"
#define  INNER_CLOSEH                   "closeH"

#define  INNER_DILARADIUS               "dilaRadius"
#define  INNER_RATEWH                   "rateWH"
#define  INNER_INNERCLOSINGW            "innerClosingW"
#define  INNER_ERROSIONW                "errosionW"

// double dilaRadius, double rateWH;
 //double innerClosingW, double errosionW,
/****************************** Edge 配置项 ********************************/
#define  EDGE_ENABLE_CH                "enableEdge"
#define  EDGE_LEFTROIX1				   "edge_leftroix1"
#define  EDGE_LEFTROIY1				   "edge_leftroiy1"
#define  EDGE_LEFTROIX2                "edge_leftroix2"
#define  EDGE_LEFTROIY2				   "edge_leftroiy2"

#define  EDGE_TOPROIX1				   "edge_toproix1"
#define  EDGE_TOPROIY1				   "edge_toproiy1"
#define  EDGE_TOPROIX2                 "edge_toproix2"
#define  EDGE_TOPROIY2				   "edge_toproiy2"

#define  EDGE_RIGHTROIX1				"edge_rightroix1"
#define  EDGE_RIGHTROIY1				"edge_rightroiy1"
#define  EDGE_RIGHTROIX2                "edge_rightroix2"
#define  EDGE_RIGHTROIY2				"edge_rightroiy2"

#define  EDGE_BOTTOMROIX1				"edge_bottomroix1"
#define  EDGE_BOTTOMROIY1				"edge_bottomroiy1"
#define  EDGE_BOTTOMROIX2               "edge_bottomroix2"
#define  EDGE_BOTTOMROIY2				"edge_bottomroiy2"

#define  EDGE_FITGAP					 "edge_fitgap"
#define  EDGE_SMOOTHLINEPARAM			 "edge_smoothlineparam"
#define  EDGE_SMOOTHCIRCULARPARAM        "edge_smoothcircularparam"
#define  EDGE_MINDEFECTAREA				 "edge_mindefectarea"
#define  EDGE_MINDEEP					 "edge_mindeep"
#define  EDGE_EDGEMINGRAY				 "edge_edgemingray"
#define  EDGE_EDGEMAXGRAY				 "edge_edgemaxgray"
#define  EDGE_MINDEFECTDIST				 "edge_mindefectdist"
#define  EDGE_OPENINGRADIUS				 "edge_openingradius"
#define  EDGE_DEFECTMINGRAY				 "edge_defectMinGray"
#define  EDGE_DEFECTMAXGRAY				 "edge_defectMaxGray"
#define  EDGE_CIRCULARRADIUS             "edge_circularRadius"

#define  LISTIONHOLE_ENABLE_CH                   "enableListionhole"
#define  LISTIONHOLE_LEFTROIX1				     "ListionHole_leftroix1"
#define  LISTIONHOLE_LEFTROIY1				     "ListionHole_leftroiy1"
#define  LISTIONHOLE_LEFTROIX2                   "ListionHole_leftroix2"
#define  LISTIONHOLE_LEFTROIY2				     "ListionHole_leftroiy2"

#define  LISTIONHOLE_FITGAP                      "ListionHole_fitgap"
#define  LISTIONHOLE_SMOOTHLINEPARAM			 "ListionHole_smoothlineparam"
#define  LISTIONHOLE_SMOOTHCIRCULARPARAM         "ListionHole_smoothcircularparam"
#define  LISTIONHOLE_MINDEFECTAREA				 "ListionHole_mindefectarea"
#define  LISTIONHOLE_MINDEEP					 "ListionHole_mindeep"
#define  LISTIONHOLE_EDGEMINGRAY				 "ListionHole_edgemingray"
#define  LISTIONHOLE_EDGEMAXGRAY				 "ListionHole_edgemaxgray"
#define  LISTIONHOLE_MINDEFECTDIST				 "ListionHole_mindefectdist"
#define  LISTIONHOLE_OPENINGRADIUS				 "ListionHole_openingradius"
#define  LISTIONHOLE_DEFECTMINGRAY				 "ListionHole_defectMinGray"
#define  LISTIONHOLE_DEFECTMAXGRAY				 "ListionHole_defectMaxGray"
#define  LISTIONHOLE_CIRCULARRADIUS             "ListionHole_circularRadius"

/******************************  Flawgeneration 配置项 ********************************/
#define  FLAWGENERATION_FILE                        "flawgeneration.ini"
#define  FLAWGENERATION_SCRATCHMINLENGTH            "scratchMinLength"
#define  FLAWGENERATION_SCRATCHTHIN                 "scratchThin"
#define  FLAWGENERATION_SCRATCHLENGTH               "scratchLength"
#define  FLAWGENERATION_SCRATCHINCIRCLEAREARATE     "scratchInCircleAreaRate"
#define  FLAWGENERATION_SCRATCHBFMAXAVGGRAY			"scratchBFMaxAvgGray"
#define  FLAWGENERATION_SCRATCHCFMINAVGGRAY			"scratchCFMinAvgGray"

#define  FLAWGENERATION_DUSTMAXMEANGRAY             "dustMaxMeanGray"
#define  FLAWGENERATION_DUSTMAXREGIONNUM            "dustMaxRegionNum"
#define  FLAWGENERATION_DUSTINCIRCLEAREARATE        "dustInCircleAreaRate"
#define  FLAWGENERATION_DUSTBFMINAVGGRAY			"dustBFMinAvgGray"
#define  FLAWGENERATION_DUSTCFMAXAVGGRAY			"dustCFMaxAvgGray"

/******************************  Blob cluster配置项 ********************************/
#define  BLOBCLUSTER_FILE                       "blobcluster.ini"
#define  BLOBCLUSTER_ENABLE                     "enable"
#define  BLOBCLUSTER_THRESH                     "thresh"
#define  BLOBCLUSTER_THRESHMAX                  "threshMax"
#define  BLOBCLUSTER_DILATIONCD 				"DilationCD"
#define  BLOBCLUSTER_DILATIONMD 				"DilationMD"
#define  BLOBCLUSTER_MAXBLOBLENGTH 				"MaxBlobLength"
#define  BLOBCLUSTER_MINBLOBCDLENGTH 			"minBlobCDLength"
#define  BLOBCLUSTER_MINBLOBMDLENGTH 			"minBlobMDLength"
#define  BLOBCLUSTER_MINBLOBAREA 				"minBlobArea"
#define  BLOBCLUSTER_MAXBLOBAREA 				"maxBlobArea"
#define  BLOBCLUSTER_MAXBLOBCLUSTERCDDISTANCE 	"maxBlobClusterCDDistance"
#define  BLOBCLUSTER_MAXBLOBCLUSTERMDDISTANCE 	"maxBlobClusterMDDistance"
#define  BLOBCLUSTER_MAXBLOBCLUSTERLENGTH       "maxBlobClusterLength"

/****************************** 深度学习 配置项 ********************************/
#define  DV_FILE				        "dvconfig.ini"
#define  DV_CONFIG_FILE                 "dvconfig" , "config_file"
#define  DV_HEAT_SYMBOL_FILE            "dvconfig" , "heat_symbol_file"
#define  DV_HEAT_PARAMS_FILE            "dvconfig" , "heat_params_file" 
#define  DV_DEFECT_SYMBOL_FILE          "dvconfig" , "defect_symbol_file"
#define  DV_DEFECT_PARAMS_FILE          "dvconfig" , "defect_params_file" 

#define  DV_HEAT_SYMBOL_D_FILE            "dvconfig" , "heat_symbol_d_file"
#define  DV_HEAT_PARAMS_D_FILE            "dvconfig" , "heat_params_d_file" 
#define  DV_DEFECT_SYMBOL_D_FILE          "dvconfig" , "defect_symbol_d_file"
#define  DV_DEFECT_PARAMS_D_FILE          "dvconfig" , "defect_params_d_file" 

#define  DV_HEAT_BATCH_SIZE             "dvconfig" , "heat_batch_size"
#define  DV_DEFECT_BATCH_SIZE           "dvconfig" , "defect_batch_size"
#define  DV_MODEL_WIDTH					"dvconfig" , "model_width"
#define  DV_MODEL_HEIGHT				"dvconfig" , "model_height"
#define  DV_DEFECT_MODEL_WIDTH          "dvconfig" , "defect_model_width"
#define  DV_DEFECT_MODEL_HEIGHT         "dvconfig" , "defect_model_height"
#define  DV_MODEL_CHANNELS				"dvconfig" , "model_channels"
#define  DV_DEFECT_MODEL_CHANNELS       "dvconfig" , "defect_model_channels"
#define  DV_DEVICE_TYPE					"dvconfig" , "device_type"
#define  DV_DEVICE_ID					"dvconfig" , "device_id"
#define  DV_TIMEOUT						"dvconfig" , "timeout"
#define  DV_REFINER_NUMBS				"dvconfig" , "refiner_numbs"
#define  DV_CLASS_NUM   				"dvconfig" , "class_num"
#define  DV_DEFECT_THRESHOLD   			"dvconfig" , "defect_threshold"
#define  DV_CONFIG_GROUP_NAME			"dvconfig"
#define  DV_ENABLE_KEY  				"enable"
#define  DV_ENABLE_0					"dvconfig" , "enable0"
#define  DV_ENABLE_1					"dvconfig" , "enable1"
#define  DV_ENABLE_2					"dvconfig" , "enable2"
#define  DV_ENABLE_3					"dvconfig" , "enable3"
#define  DV_ENABLE_4					"dvconfig" , "enable4"
#define  DV_ENABLE_5					"dvconfig" , "enable5"
#define  DV_DEFECT_INDEX_KEY    		"index"
#define  DV_CONFIG_UP_GROUP_NAME		"up"
#define  DV_CONFIG_DOWN_GROUP_NAME		"down"
#define  DV_CONTRAST_KEY  			    "contrast"
#define  DV_SIZE_KEY  	    		    "size"
#define  DV_HEAT_THRESHOLD				"dvconfig" , "heat_threshold"   //检测模型传入阈值
#define  DV_CUT_THRESHOLD				"dvconfig" , "cut_threshold"    //切图阈值
#define  DV_CUT_UP_INDEX				"dvconfig" , "cut_up_index"    //上相机切图通道索引
#define  DV_CUT_DOWN_INDEX				"dvconfig" , "cut_down_index"    //下相机切图通道索引
#define  DV_CLASSIFY_SIZE				"dvconfig" , "classify_size"    //过滤检测区域大小
#define  DV_H_LOG_FLAGS		    		"dvconfig" , "h_log_flags"     //log
#define  DV_H_LOG_PATH		    		"dvconfig" , "h_log_path"     //log
#define  DV_SHOW_ALL_CHANNEL			"dvconfig" , "show_all_channel"    //显示所有通道图片
#define  DV_UNION_CHANNEL_INDEX     	"dvconfig" , "union_index"    //通道图片

#define  DV_CONFIG_BACKGROUND_GROUP_NAME		"bcgray"
#define  DV_CONFIG_GRAY_VALUE                   "gray"

/**********************************光源时序配置项***********************************************/
#define ILLUMINATIONCONTROL_FILE      "illuminationControl.ini"
#define PROJADDR        "ProjAddr"       //址地位
#define PROJDADDR        "ProjDAddr"       //址地位
#define PRO_TYPE        "Pro_Type"       //类型     add 20180621
#define PRO_COMERAINDEX "Pro_ComeraIndex"//相机索引 add 20180621
#define ENABELCHANNLE   "EnabelChannle"  //通道使能标志 add 20180621
#define LIGHTPOWERA_F   "LightPowerA_F"  //亮度 - 上
#define LIGHTPOWERB_F   "LightPowerB_F"
#define LIGHTPOWERC_F   "LightPowerC_F"
#define LIGHTPOWERD_F   "LightPowerD_F"
#define LIGHTPOWERB_R   "LightPowerB_R"  // 下
#define LIGHTPOWERC_R   "LightPowerC_R"
#define LIGHTPOWERD_R   "LightPowerD_R"
#define DUTYCYCLEA_F    "DutyCycleA_F"  //脉宽-上
#define DUTYCYCLEB_F	"DutyCycleB_F"
#define DUTYCYCLEC_F	"DutyCycleC_F"
#define DUTYCYCLED_F	"DutyCycleD_F"
#define DUTYCYCLEA_R	"DutyCycleA_R" //下
#define DUTYCYCLEB_R	"DutyCycleB_R"
#define DUTYCYCLEC_R	"DutyCycleC_R"
#define DUTYCYCLED_R	"DutyCycleD_R"
#define LIGHTON_F       "LightOn_F"    //光源使能 上
#define LIGHTON_R		"LightOn_R"
#define CAMERAON_F      "CameraOn_F"  //相机使能 上
#define CAMERAON_R      "CameraOn_R"
#define DARKLINGHTMODEL "DarkLinghtModel"   //暗场光源控制方式 目前有10种类型
#define DARKLINGHTLINE  "DarkLinghtLine"    //多段控制时多段位置
#define DARKLINGHTLINE2 "DarkLinghtLine2"   //多段控制时第二段的终点
#define DARKLINGHTLINE3 "DarkLinghtLine3"   //多段控制时第二段的终点
#define DARKLINGHTLINE4 "DarkLinghtLine4"   //多段控制时第二段的终点
#define DARKLINGHTLINE5 "DarkLinghtLine5"   //多段控制时第二段的终点
#define DARKLINGHTLINE6 "DarkLinghtLine6"   //多段控制时第二段的终点
#define DARKLINGHTLINE7 "DarkLinghtLine7"   //多段控制时第二段的终点
#define DARKLINGHTLINE8 "DarkLinghtLine8"   //多段控制时第二段的终点
#define DARKLINGHTLINE9 "DarkLinghtLine9"   //多段控制时第二段的终点
/**********************************光源电流配置项***********************************************/
#define INITLINGHTELE_FILE  "initLinghtEle.ini"
#define KEY_INIT            "INIT"       //址地位
#define KEY_PROJADDR        "ProjAddr"       //址地位
#define DUTYCYCLEA1_ELE_F   "DutyCycleA1_ELE_F"  //
#define DUTYCYCLEA2_ELE_F   "DutyCycleA2_ELE_F"
#define DUTYCYCLEA3_ELE_F   "DutyCycleA3_ELE_F"
#define DUTYCYCLEA4_ELE_F   "DutyCycleA4_ELE_F"
#define DUTYCYCLEB1_ELE_F   "DutyCycleB1_ELE_F"  //
#define DUTYCYCLEB2_ELE_F   "DutyCycleB2_ELE_F"
#define DUTYCYCLEB3_ELE_F   "DutyCycleB3_ELE_F"
#define DUTYCYCLEB4_ELE_F   "DutyCycleB4_ELE_F"
#define DUTYCYCLEB5_ELE_F   "DutyCycleB5_ELE_F"
#define DUTYCYCLEB6_ELE_F   "DutyCycleB6_ELE_F"
#define DUTYCYCLEC1_ELE_F   "DutyCycleC1_ELE_F"  //
#define DUTYCYCLEC2_ELE_F   "DutyCycleC2_ELE_F"
#define DUTYCYCLEC3_ELE_F   "DutyCycleC3_ELE_F"
#define DUTYCYCLED1_ELE_F   "DutyCycleD1_ELE_F"  //
#define DUTYCYCLED2_ELE_F   "DutyCycleD2_ELE_F"
#define DUTYCYCLED3_ELE_F   "DutyCycleD3_ELE_F"

#define DUTYCYCLEB1_ELE_R   "DutyCycleB1_ELE_R"  //
#define DUTYCYCLEB2_ELE_R   "DutyCycleB2_ELE_R"
#define DUTYCYCLEB3_ELE_R   "DutyCycleB3_ELE_R"
#define DUTYCYCLEB4_ELE_R   "DutyCycleB4_ELE_R"
#define DUTYCYCLEB5_ELE_R   "DutyCycleB5_ELE_R"
#define DUTYCYCLEB6_ELE_R   "DutyCycleB6_ELE_R"
#define DUTYCYCLEC1_ELE_R   "DutyCycleC1_ELE_R"  //
#define DUTYCYCLEC2_ELE_R   "DutyCycleC2_ELE_R"
#define DUTYCYCLEC3_ELE_R   "DutyCycleC3_ELE_R"
#define DUTYCYCLED1_ELE_R   "DutyCycleD1_ELE_R"  //
#define DUTYCYCLED2_ELE_R   "DutyCycleD2_ELE_R"
#define DUTYCYCLED3_ELE_R   "DutyCycleD3_ELE_R"

#define LIGHTPOWERA_F    "LightPowerA_F"
#define LIGHTPOWERB_F    "LightPowerB_F"
#define LIGHTPOWERC_F    "LightPowerC_F"
#define LIGHTPOWERD_F    "LightPowerD_F"

#define LIGHTPOWERB_R    "LightPowerB_R"
#define LIGHTPOWERC_R    "LightPowerC_R"
#define LIGHTPOWERD_R    "LightPowerD_R"

#define SV_ON_F           "SV_ON_F"
#define SV_ON_R           "SV_ON_R"

/*****************相机运动参数配置************************/
#define USERSETTING_CAMERA_CFG_FILE             "usercamerasetting.ini"

#define CAMERAPARAM_SPEED             "CameraParam","dSpeed"  ////速度
#define CAMERAPARAM_MOVEUNIT          "CameraParam","moveunit"  ////精度

#define CAMERAPARAM_DGETIMAGELEN      "CameraParam","dGetImageLen"  //视野长度
#define CAMERAPARAM_PLUSEWIDGH        "CameraParam","plusewidgh" //脉宽

#define CAMERAPARAM_FSTARTPOS         "CameraParam","Fstartpos"  //启动延时
#define CAMERAPARAM_MOVELENTH         "CameraParam","MoveLenth"  //移动距离
#define CAMERAPARAM_FIVS              "CameraParam","Fivs"     //触发补偿
#define CAMERAPARAM_CAMERA_K         "CameraParam","Camera_k"  //几K相机

#define CAMERAPARAM_FIVSCOUNT         "CameraParam","Fivscount"  //单张图像行数
#define CAMERAPARAM_ISEQUENCE         "CameraParam","iSequence"  //时序
#define CAMERAPARAM_FIVS              "CameraParam","Fivs"       //补偿触发次数
#define CAMERAPARAM_NORMAL_TIME        "CameraParam","normal_time"       //
#define CAMERAPARAM_BUSY_TIME          "CameraParam","busy_time"       //
#define CAMERAPARAM_BUSY_MARK          "CameraParam","busy_mark"       //
#define CAMERAPARAM_CURPRONUMBER          "CameraParam","CurProNumber"       //

#define IMAGE_PARAM_SAVE_SIMAGE_ENABLE     "ImageParam", "EnableSaveSI"     //使能保存原始文件
#define IMAGE_PARAM_STATIC_ENABLE          "ImageParam", "EnableStatic"     //使能静态校准
#define IMAGE_PARAM_STATIC_SAVE_ENABLE     "ImageParam", "EnableSaveStatic" //使能静态校准图片保存
#define IMAGE_PARAM_DYNAMIC_ENABLE         "ImageParam", "EnableDynamic"    //使能动态校准
#define IMAGE_PARAM_DYNAMIC_SAVE_ENABLE    "ImageParam", "EnableSaveDynamic"    //使能动态校准图片保存
#define IMAGE_PARAM_FILTER_ENABLE          "ImageParam", "EnableFilter"     //使能信号滤波校准
#define IMAGE_PARAM_FILTER_SAVE_ENABLE     "ImageParam", "EnableSaveFilter" //使能信号滤波校准图片保存
#define IMAGE_PARAM_THRESHOLD_ENABLE       "ImageParam", "EnableThreshold"     //使能阈值分割
#define IMAGE_PARAM_COMPOUND_ENABLE        "ImageParam", "EnableCompound"     //使能合并
#define IMAGE_PARAM_THRESHOLD_SAVE_ENABLE  "ImageParam", "EnableSaveThreshold"     //使能阈值分割图片保存
#define IMAGE_PARAM_CUT_IMAGE              "ImageParam", "EnableCut"        //使能切图功能
#define IMAGE_PARAM_CUT_SAVE_IMAGE         "ImageParam", "EnableSaveCut"    //使能切图功能图片保存
#define IMAGE_PARAM_SAVE_CORRENT_IMAGE     "ImageParam", "EnableSavePI"     //使能保存校准图像
#define IMAGE_PARAM_DEFECT_DETECT_ENABLE   "ImageParam", "EnableDetect"     //使能缺陷检测
#define IMAGE_PARAM_H_SY_DETECT_ENABLE     "ImageParam", "EnableSYDetect"     //使能H SY算法
#define IMAGE_PARAM_H_ENABLEBUNITE     "ImageParam", "EnableBunite"     //使能拼图
#define IMAGE_PARAM_IS_WHOLE               "ImageParam", "EnbaleWhole"      //是否整体处理, 不分图
#define IMAGE_PARAM_DUL_LINES              "ImageParam", "DulLines"         //叠加行数
#define IMAGE_PARAM_ON_LINE                "ImageParam", "Online"           //在线图片还是离线图片
#define IMAGE_PARAM_INNER_ENABLE           "ImageParam", "EnableInner"     //使能Inner
#define IMAGE_PARAM_CLASSIFICATION_ENABLE  "ImageParam", "EnableClassification"     //使能Classification检测
#define IMAGE_PARAM_EDGEDETECT_ENABLE		"ImageParam", "EnableEdgeDetect"     //使能EdgeDetect检测
#define IMAGE_PARAM_CORNERDETECT_ENABLE		"ImageParam", "EnableCornerDetect"     //使能EdgeDetect检测
#define IMAGE_PARAM_LISTIONHOLE_ENABLE		"ImageParam", "EnableListionHoleDetect"     //使能ListionHole检测
#define IMAGE_PARAM_SAVE_FILE_ENABLE		"ImageParam", "EnableSaveDefectFile"     // 使能缺陷文件保存
#define IMAGE_PARAM_DEFECT_LEVEL_ENABLE		"ImageParam", "EnableDefectLevel"		 // 使能缺陷分级
#define IMAGE_PARAM_SAVE_COMPOUND_ENABLE	"ImageParam", "EnableSaveCompound"		 // 保存合并图像

/*****************参数配置************************/
#define IMAGE_GROUP_NAME        "image"
#define IMAGE_SAVE_PATH         "image", "savepath"              //图像保存路径
#define IMAGE_OFFLINE_PATH      "image", "readpath"              //离线图片加载路径

#define WORK_PRODUCT_SN         "product",  "sn"  

/*****************上下相机关系************************/
#define CORRECTION_X_SCALE     "cor", "xscale"
#define CORRECTION_Y_SCALE     "cor", "yscale"
#define CORRECTION_X_OFFSET    "cor", "xoffset"
#define CORRECTION_Y_OFFSET    "cor", "yoffset"
#define CORRECTION_IS_MIRROR   "cor", "mirror"

#define USERSETTING_CFG_FILE             "usersetting.ini"
/***********************************日志信息**************************************/
#define LOG_SAVE_PARAM_INFO              "log"
#define LOG_SAVE_GRAY_GROUP_NAME         "gray"           //保存灰度值的组名
#define LOG_SAVE_GRAY_ENABLE             "gray","enable"          //保存图像上指定位置像素是否使能
#define LOG_SAVE_GRAY_COUNT              "gray","count"          //每个图要保存的点个数
#define LOG_SAVE_GRAY_POINT              "point"          //没个点的坐标， 用","分割

/**********************************抠图拼图配置项***********************************************/
#define  CUT_OUT_THRESHOLD_E1            "cutout" , "threshold_e1"
#define  CUT_OUT_THRESHOLD_E2            "cutout" , "threshold_e2"
#define  CUT_OUT_THRESHOLD_F1            "cutout" , "threshold_f1"
#define  CUT_OUT_THRESHOLD_F2            "cutout" , "threshold_f2"
#define  CUT_OUT_DILIATION11             "cutout" , "diliation1_1"
#define  CUT_OUT_DILIATION12             "cutout" , "diliation1_2"
#define  CUT_OUT_DILIATION21             "cutout" , "diliation2_1"
#define  CUT_OUT_DILIATION22             "cutout" , "diliation2_2"
#define  CUT_OUT_ADD_VAL1                "cutout" , "addVal1"
#define  CUT_OUT_ADD_VAL2                "cutout" , "addVal2"

#define LOG_SAVE_DEFECT_RESULT_ENABLE    "result","enable"          //保存图像上指定位置像素是否使能

#define PRODUCT_LINE_RUN_MODEL           "flow", "model"             //流水线运行模式
#define PRODUCT_LINE_RUN_MODEL_KEY       "flow"
#define PRODUCT_FLOW_NAMES_1             "flow", "names1"                //流程名称，各流程名称间用 ',' 号分割
#define PRODUCT_FLOW_NAMES_2             "flow", "names2"                //流程名称，各流程名称间用 ',' 号分割

//相机采集图片参数
#define CAM_ATTRIBUTE_FILE_NAME  "camattribute.ini"           // 相机属性文件
#define CAM_ATTRIBUTE_WIDTH      "camattribute", "width"
#define CAM_ATTRIBUTE_HEIGHT     "camattribute", "height"
#define CAM_ATTRIBUTE_BLOCKSIZE  "camattribute", "blocksize"
#define CAM_ATTRIBUTE_EXPOSURE   "camattribute", "exposure"
#define CAM_ATTRIBUTE_GAIN       "camattribute", "gain"

/*----缺陷定义-----*/
#define DEFECT_CFG_FILE                          "defectcfg.ini"
#define DEFECT_MAX_NUMBER		                "defect", "defect_max_number"              //    最个数

// 1 漏光
#define LIGHTLEAK						 "LightLeak"
#define LIGHTLEAK_ENABLE		         "LightLeak", "LightLeak_enable"        //    检测使能
#define LIGHTLEAK_AREA_MAX		         "LightLeak", "LightLeak_area_max"        //    最大面积
#define LIGHTLEAK_LEN_MAX		         "LightLeak", "LightLeak_len_max"         //    最大长度
#define LIGHTLEAK_CH_AREA_MIN		     "LightLeak", "LightLeak_ch_area_min"     //    大于面积-选择一
#define LIGHTLEAK_CH_AREA_MAX		     "LightLeak", "LightLeak_ch_area_max"     //    小于面积-选择一
#define LIGHTLEAK_CH_AREA_NUMBER		 "LightLeak", "LightLeak_ch_area_number"  //    -选择一面积最大个数
#define LIGHTLEAK_CH_LEN_MIN		     "LightLeak", "LightLeak_ch_len_min"      //    大于长度-选择一
#define LIGHTLEAK_CH_LEN_MAX		     "LightLeak", "LightLeak_ch_len_max"      //	小于长度-选择一
#define LIGHTLEAK_CH_LEN_NUMBER		     "LightLeak", "LightLeak_ch_len_number"    //	-选择一长度最大个数
#define LIGHTLEAK_CH_SEC_AREA_MAX		 "LightLeak", "LightLeak_ch_sec_area_max"   //   选择二大于面积
#define LIGHTLEAK_CH_SEC_AREA_NUMBER	 "LightLeak", "LightLeak_ch_sec_area_number"  // 选择二面积最大个数
#define LIGHTLEAK_CH_SEC_LEN		     "LightLeak", "LightLeak_ch_sec_len"         //  选择二大于长度
#define LIGHTLEAK_CH_SEC_LEN_NUMBER		 "LightLeak", "LightLeak_ch_sec_len_number"   // 选择二长度最大个数

// 2 溢墨
#define EXCESSLNK				         "ExcessInk"
#define EXCESSLNK_ENABLE		         "ExcessInk", "ExcessInk_enable"        //    检测使能
#define EXCESSLNK_AREA_MAX		         "ExcessInk", "ExcessInk_area_max"        //    最大面积
#define EXCESSLNK_LEN_MAX		         "ExcessInk", "ExcessInk_len_max"         //    最大长度
#define EXCESSLNK_CH_AREA_MIN		     "ExcessInk", "ExcessInk_ch_area_min"     //    大于面积-选择一
#define EXCESSLNK_CH_AREA_MAX		     "ExcessInk", "ExcessInk_ch_area_max"     //    小于面积-选择一
#define EXCESSLNK_CH_AREA_NUMBER		 "ExcessInk", "ExcessInk_ch_area_number"  //    -选择一面积最大个数
#define EXCESSLNK_CH_LEN_MIN		     "ExcessInk", "ExcessInk_ch_len_min"      //    大于长度-选择一
#define EXCESSLNK_CH_LEN_MAX		     "ExcessInk", "ExcessInk_ch_len_max"      //	小于长度-选择一
#define EXCESSLNK_CH_LEN_NUMBER		     "ExcessInk", "ExcessInk_ch_len_number"    //	-选择一长度最大个数
#define EXCESSLNK_CH_SEC_AREA_MAX		 "ExcessInk", "ExcessInk_ch_sec_area_max"   //   选择二大于面积
#define EXCESSLNK_CH_SEC_AREA_NUMBER	 "ExcessInk", "ExcessInk_ch_sec_area_number"  // 选择二面积最大个数
#define EXCESSLNK_CH_SEC_LEN		     "ExcessInk", "ExcessInk_ch_sec_len"         //  选择二大于长度
#define EXCESSLNK_CH_SEC_LEN_NUMBER		 "ExcessInk", "ExcessInk_ch_sec_len_number"   // 选择二长度最大个数

// 3 异色
#define DISCOLOURATION						 "Discolouration"
#define DISCOLOURATION_ENABLE		         "Discolouration", "Discolouration_enable"        //    检测使能
#define DISCOLOURATION_AREA_MAX		         "Discolouration", "Discolouration_area_max"        //    最大面积
#define DISCOLOURATION_LEN_MAX		         "Discolouration", "Discolouration_len_max"         //    最大长度
#define DISCOLOURATION_CH_AREA_MIN		     "Discolouration", "Discolouration_ch_area_min"     //    大于面积-选择一
#define DISCOLOURATION_CH_AREA_MAX		     "Discolouration", "Discolouration_ch_area_max"     //    小于面积-选择一
#define DISCOLOURATION_CH_AREA_NUMBER		 "Discolouration", "Discolouration_ch_area_number"  //    -选择一面积最大个数
#define DISCOLOURATION_CH_LEN_MIN		     "Discolouration", "Discolouration_ch_len_min"      //    大于长度-选择一
#define DISCOLOURATION_CH_LEN_MAX		     "Discolouration", "Discolouration_ch_len_max"      //	小于长度-选择一
#define DISCOLOURATION_CH_LEN_NUMBER		 "Discolouration", "Discolouration_ch_len_number"    //	-选择一长度最大个数
#define DISCOLOURATION_CH_SEC_AREA_MAX		 "Discolouration", "Discolouration_ch_sec_area_max"   //   选择二大于面积
#define DISCOLOURATION_CH_SEC_AREA_NUMBER	 "Discolouration", "Discolouration_ch_sec_area_number"  // 选择二面积最大个数
#define DISCOLOURATION_CH_SEC_LEN		     "Discolouration", "Discolouration_ch_sec_len"         //  选择二大于长度
#define DISCOLOURATION_CH_SEC_LEN_NUMBER	 "Discolouration", "Discolouration_ch_sec_len_number"   // 选择二长度最大个数

// 4 锯齿
#define SAWEGDGE        	             "SawEgdge"
#define SAWEGDGE_ENABLE		             "SawEgdge", "SawEgdge_enable"        //    检测使能
#define SAWEGDGE_AREA_MAX		         "SawEgdge", "SawEgdge_area_max"        //    最大面积
#define SAWEGDGE_LEN_MAX		         "SawEgdge", "SawEgdge_len_max"         //    最大长度
#define SAWEGDGE_CH_AREA_MIN		     "SawEgdge", "SawEgdge_ch_area_min"     //    大于面积-选择一
#define SAWEGDGE_CH_AREA_MAX		     "SawEgdge", "SawEgdge_ch_area_max"     //    小于面积-选择一
#define SAWEGDGE_CH_AREA_NUMBER		     "SawEgdge", "SawEgdge_ch_area_number"  //    -选择一面积最大个数
#define SAWEGDGE_CH_LEN_MIN		         "SawEgdge", "SawEgdge_ch_len_min"      //    大于长度-选择一
#define SAWEGDGE_CH_LEN_MAX		         "SawEgdge", "SawEgdge_ch_len_max"      //	小于长度-选择一
#define SAWEGDGE_CH_LEN_NUMBER		     "SawEgdge", "SawEgdge_ch_len_number"    //	-选择一长度最大个数
#define SAWEGDGE_CH_SEC_AREA_MAX		 "SawEgdge", "SawEgdge_ch_sec_area_max"   //   选择二大于面积
#define SAWEGDGE_CH_SEC_AREA_NUMBER	     "SawEgdge", "SawEgdge_ch_sec_area_number"  // 选择二面积最大个数
#define SAWEGDGE_CH_SEC_LEN		         "SawEgdge", "SawEgdge_ch_sec_len"         //  选择二大于长度
#define SAWEGDGE_CH_SEC_LEN_NUMBER		 "SawEgdge", "SawEgdge_ch_sec_len_number"   // 选择二长度最大个数

//5 脏污
#define SMIRCH				             "Smirch"
#define SMIRCH_ENABLE		             "Smirch", "Smirch_enable"        //    检测使能
#define SMIRCH_AREA_MAX		             "Smirch", "Smirch_area_max"        //        最大面积
#define SMIRCH_LEN_MAX		             "Smirch", "Smirch_len_max"         //		  最大长度
#define SMIRCH_CH_AREA_MIN		         "Smirch", "Smirch_ch_area_min"     //		  大于面积-选择一
#define SMIRCH_CH_AREA_MAX		         "Smirch", "Smirch_ch_area_max"     //		  小于面积-选择一
#define SMIRCH_CH_AREA_NUMBER		     "Smirch", "Smirch_ch_area_number"  //		  -选择一面积最大个数
#define SMIRCH_CH_LEN_MIN		         "Smirch", "Smirch_ch_len_min"      //		  大于长度-选择一
#define SMIRCH_CH_LEN_MAX		         "Smirch", "Smirch_ch_len_max"      //		  小于长度-选择一
#define SMIRCH_CH_LEN_NUMBER		     "Smirch", "Smirch_ch_len_number"    //		  -选择一长度最大个数
#define SMIRCH_CH_SEC_AREA_MAX		     "Smirch", "Smirch_ch_sec_area_max"   //	   选择二大于面积
#define SMIRCH_CH_SEC_AREA_NUMBER	     "Smirch", "Smirch_ch_sec_area_number"  //	   选择二面积最大个数
#define SMIRCH_CH_SEC_LEN		         "Smirch", "Smirch_ch_sec_len"         //	   选择二大于长度
#define SMIRCH_CH_SEC_LEN_NUMBER		 "Smirch", "Smirch_ch_sec_len_number"   //	   选择二长度最大个数

//6 油墨堆积
#define ACCULNK				             "AccuLnk"
#define ACCULNK_ENABLE		             "AccuLnk", "AccuLnk_enable"        //    检测使能
#define ACCULNK_AREA_MAX		         "AccuLnk", "AccuLnk_area_max"        //        最大面积
#define ACCULNK_LEN_MAX		             "AccuLnk", "AccuLnk_len_max"         //		  最大长度
#define ACCULNK_CH_AREA_MIN		         "AccuLnk", "AccuLnk_ch_area_min"     //		  大于面积-选择一
#define ACCULNK_CH_AREA_MAX		         "AccuLnk", "AccuLnk_ch_area_max"     //		  小于面积-选择一
#define ACCULNK_CH_AREA_NUMBER		     "AccuLnk", "AccuLnk_ch_area_number"  //		  -选择一面积最大个数
#define ACCULNK_CH_LEN_MIN		         "AccuLnk", "AccuLnk_ch_len_min"      //		  大于长度-选择一
#define ACCULNK_CH_LEN_MAX		         "AccuLnk", "AccuLnk_ch_len_max"      //		  小于长度-选择一
#define ACCULNK_CH_LEN_NUMBER		     "AccuLnk", "AccuLnk_ch_len_number"    //		  -选择一长度最大个数
#define ACCULNK_CH_SEC_AREA_MAX		     "AccuLnk", "AccuLnk_ch_sec_area_max"   //	   选择二大于面积
#define ACCULNK_CH_SEC_AREA_NUMBER	     "AccuLnk", "AccuLnk_ch_sec_area_number"  //	   选择二面积最大个数
#define ACCULNK_CH_SEC_LEN		         "AccuLnk", "AccuLnk_ch_sec_len"         //	   选择二大于长度
#define ACCULNK_CH_SEC_LEN_NUMBER		 "AccuLnk", "AccuLnk_ch_sec_len_number"   //	   选择二长度最大个数

//7 孔变形
#define DISTHOLE		             "DistHole"
#define DISTHOLE_ENABLE		             "DistHole", "DistHole_enable"        //    检测使能
#define DISTHOLE_AREA_MAX		         "DistHole", "DistHole_area_max"        //        最大面积
#define DISTHOLE_LEN_MAX		         "DistHole", "DistHole_len_max"         //		  最大长度
#define DISTHOLE_CH_AREA_MIN		     "DistHole", "DistHole_ch_area_min"     //		  大于面积-选择一
#define DISTHOLE_CH_AREA_MAX		     "DistHole", "DistHole_ch_area_max"     //		  小于面积-选择一
#define DISTHOLE_CH_AREA_NUMBER		     "DistHole", "DistHole_ch_area_number"  //		  -选择一面积最大个数
#define DISTHOLE_CH_LEN_MIN		         "DistHole", "DistHole_ch_len_min"      //		  大于长度-选择一
#define DISTHOLE_CH_LEN_MAX		         "DistHole", "DistHole_ch_len_max"      //		  小于长度-选择一
#define DISTHOLE_CH_LEN_NUMBER		     "DistHole", "DistHole_ch_len_number"    //		  -选择一长度最大个数
#define DISTHOLE_CH_SEC_AREA_MAX		 "DistHole", "DistHole_ch_sec_area_max"   //	   选择二大于面积
#define DISTHOLE_CH_SEC_AREA_NUMBER	     "DistHole", "DistHole_ch_sec_area_number"  //	   选择二面积最大个数
#define DISTHOLE_CH_SEC_LEN		         "DistHole", "DistHole_ch_sec_len"         //	   选择二大于长度
#define DISTHOLE_CH_SEC_LEN_NUMBER		 "DistHole", "DistHole_ch_sec_len_number"   //	   选择二长度最大个数

//8 漏印
#define MISPRINT			             "MisPrint"
#define MISPRINT_ENABLE		             "MisPrint", "MisPrint_enable"        //    检测使能
#define MISPRINT_AREA_MAX		         "MisPrint", "MisPrint_area_max"        //        最大面积
#define MISPRINT_LEN_MAX		         "MisPrint", "MisPrint_len_max"         //		  最大长度
#define MISPRINT_CH_AREA_MIN		     "MisPrint", "MisPrint_ch_area_min"     //		  大于面积-选择一
#define MISPRINT_CH_AREA_MAX		     "MisPrint", "MisPrint_ch_area_max"     //		  小于面积-选择一
#define MISPRINT_CH_AREA_NUMBER		     "MisPrint", "MisPrint_ch_area_number"  //		  -选择一面积最大个数
#define MISPRINT_CH_LEN_MIN		         "MisPrint", "MisPrint_ch_len_min"      //		  大于长度-选择一
#define MISPRINT_CH_LEN_MAX		         "MisPrint", "MisPrint_ch_len_max"      //		  小于长度-选择一
#define MISPRINT_CH_LEN_NUMBER		     "MisPrint", "MisPrint_ch_len_number"    //		  -选择一长度最大个数
#define MISPRINT_CH_SEC_AREA_MAX		 "MisPrint", "MisPrint_ch_sec_area_max"   //	   选择二大于面积
#define MISPRINT_CH_SEC_AREA_NUMBER	     "MisPrint", "MisPrint_ch_sec_area_number"  //	   选择二面积最大个数
#define MISPRINT_CH_SEC_LEN		         "MisPrint", "MisPrint_ch_sec_len"         //	   选择二大于长度
#define MISPRINT_CH_SEC_LEN_NUMBER		 "MisPrint", "MisPrint_ch_sec_len_number"   //	   选择二长度最大个数

 // 9 刮伤
#define SCRATCHES				         "Scratches"
#define SCRATCHES_ENABLE		         "Scratches", "Scratches_enable"        //    检测使能
#define SCRATCHES_AREA_MAX		         "Scratches", "Scratches_area_max"        //    最大面积
#define SCRATCHES_LEN_MAX		         "Scratches", "Scratches_len_max"         //    最大长度
#define SCRATCHES_CH_AREA_MIN		     "Scratches", "Scratches_ch_area_min"     //    大于面积-选择一
#define SCRATCHES_CH_AREA_MAX		     "Scratches", "Scratches_ch_area_max"     //    小于面积-选择一
#define SCRATCHES_CH_AREA_NUMBER		 "Scratches", "Scratches_ch_area_number"  //    -选择一面积最大个数
#define SCRATCHES_CH_LEN_MIN		     "Scratches", "Scratches_ch_len_min"      //    大于长度-选择一
#define SCRATCHES_CH_LEN_MAX		     "Scratches", "Scratches_ch_len_max"      //	小于长度-选择一
#define SCRATCHES_CH_LEN_NUMBER		     "Scratches", "Scratches_ch_len_number"    //	-选择一长度最大个数
#define SCRATCHES_CH_SEC_AREA_MAX		 "Scratches", "Scratches_ch_sec_area_max"   //   选择二大于面积
#define SCRATCHES_CH_SEC_AREA_NUMBER	 "Scratches", "Scratches_ch_sec_area_number"  // 选择二面积最大个数
#define SCRATCHES_CH_SEC_LEN		     "Scratches", "Scratches_ch_sec_len"         //  选择二大于长度
#define SCRATCHES_CH_SEC_LEN_NUMBER		 "Scratches", "Scratches_ch_sec_len_number"   // 选择二长度最大个数


//10 崩边
#define CHIPS				             "Chips"
#define CHIPS_ENABLE		             "Chips", "Chips_enable"        //    检测使能
#define CHIPS_AREA_MAX		             "Chips", "Chips_area_max"        //        最大面积
#define CHIPS_LEN_MAX		             "Chips", "Chips_len_max"         //		最大长度
#define CHIPS_CH_AREA_MIN		         "Chips", "Chips_ch_area_min"     //		大于面积-选择一
#define CHIPS_CH_AREA_MAX		         "Chips", "Chips_ch_area_max"     //		小于面积-选择一
#define CHIPS_CH_AREA_NUMBER		     "Chips", "Chips_ch_area_number"  //		-选择一面积最大个数
#define CHIPS_CH_LEN_MIN		         "Chips", "Chips_ch_len_min"      //		大于长度-选择一
#define CHIPS_CH_LEN_MAX		         "Chips", "Chips_ch_len_max"      //		小于长度-选择一
#define CHIPS_CH_LEN_NUMBER		         "Chips", "Chips_ch_len_number"    //		-选择一长度最大个数
#define CHIPS_CH_SEC_AREA_MAX		     "Chips", "Chips_ch_sec_area_max"   //		 选择二大于面积
#define CHIPS_CH_SEC_AREA_NUMBER	     "Chips", "Chips_ch_sec_area_number"  //	 选择二面积最大个数
#define CHIPS_CH_SEC_LEN		         "Chips", "Chips_ch_sec_len"         //		 选择二大于长度
#define CHIPS_CH_SEC_LEN_NUMBER		     "Chips", "Chips_ch_sec_len_number"   //	 选择二长度最大个数

//11 凹凸
#define DENT			                 "Dent"
#define DENT_ENABLE		                 "Dent", "Dent_enable"        //          检测使能
#define DENT_AREA_MAX		             "Dent", "Dent_area_max"        //        最大面积
#define DENT_LEN_MAX		             "Dent", "Dent_len_max"         //		  最大长度
#define DENT_CH_AREA_MIN		         "Dent", "Dent_ch_area_min"     //		  大于面积-选择一
#define DENT_CH_AREA_MAX		         "Dent", "Dent_ch_area_max"     //		  小于面积-选择一
#define DENT_CH_AREA_NUMBER		         "Dent", "Dent_ch_area_number"  //		  -选择一面积最大个数
#define DENT_CH_SEC_AREA_MAX		     "Dent", "Dent_ch_sec_area_max"   //	  选择二大于面积
#define DENT_CH_SEC_AREA_NUMBER	         "Dent", "Dent_ch_sec_area_number"  //	  选择二面积最大个数
//
/***********************************SetupUI 页面用到的配置信息**************************************/
#define SETUPUI_FILE_NAME					"setupui.ini"  

#define SETUPUI_FILTER_DEFECT_ENABLE1		"filter","defect_enable1"           // 使能 缺陷1
#define SETUPUI_FILTER_DEFECT_ENABLE2		"filter","defect_enable2"           // 使能 缺陷2
#define SETUPUI_FILTER_DEFECT_ENABLE3		"filter","defect_enable3"           // 使能 缺陷3
#define SETUPUI_FILTER_DEFECT_ENABLE4		"filter","defect_enable4"           // 使能 缺陷4
#define SETUPUI_FILTER_DEFECT_ENABLE5		"filter","defect_enable5"           // 使能 缺陷5
#define SETUPUI_FILTER_DEFECT_ENABLE6		"filter","defect_enable6"           // 使能 缺陷6
#define SETUPUI_FILTER_DEFECT_ENABLE7		"filter","defect_enable7"           // 使能 缺陷7
#define SETUPUI_FILTER_DEFECT_ENABLE8		"filter","defect_enable8"           // 使能 缺陷8
#define SETUPUI_FILTER_DEFECT_ENABLE9		"filter","defect_enable9"           // 使能 缺陷9
#define SETUPUI_FILTER_DEFECT_ENABLE10		"filter","defect_enable10"          // 使能 缺陷10
#define SETUPUI_FILTER_DEFECT_ENABLE11		"filter","defect_enable11"          // 使能 缺陷11

#define SETUPUI_FILTER_SEVERITY_ENABLE1		"filter","Severity_enable1"           // 使能 缺陷Severity1
#define SETUPUI_FILTER_SEVERITY_ENABLE2		"filter","Severity_enable2"           // 使能 缺陷Severity2
#define SETUPUI_FILTER_SEVERITY_ENABLE3		"filter","Severity_enable3"           // 使能 缺陷Severity3
#define SETUPUI_FILTER_SEVERITY_ENABLE4		"filter","Severity_enable4"           // 使能 缺陷Severity4
#define SETUPUI_FILTER_SEVERITY_ENABLE5		"filter","Severity_enable5"           // 使能 缺陷Severity5

#define SETUPUI_FILTER_OK_ENABLE			"filter","OK_enable"						 // 使能 OK
#define SETUPUI_FILTER_OK2_ENABLE			"filter","OK2_enable"						 // 使能 OK2
#define SETUPUI_FILTER_OK3_ENABLE			"filter","OK3_enable"						 // 使能 OK3
#define SETUPUI_FILTER_NG_ENABLE			"filter","NG_enable"						 // 使能 NG

/***********************************缺陷分类**************************************/
#define DEFECT_SEVERITY_LEVEL_FILE_NAME    "severity.ini"                       // 缺陷分类group后缀, 防止与其他已有命名大小写不一致导致冲突
#define DEFECT_SEVERITY_LEVEL_SUFFIX       "_Severity"                       // 缺陷分类group后缀, 防止与其他已有命名大小写不一致导致冲突
#define DEFECT_SEVERITY_LEVEL_ENABLE       "Severity_Enable"                // 是否使能
#define DEFECT_SEVERITY_LEVEL_DES          "Severity_Des"                   // 缺陷描述
#define DEFECT_SEVERITY_LEVEL_NATURE       "Severity_Nature"                // 属性分类
#define DEFECT_SEVERITY_LEVEL_1ST          "Severity_1st"                   // 第一级
#define DEFECT_SEVERITY_LEVEL_2ND          "Severity_2nd"                   // 第二级
#define DEFECT_SEVERITY_LEVEL_3RD          "Severity_3rd"                   // 第三级
#define DEFECT_SEVERITY_LEVEL_4TH          "Severity_4th"                   // 第四级
#define DEFECT_SEVERITY_LEVEL_5TH          "Severity_5th"                   // 第五级

/**********************************产品分类**************************************/
#define DEFECT_QUALIFICATION_FILE         "qualification.ini"                       // 缺陷分类group后缀, 防止与其他已有命名大小写不一致导致冲突
#define DEFECT_QUALIFICATION_SUFFIX       "_Qualification"                       // 缺陷分类group后缀, 防止与其他已有命名大小写不一致导致冲突
#define DEFECT_QUALIFICATION_ENABLE       "Qualification_Enable"                // 是否使能
#define DEFECT_QUALIFICATION_DES          "Qualification_Des"                   // 缺陷描述
#define DEFECT_QUALIFICATION_1ST          "Qualification_1st"                   // 第一级缺陷个数
#define DEFECT_QUALIFICATION_2ND          "Qualification_2nd"                   // 第二级缺陷个数
#define DEFECT_QUALIFICATION_3RD          "Qualification_3rd"                   // 第三级缺陷个数
#define DEFECT_QUALIFICATION_4TH          "Qualification_4th"                   // 第四级缺陷个数
#define DEFECT_QUALIFICATION_5TH          "Qualification_5th"                   // 第五级缺陷个数

#define QUALIFICATION_ENABLE_OK					"General", "enable_ok"
#define QUALIFICATION_ENABLE_OK2				"General","enable_ok2"
#define QUALIFICATION_ENABLE_OK3				"General","enable_ok3"
#define QUALIFICATION_ENABLE_NG					"General","enable_ng"

#define QUALIFICATION_ENABLE_LIGHTLEAK			"General","enable_LightLeak"
#define QUALIFICATION_ENABLE_EXCESSINK			"General","enable_ExcessInk"
#define QUALIFICATION_ENABLE_DISCOLOURATION		"General","enable_Discolouration"
#define QUALIFICATION_ENABLE_SAWEGDGE			"General","enable_SawEgdge"
#define QUALIFICATION_ENABLE_SMIRCH				"General","enable_Smirch"
#define QUALIFICATION_ENABLE_ACCULNK			"General","enable_AccuLnk"
#define QUALIFICATION_ENABLE_DISTHOLE			"General","enable_DistHole"
#define QUALIFICATION_ENABLE_MISPRINT			"General","enable_MisPrint"
#define QUALIFICATION_ENABLE_SCRATCHES			"General","enable_Scratches"
#define QUALIFICATION_ENABLE_CHIPS				"General","enable_Chips"
#define QUALIFICATION_ENABLE_DENT				"General","enable_Dent"
#define QUALIFICATION_ENABLE_IMPRESS			"General","enable_Impress"
#define QUALIFICATION_ENABLE_RIPPLES			"General","enable_Ripples"
#define QUALIFICATION_ENABLE_FALSESMIRCH				"General","enable_FalseSmirch"
#define QUALIFICATION_ENABLE_FALSESCRATCHES				"General","enable_FalseScratches"

#define QUALIFICATION_ENABLE_LEVEL0				"General","enable_level0"
#define QUALIFICATION_ENABLE_LEVEL1				"General","enable_level1"
#define QUALIFICATION_ENABLE_LEVEL2				"General","enable_level2"
#define QUALIFICATION_ENABLE_LEVEL3				"General","enable_level3"
#define QUALIFICATION_ENABLE_LEVEL4				"General","enable_level4"
#define QUALIFICATION_ENABLE_LEVEL5				"General","enable_level5"
#define QUALIFICATION_ENABLE_LEVEL6				"General","enable_level6"
#define QUALIFICATION_ENABLE_LEVEL7				"General","enable_level7"

#define QUALIFICATION_ENABLE_NG_ABORT			"General","enable_ng_abort"
#define QUALIFICATION_ENABLE_CNT_ABORT			"General","enable_cnt_abort"
#define QUALIFICATION_ENABLE_TIMEOUT_ABORT		"General","enable_timeout_abort"
#define QUALIFICATION_CNT_ABORT					"General","cnt_abort"
#define QUALIFICATION_TIMEOUT_ABORT				"General","timeout_abort"

#define  QUALIFICATION_TOPLIMITCFG_SMIRCHCNT			"ToplimitCfg", "SmirchCnt"		// 1 脏污 个数上限
#define  QUALIFICATION_TOPLIMITCFG_HAIRCNT				"ToplimitCfg", "HairCnt"		// 2 毛发 个数上限
#define  QUALIFICATION_TOPLIMITCFG_DIRTPITSCNT			"ToplimitCfg", "DirtpitsCnt"	// 3 尘点 个数上限
#define  QUALIFICATION_TOPLIMITCFG_SMIRCHAREA			"ToplimitCfg", "SmirchArea"		// 1 脏污 面积上限
#define  QUALIFICATION_TOPLIMITCFG_HAIRAREA				"ToplimitCfg", "HairArea"		// 2 毛发 面积上限
#define  QUALIFICATION_TOPLIMITCFG_DIRTAREA				"ToplimitCfg", "DirtArea"		// 3 尘点 面积上限




#define NOTIFY_PROGRAM_NEW                "program_new"                   //配置操作消息 -- 新建
#define NOTIFY_PROGRAM_OPEN               "program_open"                  //配置操作消息 -- 打开 
#define NOTIFY_PROGRAM_SAVE               "program_save"                  //配置操作消息 -- 保存

#define NOTIFY_RESET_UPDATE_DECT        "Notify_update_dect"            //更新产品长度
#define NOTIFY_PRODUCT                  "NOTIFY_Product"                //发送数据到统计Ui


#define NOTIFY_UPDATE_H_PARAM              "update_h_param"              //更新H算法参数， 重新初始化算法

#define PRUDUCT_INFO_FILE                  "productinfo.ini"
#define PRUDUCT_DES                        "pruduct", "des"

/******************************  皮带modbus地址 配置项 ********************************/
#define CONVERY_MODBUS_FILE         "modbusaddress.ini"                 // 配置文件名
#define ADDRESS                     "address"                           // 地址group
#define SET_TEMPERATURE1            "gettemperature1"					// 温度1
#define SET_TEMPERATURE2            "gettemperature2"					// 温度2
#define SET_TEMPERATURE3            "gettemperature3"					// 温度3
#define SET_TEMPERATURE4            "gettemperature4"					// 温度4
#define SET_TEMPERATURE5            "gettemperature5"					// 温度5
#define SET_TEMPERATURE6            "gettemperature6"					// 温度6
#define SET_TEMPERATURE7            "gettemperature7"					// 温度7
#define SET_TEMPERATURE8            "gettemperature8"					// 温度8
#define GET_INPUT                   "getinput"					        // 16路IO输入
#define SET_OUTPUT                  "setoutput"					        // 16路IO输出
#define GET_CMDPOS                  "getcmdpos"					        // 获取指令位置
#define GET_ACTUAL_POS              "getactualpos"					    // 获取实际位置
#define SET_ACC                     "setacc"					        // 设置加速度
#define SET_DEC                     "setdec"					        // 设置减速度
#define SET_VEL                     "setvel"					        // 设置速度
#define SET_BEGIN_VEL               "setbeginvel"				        // 设置初始速度
#define SET_TRIGGER_INTERVAL        "settriggerinterval"				// 设置光源触发间隔
#define SET_TRIGGER_DELAY           "settriggerdelay"				    // 设置光源触发延时
#define SET_TRIGGER_COUNT           "settriggercount"				    // 设置光源触发次数
#define SET_PULSECOUNT              "setpulsecount"					    // 设置定量运动脉冲数
#define SET_DRIVERMODE              "setdrivemode"					    // 设置驱动模式
#define SET_DRIVECMD                "setdrivecmd"					    // 设置驱动命令(启动/停止)
#define SET_TRIGGERAUTO             "settriggerauto"                    // 设置自动触发(启动/停止)
#define GET_TRIGGER_FINISH          "gettriggerfinish"                  // 触发完成

// 光源电流定义
struct LightEleValue
{
    unsigned short project_addr;
	unsigned short f_electricity[4][6];  // 每个通道中每个模组的电流值
	unsigned short r_electricity[4][6];  // 每个通道中每个模组的电流值
	unsigned short f_on;              // 
	unsigned short r_on;              // 
	unsigned short f_power[4];              // 
	unsigned short r_power[4];              // 
	LightEleValue()
	{
        project_addr = 2001;
		memset(f_electricity, 0, sizeof(f_electricity));
		memset(r_electricity, 0, sizeof(r_electricity));
        f_on = 0;
        r_on = 0;
		memset(f_power, 0, sizeof(f_power));
		memset(r_power, 0, sizeof(r_power));
    }
    LightEleValue(const LightEleValue &other)
    {
        project_addr = other.project_addr;
        memcpy(f_electricity, other.f_electricity, sizeof(f_electricity));
        memcpy(r_electricity, other.r_electricity, sizeof(r_electricity));
        f_on = other.f_on;
        r_on = other.r_on;
        memcpy(f_power, other.f_power, sizeof(f_power));
        memcpy(r_power, other.r_power, sizeof(r_power));
    }
    LightEleValue operator=(const LightEleValue &other)
    {
        project_addr = other.project_addr;
        memcpy(f_electricity, other.f_electricity, sizeof(f_electricity));
        memcpy(r_electricity, other.r_electricity, sizeof(r_electricity));
        f_on = other.f_on;
        r_on = other.r_on;
        memcpy(f_power, other.f_power, sizeof(f_power));
        memcpy(r_power, other.r_power, sizeof(r_power));
        return *this;
    }
};
#define  SECTION_COUNT   10
#define  U_MUT_SECTION_COUNT          12
//配方
struct IllumChannelValue
{
	// 上面通道
    unsigned short project_addr;
    unsigned short project_d_addr;
	unsigned short f_intensity[4];      // 亮度
	unsigned short f_start[4];            // 开始时间
	unsigned short f_duty[4];            // 脉宽时间
	unsigned short f_enable;             // 光源是否是能
										 //unsigned short f_electricity[4][6];  // 每个通道中每个模组的电流值
	unsigned short f_cdelay[4];          // 相机触发延时
	unsigned short camera_enable;            // 相机是否使能  
										//下面通道
	unsigned short r_intensity[4];      // 亮度
	unsigned short r_start[4];           // 开始时间
	unsigned short r_duty[4];            // 脉宽时间
	unsigned short r_enable;          // 是否是能
										 // unsigned short r_electricity[4][6];  // 每个通道中每个模组的电流值
	unsigned short r_cdelay[4];          // 相机触发延时
    unsigned short f_draklightmodel[U_MUT_SECTION_COUNT];    // 暗场光源的打光模式
    unsigned short f_sectionlines[U_MUT_SECTION_COUNT];   // 分段切换的位置
	IllumChannelValue()
	{
        project_addr = 2017;
        project_d_addr = 2017;
		memset(f_intensity, 0, sizeof(f_intensity));
		memset(f_start, 0, sizeof(f_start));
		memset(f_duty, 0, sizeof(f_duty));
		//memset(f_enable, 0, sizeof(f_enable));
        f_enable = 0;
		//memset(f_electricity,0,sizeof(f_electricity));
		memset(f_cdelay, 0, sizeof(f_cdelay));
		camera_enable = 0;

		memset(r_intensity, 0, sizeof(r_intensity));
		memset(r_start, 0, sizeof(r_start));
		memset(r_duty, 0, sizeof(r_duty));
        r_enable = 0;
		// memset(r_electricity,0,sizeof(r_electricity));
        memset(r_cdelay, 0, sizeof(r_cdelay));
        memset(f_draklightmodel, 0, sizeof(f_draklightmodel));
        memset(f_sectionlines, 0, sizeof(f_sectionlines));
	}
	void clearValue()
	{
        project_addr = 2017;
        project_d_addr = 2017;
		memset(f_intensity, 0, sizeof(f_intensity));
		memset(f_duty, 0, sizeof(f_duty));
		memset(f_start, 0, sizeof(f_start));
		f_enable = 0;
		//memset(f_electricity,0,sizeof(f_electricity));
        memset(f_cdelay, 0, sizeof(f_cdelay));
        camera_enable = 0;

		memset(r_intensity, 0, sizeof(r_intensity));
		memset(r_duty, 0, sizeof(r_duty));
        memset(r_start, 0, sizeof(r_start));
        r_enable = 0;
		//  memset(r_electricity,0,sizeof(r_electricity));
        memset(r_cdelay, 0, sizeof(r_cdelay));
        memset(f_draklightmodel, 0, sizeof(f_draklightmodel));
        memset(f_sectionlines, 0, sizeof(f_sectionlines));
	}
    IllumChannelValue(const IllumChannelValue &other)
    {
        project_addr = other.project_addr;
        project_d_addr = other.project_d_addr;
        memcpy(f_intensity, other.f_intensity, sizeof(f_intensity));
        memcpy(f_start, other.f_start, sizeof(f_start));
        memcpy(f_duty, other.f_duty, sizeof(f_duty));
        f_enable = other.f_enable;
        memcpy(f_cdelay, other.f_cdelay, sizeof(f_cdelay));
        camera_enable = other.camera_enable;

        memcpy(r_intensity, other.r_intensity, sizeof(r_intensity));
        memcpy(r_start, other.r_start, sizeof(r_start));
        memcpy(r_duty, other.r_duty, sizeof(r_duty));
        r_enable = other.r_enable;
        memcpy(r_cdelay, other.r_cdelay, sizeof(r_cdelay));
        memcpy(f_draklightmodel, other.f_draklightmodel, sizeof(f_draklightmodel));
        memcpy(f_sectionlines, other.f_sectionlines, sizeof(f_sectionlines));
    }

    IllumChannelValue operator=(const IllumChannelValue &other)
    {
        project_addr = other.project_addr;
        project_d_addr = other.project_d_addr;
        memcpy(f_intensity, other.f_intensity, sizeof(f_intensity));
        memcpy(f_start, other.f_start, sizeof(f_start));
        memcpy(f_duty, other.f_duty, sizeof(f_duty));
        f_enable = other.f_enable;
        memcpy(f_cdelay, other.f_cdelay, sizeof(f_cdelay));
        camera_enable = other.camera_enable;

        memcpy(r_intensity, other.r_intensity, sizeof(r_intensity));
        memcpy(r_start, other.r_start, sizeof(r_start));
        memcpy(r_duty, other.r_duty, sizeof(r_duty));
        r_enable = other.r_enable;
        memcpy(r_cdelay, other.r_cdelay, sizeof(r_cdelay));
        memcpy(f_draklightmodel, other.f_draklightmodel, sizeof(f_draklightmodel));
        memcpy(f_sectionlines, other.f_sectionlines, sizeof(f_sectionlines));
        return *this;
    }
};
/*----R算法缺陷分类-----*/
#define R_TYPE_CFG_FILE                               "RType.ini"
#define RTYPE_MINAREA		                          "rtype", "minArea"        
#define RTYPE_maxArea		                          "rtype", "maxArea"   
#define RTYPE_minRect2DensityBrightPrepR		      "rtype", "minRect2DensityBrightPrepR"   
#define RTYPE_GeometricLengthBrightPrepR		      "rtype", "GeometricLengthBrightPrepR"   
#define RTYPE_maxCircleDensityBrightPrepR		      "rtype", "maxCircleDensityBrightPrepR"   
#define RTYPE_maxSub1stCircleDensityBrightPrepR		  "rtype", "maxSub1stCircleDensityBrightPrepR"  
#define RTYPE_GrayMeanGvBrightPrepB		              "rtype", "GrayMeanGvBrightPrepB" 
#define RTYPE_GrayMeanGvBrightPrepC		              "rtype", "GrayMeanGvBrightPrepC"  
#define RTYPE_GeometricRect2SlimnessBrightPrepR		  "rtype", "GeometricRect2SlimnessBrightPrepR" 
#define RTYPE_ListionHoleSize		                  "rtype", "ListionHoleSize" 

#define IFLAG_ENABLE_LIGHTA 0x01 //->A模组使能
#define IFLAG_ENABLE_LIGHTB 0x02 //->B模组使能
#define IFLAG_ENABLE_LIGHTC 0x04 //->C模组使能
#define IFLAG_ENABLE_LIGHTD 0x08 //->D模组使能
#define IFLAG_ENABLE_CAM1   0x10 //->相机1触发
#define IFLAG_ENABLE_CAM2   0x20 //->相机2触发
#define IFLAG_ENABLE_CAM3   0x40 //->相机3触发
#define IFLAG_ENABLE_CAM4   0x80 //->相机4触发 

//
#endif


