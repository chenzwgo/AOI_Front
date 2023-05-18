#pragma once
#include <string>
using namespace std;
class IJobCamera
{
public:
	enum
	{
		IID_IJOBCAMERA = 0x300,
	};
	virtual ~IJobCamera(){}

	virtual void SetJobPath(const char* path) = 0;

	virtual bool GetImage() = 0;

	virtual bool GetResult(void **pImageBuffer,unsigned int &iBufferSize) = 0;

	virtual bool GetResult(string &OutImagePath ) = 0;

};