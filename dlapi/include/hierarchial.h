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
 * @param   data_file   ѵ�������� . csv����ļ����ڲ�����ȱ����������ȣ�ǿ��
 *
 * @return  A HiHandle. �ɹ����ط�������ʧ�ܷ���0
 **************************************************************************************************/

HIERARCHIAL_DLL HiHandle hiearachial_init(const char* data_file);

/**********************************************************************************************//**
 * @fn  HIERARCHIAL_DLL int hiearachial(HiHandle hiHandle, dv::DefectAttr* defectAttr);
 *
 * @brief  �ּ����.
 *
 * @date    2019/12/13
 *
 * @param           hiHandle    The parameter.
 * @param [in,out]  defectAttr  If non-null, the defect attribute.
 *
 * @return  ���طּ��ĵȼ���������ǻ��ˣ��ּ����ص���0.
 **************************************************************************************************/

HIERARCHIAL_DLL int hiearachial(HiHandle hiHandle, dv::DefectAttr* defectAttr);

HIERARCHIAL_DLL void hiearachial_release(HiHandle* handle);

#endif