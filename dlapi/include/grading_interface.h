#pragma once
#include "dv.h"
#ifdef __cplusplus
#define DEEP_VISION_EXTERN_C extern "C"
#else
#define DEEP_VISION_EXTERN_C
#endif

#ifdef _WIN32
#ifdef GRADING_EXPORTS
#define GRADING_DLL DEEP_VISION_EXTERN_C __declspec(dllexport)
#else
#define GRADING_DLL DEEP_VISION_EXTERN_C __declspec(dllimport)
#endif
#else
#define GRADING_DLL DEEP_VISION_EXTERN_C
#endif
typedef void* GradingHandle;

//导入模型数据
GRADING_DLL GradingHandle grading_init(const char* model_file);

//检测等级
//失败返回 -1 成功返回等级数
// handle 导入的模型句柄
// attr 缺陷属性结构体
//  channel_index 通道数， A ->0 ,B-> 1 C -> 2 ...
GRADING_DLL int grading_detect(GradingHandle handle,dv::DefectAttr* attr,int channel_index);


//释放模型句柄
GRADING_DLL void grading_uninit(GradingHandle* handle);
