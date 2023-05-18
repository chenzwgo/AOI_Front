#ifndef _ISDKResource_H_
#define _ISDKResource_H_

class ISDKResource
{
public:
        virtual ~ISDKResource() {};

	virtual bool Init() = 0;

	virtual bool UnInit() = 0;

        virtual void* queryScriptObject(const int iid) = 0;

        virtual void* queryPriInterface(const int iid) = 0;
};
#endif
