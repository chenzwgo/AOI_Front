#pragma once
#include "IAutoJobItem.h"
class IJobCamera;
class IJobFlowCameraFactory
{
public:
	enum
	{
		IID_IJOBFLOWCAMERAFACTORY = 0x200,//
	};
	virtual ~IJobFlowCameraFactory(){}
	// 获取相机列表
	virtual IAutoJobItem *CreateCameraByPath(const char *JobPath) = 0 ;

	virtual void ReleaseCameraByPath(const char *JobPath) = 0 ;
protected:
private:
};