#ifndef I_IMAGE_LOGIC_MANAGER_H
#define I_IMAGE_LOGIC_MANAGER_H
#include <string>
using namespace std;
struct CImageProduct; 

class IImageLogicManager
{
public:
    ~IImageLogicManager(){};
    virtual void init()	= 0;
    virtual void uninit()	= 0;
    virtual bool setImageParam(int iMinLength = 12800)	= 0;
    virtual bool setProductPtr(CImageProduct *product)	= 0;
    virtual bool startGetImage()	= 0;
    virtual bool isAcqFinished()	= 0;
    virtual bool resetImageProcess()	= 0;
    virtual bool triggerGrabImage()	= 0;
    virtual bool abortGrabImage()	= 0;

};
#endif // I_IMAGE_LOGIC_MANAGER_H
