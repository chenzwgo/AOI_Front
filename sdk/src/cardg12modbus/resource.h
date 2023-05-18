#ifndef __RESOUCE_GG_H_
#define __RESOUCE_GG_H_
#include "isdkresource.h"

class Resource :public ISDKResource
{
public:
	Resource(void);
	~Resource(void);

	 bool Init() ;

	 bool UnInit() ;

     virtual void* queryScriptObject(const int iid);

     virtual void* queryPriInterface(const int iid);
private:
	bool m_bInit;
};

#endif
