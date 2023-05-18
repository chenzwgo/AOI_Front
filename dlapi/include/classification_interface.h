#ifndef CLASSIFICATION_INTERFACE_H_
#define CLASSIFICATION_INTERFACE_H_

#include <thread>
#include "dv.h"


//用于调试
#define WHOLE_PIC 1

#ifdef __cplusplus
#define DEEP_VISION_EXTERN_C extern "C"
#else
#define DEEP_VISION_EXTERN_C
#endif

#ifdef _WIN32
#ifdef CLASSIFICATION_LIB_EXPORTS
#define DV_CLASSIFICATION_DLL DEEP_VISION_EXTERN_C __declspec(dllexport)
#else
#define DV_CLASSIFICATION_DLL DEEP_VISION_EXTERN_C __declspec(dllimport)
#endif
#else
#define DV_CLASSIFICATION_DLL DEEP_VISION_EXTERN_C
#endif

//bjParam ->检测需要的参数结构体。
//config_json ->配产品路径
//初始化
DV_CLASSIFICATION_DLL int InitBJParam(dv::BJInputParam* bjParam, const char* product_dir);

//用于调试保存调试信息的文件名设置

DV_CLASSIFICATION_DLL void setImageName(const char* imgName, size_t N, size_t idx);


//晶博检测检测  信濠   维达力：
DV_CLASSIFICATION_DLL int classifySiyinDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo, dv::DeformationInfo* deformationInfo, const int* terminal_flag=nullptr);

DV_CLASSIFICATION_DLL int classifySiyinPanelDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, dv::DeformationInfo* deformationInfo, const int* terminal_flag = nullptr);
//悦目
DV_CLASSIFICATION_DLL int classifySiyinLogoDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classifySiyinDetect_horizontal(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);
//检测2k图像
DV_CLASSIFICATION_DLL int classifySiyin2kDetect(dv::BJInputParam* bjParam,dv::Image* image, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo);
//释放句柄。
DV_CLASSIFICATION_DLL int classifyWhiteDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult,dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);

//DV_CLASSIFICATION_DLL int classifyFskWhiteDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);


DV_CLASSIFICATION_DLL int classify3DWhiteHorizontalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classify3DWhiteVerticalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classify3DSiyinHorizontalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classify3DSiyinVerticalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);


DV_CLASSIFICATION_DLL void UnInitBJParam(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo,dv::RippleResultInfo* rippleResultInfo,dv::DeformationInfo* deformationInfo);

DV_CLASSIFICATION_DLL void UnInitBJParam_2k(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo);

//用于性能统计工具的接口
DV_CLASSIFICATION_DLL int classify_silk(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo, dv::DeformationInfo* deformationInfo, const char* mid_dir);


// 对于富士康玻璃接口重新设计的接口
//bjParam ->检测需要的参数结构体。
// bjParam_size 结构体大小 在本接口中的大小是2 bjparam[0] 第一阶段5张图 bjParam[1] 第二阶段的2张图  bjparam[2]  第三阶段的1张图
//product_dir ->配产品路径
// station_index // 白玻璃 默认是0就可以了，丝印玻璃 0->前端5张图 1 -> 后端1张图 2-> 后端2张图
//初始化
DV_CLASSIFICATION_DLL int InitBJParam_FSK(dv::BJInputParam* bjParam,int bjParam_size, const char* product_dir,int station_index = 0);


//第一，二阶段的白玻璃检测接口
DV_CLASSIFICATION_DLL int classifywhite_FSK(dv::BJInputParam* bjParam,int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);

//第三阶段的白玻璃检测接口
DV_CLASSIFICATION_DLL int classifywhitesingle_FSK(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo,const int* terminal_flag = nullptr);
//释放接口
DV_CLASSIFICATION_DLL void UnInitBJParam_FSK(dv::BJInputParam* bjParam,int bjParam_size, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo);

//画矩形框的图的边缘系统，传递两张图，C和D
DV_CLASSIFICATION_DLL int GetEdgeSystem(dv::BJInputParam* param,dv::Image* image,int size);


//横向 画矩形框的图的边缘系统 传递正面的亮场图 工位2或工位3的亮场图
DV_CLASSIFICATION_DLL int GetHorizonEdgeSystem(dv::BJInputParam * bjParam, dv::Image * image, int size);


//前端丝印玻璃检测接口
DV_CLASSIFICATION_DLL int classifySilk_FSK(dv::BJInputParam* bjParam, int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);


//后端丝印玻璃检测接口（单张图）
DV_CLASSIFICATION_DLL int classifySilkSingle_FSK(dv::BJInputParam* bjParam,int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);


//后端丝印玻璃检测接口(两张图)
DV_CLASSIFICATION_DLL int classifySilkTwo_FSK(dv::BJInputParam* bjParam, int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classifyFSKSilk_leakLight(dv::Image* image, dv::DefectResultInfo* defectResult);

DV_CLASSIFICATION_DLL void ReleaseDefectResultInfo(dv::DefectResultInfo*defectResult,int defectResult_size);

//std::vector<std::string> image_names;
#endif  // CLASSIFICATION_INTERFACE_H_
