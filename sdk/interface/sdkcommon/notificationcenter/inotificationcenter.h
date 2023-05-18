#ifndef __INOTIFICATIONCENTERSERVICE_H__
#define __INOTIFICATIONCENTERSERVICE_H__


typedef int (*pFunSelector)(unsigned int enNotifyType, void* pNotifyData);

class INotificationCenter
{
public:
		enum
		{
			IID_NOTIFICATIONCENTER= 0x202
		};
    virtual void addObserver(const char* pName,pFunSelector pfun) = 0;
    virtual void postNotification(const char* pName,unsigned int enNotifyType,void* pNotifyData) = 0;
    virtual void sendNotification(const char* pName,unsigned int enNotifyType,void* pNotifyData) = 0;
    virtual void removeObserver(const char* pName,pFunSelector pfun) = 0;
	  virtual void stopRunNotificationCenter() = 0;
};
#endif  //__INOTIFICATIONCENTERSERVICE_H__

