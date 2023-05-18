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

//����ģ������
GRADING_DLL GradingHandle grading_init(const char* model_file);

//���ȼ�
//ʧ�ܷ��� -1 �ɹ����صȼ���
// handle �����ģ�;��
// attr ȱ�����Խṹ��
//  channel_index ͨ������ A ->0 ,B-> 1 C -> 2 ...
GRADING_DLL int grading_detect(GradingHandle handle,dv::DefectAttr* attr,int channel_index);


//�ͷ�ģ�;��
GRADING_DLL void grading_uninit(GradingHandle* handle);
