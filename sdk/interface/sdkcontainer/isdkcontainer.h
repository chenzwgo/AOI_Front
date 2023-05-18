#ifndef __IRESOURCECONTAINER_H__
#define __IRESOURCECONTAINER_H__
#include "isdkresource.h"

class ISDKContainer
{
public:
    virtual ~ISDKContainer(){};
	virtual long GetResourceCount() = 0 ;

	virtual const char* GetResourceName(const int iResourcePos) = 0 ;

	virtual const char* GetResourceName(const char* pszResourceCLSID) = 0 ;

	virtual const char* GetResourceCLSID(const int iResourcePos) = 0 ;

	virtual unsigned long GetResourceVersion(const int iResourcePos) = 0 ;

	virtual unsigned long GetResourceVersion(const char* pszResourceCLSID) = 0 ;

	virtual const char* GetResourceType(const int iResourcePos) = 0 ;

	virtual const char* GetResourceType(const char* pszResourceCLSID) = 0 ;


        virtual ISDKResource* GetResource(const char* pszResourceCLSID) = 0;

    //virtual ISDKCallInterface* GetInterface(const char* pszResourceCLSID,unsigned int  iInterfaceId = 0) = 0 ;

};
#endif //__IRESOURCECONTAINER_H__

