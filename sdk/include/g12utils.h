#ifndef _G12UTILS_H_
#define _G12UTILS_H_
#include "g12commonexport.h"
#include <string>
#include <vector>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

    RESOURCE_LIBRARY_EXPORT  unsigned char* loadImage(const string& strFileName);

    RESOURCE_LIBRARY_EXPORT  bool getImageSize( const string& strFileName, unsigned int& iWidth, unsigned int& iHeight );

    RESOURCE_LIBRARY_EXPORT int usSleep(int us);//΢����ʱ����ʵ��

    RESOURCE_LIBRARY_EXPORT bool startTrigger(int iNumber, int iplusWidth = 100);//΢����ʱ����ʵ��

    RESOURCE_LIBRARY_EXPORT bool isTriggerFinished();//�����ȴ����ٴ������

#ifdef __cplusplus
}
#endif

#endif _DISPENSERCOMMON_H_

