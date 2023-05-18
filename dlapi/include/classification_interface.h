#ifndef CLASSIFICATION_INTERFACE_H_
#define CLASSIFICATION_INTERFACE_H_

#include <thread>
#include "dv.h"


//���ڵ���
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

//bjParam ->�����Ҫ�Ĳ����ṹ�塣
//config_json ->���Ʒ·��
//��ʼ��
DV_CLASSIFICATION_DLL int InitBJParam(dv::BJInputParam* bjParam, const char* product_dir);

//���ڵ��Ա��������Ϣ���ļ�������

DV_CLASSIFICATION_DLL void setImageName(const char* imgName, size_t N, size_t idx);


//���������  ���   ά������
DV_CLASSIFICATION_DLL int classifySiyinDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo, dv::DeformationInfo* deformationInfo, const int* terminal_flag=nullptr);

DV_CLASSIFICATION_DLL int classifySiyinPanelDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, dv::DeformationInfo* deformationInfo, const int* terminal_flag = nullptr);
//��Ŀ
DV_CLASSIFICATION_DLL int classifySiyinLogoDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classifySiyinDetect_horizontal(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);
//���2kͼ��
DV_CLASSIFICATION_DLL int classifySiyin2kDetect(dv::BJInputParam* bjParam,dv::Image* image, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo);
//�ͷž����
DV_CLASSIFICATION_DLL int classifyWhiteDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult,dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);

//DV_CLASSIFICATION_DLL int classifyFskWhiteDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);


DV_CLASSIFICATION_DLL int classify3DWhiteHorizontalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classify3DWhiteVerticalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classify3DSiyinHorizontalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classify3DSiyinVerticalDetect(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, const int* terminal_flag = nullptr);


DV_CLASSIFICATION_DLL void UnInitBJParam(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo,dv::RippleResultInfo* rippleResultInfo,dv::DeformationInfo* deformationInfo);

DV_CLASSIFICATION_DLL void UnInitBJParam_2k(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo);

//��������ͳ�ƹ��ߵĽӿ�
DV_CLASSIFICATION_DLL int classify_silk(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo, dv::DeformationInfo* deformationInfo, const char* mid_dir);


// ���ڸ�ʿ�������ӿ�������ƵĽӿ�
//bjParam ->�����Ҫ�Ĳ����ṹ�塣
// bjParam_size �ṹ���С �ڱ��ӿ��еĴ�С��2 bjparam[0] ��һ�׶�5��ͼ bjParam[1] �ڶ��׶ε�2��ͼ  bjparam[2]  �����׶ε�1��ͼ
//product_dir ->���Ʒ·��
// station_index // �ײ��� Ĭ����0�Ϳ����ˣ�˿ӡ���� 0->ǰ��5��ͼ 1 -> ���1��ͼ 2-> ���2��ͼ
//��ʼ��
DV_CLASSIFICATION_DLL int InitBJParam_FSK(dv::BJInputParam* bjParam,int bjParam_size, const char* product_dir,int station_index = 0);


//��һ�����׶εİײ������ӿ�
DV_CLASSIFICATION_DLL int classifywhite_FSK(dv::BJInputParam* bjParam,int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);

//�����׶εİײ������ӿ�
DV_CLASSIFICATION_DLL int classifywhitesingle_FSK(dv::BJInputParam* bjParam, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo,const int* terminal_flag = nullptr);
//�ͷŽӿ�
DV_CLASSIFICATION_DLL void UnInitBJParam_FSK(dv::BJInputParam* bjParam,int bjParam_size, dv::DefectResultInfo* defectResultInfo, dv::RippleResultInfo* rippleResultInfo);

//�����ο��ͼ�ı�Եϵͳ����������ͼ��C��D
DV_CLASSIFICATION_DLL int GetEdgeSystem(dv::BJInputParam* param,dv::Image* image,int size);


//���� �����ο��ͼ�ı�Եϵͳ �������������ͼ ��λ2��λ3������ͼ
DV_CLASSIFICATION_DLL int GetHorizonEdgeSystem(dv::BJInputParam * bjParam, dv::Image * image, int size);


//ǰ��˿ӡ�������ӿ�
DV_CLASSIFICATION_DLL int classifySilk_FSK(dv::BJInputParam* bjParam, int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);


//���˿ӡ�������ӿڣ�����ͼ��
DV_CLASSIFICATION_DLL int classifySilkSingle_FSK(dv::BJInputParam* bjParam,int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);


//���˿ӡ�������ӿ�(����ͼ)
DV_CLASSIFICATION_DLL int classifySilkTwo_FSK(dv::BJInputParam* bjParam, int bjParam_size, dv::DefectResultInfo* defectResult, dv::RippleResultInfo* rippleResultInfo, const int* terminal_flag = nullptr);

DV_CLASSIFICATION_DLL int classifyFSKSilk_leakLight(dv::Image* image, dv::DefectResultInfo* defectResult);

DV_CLASSIFICATION_DLL void ReleaseDefectResultInfo(dv::DefectResultInfo*defectResult,int defectResult_size);

//std::vector<std::string> image_names;
#endif  // CLASSIFICATION_INTERFACE_H_
