#pragma once
#ifndef HIEARCHIAL_H_ 
#define HIEARCHIAL_H_
#include "dv.h"
#ifdef __cplusplus
#define DEEP_VISION_EXTERN_C extern "C"
#else
#define DEEP_VISION_EXTERN_C
#endif

#ifdef _WIN32
#ifdef HIERARCHIAL_EXPORTS
#define HIERARCHIAL_DLL DEEP_VISION_EXTERN_C __declspec(dllexport)
#else
#define HIERARCHIAL_DLL DEEP_VISION_EXTERN_C __declspec(dllimport)
#endif
#else
#define HIERARCHIAL_DLL DEEP_VISION_EXTERN_C
#endif

typedef void* HiHandle;

/**********************************************************************************************//**
 * @fn  HIERARCHIAL_DLL HiHandle hiearachial_init(const char* data_file);
 *
 * @brief   Hiearachial init.
 *
 * @date    2019/12/13
 *
 * @param   data_file   训练的数据 . csv表格文件，内部包含缺陷面积，长度，强度
 *
 * @return  A HiHandle. 成功返回非零句柄，失败返回0
 **************************************************************************************************/

HIERARCHIAL_DLL HiHandle hiearachial_init(const char* data_file);

/**********************************************************************************************//**
 * @fn  HIERARCHIAL_DLL int hiearachial(HiHandle hiHandle, dv::DefectAttr* defectAttr);
 *
 * @brief  分级检测.
 *
 * @date    2019/12/13
 *
 * @param           hiHandle    The parameter.
 * @param [in,out]  defectAttr  If non-null, the defect attribute.
 *
 * @return  返回分级的等级，如果不是划伤，分级返回的是0.
 **************************************************************************************************/

HIERARCHIAL_DLL int hiearachial(HiHandle hiHandle, dv::DefectAttr* defectAttr);

HIERARCHIAL_DLL void hiearachial_release(HiHandle* handle);

#endif